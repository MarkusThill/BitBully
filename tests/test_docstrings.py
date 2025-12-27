"""Tests that all Python code examples in docstrings execute without error and produce the expected output.

This test module scans docstrings for Markdown fenced code blocks and executes
Python examples. If an example is followed by a fenced non-Python block
(`text`, `txt`, empty language, or `none`), that block is treated as the expected
stdout and is compared against captured output.

It also supports `.pyi` stub docstrings by parsing stub files as text via
`ast.parse` and extracting module/class/function docstrings.

Notes:
    - Code blocks are expected to be fenced with triple backticks.
    - Python blocks use language tags `python` or `py`.
    - Expected output blocks use language tags `text`, `txt`, `none`, or empty.
    - Each Python example runs in a fresh namespace to keep examples independent.
"""

from __future__ import annotations

import ast
import inspect
import re
from contextlib import redirect_stderr, redirect_stdout
from dataclasses import dataclass
from importlib.resources import files
from io import StringIO
from pathlib import Path
from typing import Any

import pytest

from bitbully import BitBully, Board

# ---------- Block parsing ----------

FENCE_RE = re.compile(
    r"""(?mx)                # m: ^/$ are per-line, x: allow comments/spaces
    ^[ \t]*```[ \t]*         # opening fence with optional indentation
    (?P<lang>[A-Za-z0-9_-]*) # optional language (can be empty)
    [ \t]*\r?\n              # end of the opening fence line
    (?P<body>.*?)            # fenced body (non-greedy)
    \r?\n[ \t]*```[ \t]*$    # closing fence with optional indentation
    """,
    re.DOTALL,
)


def _dedent_block(src: str) -> str:
    """Dedent a fenced block body while preserving relative indentation.

    This function is stricter than `textwrap.dedent` for doctest-style snippets:
    it expands tabs, trims leading/trailing blank lines, computes the minimum
    indentation across *non-empty* lines, and removes exactly that indentation.

    Args:
        src (str): Raw fenced code block body.

    Returns:
        str: Dedented code block body. If the body is empty (after trimming),
            returns an empty string.
    """
    src = src.expandtabs(4)
    lines = src.splitlines()

    while lines and not lines[0].strip():
        lines.pop(0)
    while lines and not lines[-1].strip():
        lines.pop()

    if not lines:
        return ""

    indents = [len(line) - len(line.lstrip(" ")) for line in lines if line.strip()]
    min_indent = min(indents) if indents else 0
    return "\n".join(line[min_indent:] for line in lines)


def iter_fenced_blocks(doc: str | None) -> list[tuple[str, str]]:
    """Extract fenced code blocks from a Markdown-style docstring.

    Args:
        doc (str | None): Docstring text containing fenced code blocks.

    Returns:
        list[tuple[str, str]]: A list of `(language, body)` tuples where:
            - `language` is lowercased (may be empty).
            - `body` is dedented and trimmed appropriately for execution/comparison.
    """
    if not doc:
        return []

    blocks: list[tuple[str, str]] = []
    for m in FENCE_RE.finditer(doc):
        lang = (m.group("lang") or "").strip().lower()
        raw = m.group("body")
        body = _dedent_block(raw)
        blocks.append((lang, body))
    return blocks


def pair_python_with_expected(blocks: list[tuple[str, str]]) -> list[tuple[str, str | None]]:
    """Pair Python code blocks with an optional following expected-output block.

    A Python block (`python` or `py`) is paired with the next block if that next
    block is a non-Python output block (language in `{"", "text", "txt", "none"}`).
    The paired non-Python block is treated as expected stdout.

    Args:
        blocks (list[tuple[str, str]]): Sequence of `(language, body)` blocks.

    Returns:
        list[tuple[str, str | None]]: List of `(python_code, expected_stdout)` pairs.
            `expected_stdout` is `None` if no matching output block follows.
    """
    pairs: list[tuple[str, str | None]] = []
    i = 0
    while i < len(blocks):
        lang, body = blocks[i]
        if lang in {"python", "py"}:
            expected: str | None = None
            if i + 1 < len(blocks) and blocks[i + 1][0] in {"", "text", "txt", "none"}:
                expected = blocks[i + 1][1]
                i += 1  # consume expected
            pairs.append((body, expected))
        i += 1
    return pairs


# ---------- Execution helpers ----------


def _is_expression(src: str) -> bool:
    """Return whether `src` parses as a valid Python expression.

    Args:
        src (str): Candidate source code.

    Returns:
        bool: True if `src` compiles in `eval` mode; False otherwise.
    """
    try:
        compile(src, "<expr>", "eval")
        return True
    except SyntaxError:
        return False


def exec_python_block_capture_stdout(code: str, ns: dict[str, Any]) -> tuple[str, str]:
    """Execute a Python code block and capture stdout/stderr.

    The entire block is executed as-written (preserving indentation).
    Optionally emulates REPL behavior: if the *last non-empty line* is a
    top-level (non-indented) pure expression, it is evaluated and its value is
    printed to stdout.

    This avoids breaking indented blocks (e.g., `if:` suites) by only applying
    "expression tail" handling when the last line is top-level.

    Args:
        code (str): Python source to execute.
        ns (dict[str, Any]): Namespace used for execution/evaluation.

    Returns:
        tuple[str, str]: `(stdout, stderr)` captured during execution.
    """
    lines = code.splitlines()
    while lines and not lines[-1].strip():
        lines.pop()

    if not lines:
        return "", ""

    last_line = lines[-1]
    last_stripped = last_line.strip()

    is_top_level = last_line[: len(last_line) - len(last_line.lstrip())] == ""
    can_eval_tail = is_top_level and _is_expression(last_stripped)

    out, err = StringIO(), StringIO()
    with redirect_stdout(out), redirect_stderr(err):
        if can_eval_tail:
            body = "\n".join(lines[:-1])
            if body.strip():
                exec(compile(body, "<doc-python-body>", "exec"), ns, ns)

            val = eval(compile(last_stripped, "<doc-python-expr>", "eval"), ns, ns)
            if val is not None:
                print(val)
        else:
            exec(compile("\n".join(lines), "<doc-python>", "exec"), ns, ns)

    return out.getvalue(), err.getvalue()


def normalize(s: str) -> str:
    """Normalize text for robust comparisons.

    Normalization rules:
      - Strip leading/trailing whitespace from the whole string.
      - Strip leading/trailing whitespace from each line.
      - Keep line order intact.

    Args:
        s (str): Input text.

    Returns:
        str: Normalized text.
    """
    lines = [ln.strip() for ln in s.strip().splitlines()]
    return "\n".join(lines)


def public_doc_objects_of(cls: type, skip_private: bool = False) -> list[object]:
    """Return the class object and its members that have docstrings.

    This collects:
      - The class itself if it has a docstring.
      - Public members (or all members if `skip_private=False`) that have docstrings.

    Args:
        cls (type): Class to inspect.
        skip_private (bool): If True, skip members whose names start with `_`.

    Returns:
        list[object]: Objects (class and members) with docstrings.
    """
    objs: list[object] = []
    if cls.__doc__:
        objs.append(cls)

    for name, member in inspect.getmembers(cls):
        if skip_private and name.startswith("_"):
            continue
        doc = getattr(member, "__doc__", None)
        if not doc:
            continue
        objs.append(member)

    return objs


# ---------- Stub (.pyi) docstring support ----------


@dataclass(frozen=True)
class StubTarget:
    """A pseudo-object representing a docstring extracted from a `.pyi` file.

    Attributes:
        pyi_path (str): Path to the stub file.
        qualname (str): Qualified name of the documented symbol (module/class/function).
        doc (str): Extracted docstring text.
    """

    pyi_path: str
    qualname: str
    doc: str


def _ast_qualname(stack: list[str]) -> str:
    """Build a dotted qualname from a stack of AST names.

    Args:
        stack (list[str]): Nested name components (e.g., `["Cls", "method"]`).

    Returns:
        str: Dotted qualname, or `"<module>"` for an empty stack.
    """
    return ".".join(stack) if stack else "<module>"


def iter_stub_targets(pyi_path: str | Path) -> list[StubTarget]:
    """Parse a `.pyi` file and return docstring targets.

    Extracts docstrings for:
      - module
      - class definitions
      - function definitions (including async functions)

    Args:
        pyi_path (str | Path): Path to the `.pyi` file.

    Returns:
        list[StubTarget]: Extracted docstring targets.
    """
    path = Path(pyi_path)
    src = path.read_text(encoding="utf-8")
    mod = ast.parse(src, filename=str(path))

    targets: list[StubTarget] = []

    mod_doc = ast.get_docstring(mod, clean=True)
    if mod_doc:
        targets.append(StubTarget(str(path), path.name, mod_doc))

    def walk_body(body: list[ast.stmt], stack: list[str]) -> None:
        """Walk an AST body and collect docstrings from classes/functions.

        Args:
            body (list[ast.stmt]): AST statements to inspect.
            stack (list[str]): Current qualname stack.
        """
        for node in body:
            if isinstance(node, ast.ClassDef):
                qn = _ast_qualname([*stack, node.name])
                d = ast.get_docstring(node, clean=True)
                if d:
                    targets.append(StubTarget(str(path), qn, d))
                walk_body(node.body, [*stack, node.name])

            elif isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef)):
                qn = _ast_qualname([*stack, node.name])
                d = ast.get_docstring(node, clean=True)
                if d:
                    targets.append(StubTarget(str(path), qn, d))

    walk_body(mod.body, [])
    return targets


def stub_doc_targets_of(*pyi_paths: str | Path) -> list[StubTarget]:
    """Collect StubTargets from one or more `.pyi` files.

    Args:
        *pyi_paths (str | Path): One or more stub file paths.

    Returns:
        list[StubTarget]: All extracted targets from all provided stubs.
    """
    out: list[StubTarget] = []
    for p in pyi_paths:
        out.extend(iter_stub_targets(p))
    return out


def target_id(obj: object) -> str:
    """Return a stable pytest id for runtime objects and stub targets.

    Args:
        obj (object): Runtime object (class/function/etc.) or a `StubTarget`.

    Returns:
        str: Stable identifier used by pytest parameterization.
    """
    if isinstance(obj, StubTarget):
        return f"{Path(obj.pyi_path).name}:{obj.qualname}"
    return getattr(obj, "__qualname__", str(obj))


def get_doc(obj: object) -> str:
    """Get docstring text for runtime objects and stub targets.

    Args:
        obj (object): Runtime object or `StubTarget`.

    Returns:
        str: Docstring text (empty string if absent).
    """
    if isinstance(obj, StubTarget):
        return obj.doc
    return inspect.getdoc(obj) or ""


def find_stub() -> Path:
    """Locate the installed stub file in a wheel, with a repo fallback.

    Resolution strategy:
      1) Installed wheel: `importlib.resources.files("bitbully") / "bitbully_core.pyi"`
      2) Repo-relative fallback for local development.

    Returns:
        Path: Path to `bitbully_core.pyi`.

    Raises:
        FileNotFoundError: If the stub file cannot be located.
    """
    try:
        p = files("bitbully") / "bitbully_core.pyi"
        if p.is_file():
            return Path(p)
    except Exception:
        pass

    here = Path(__file__).resolve()
    repo_root = here.parents[2]  # adjust if your test layout differs
    p = repo_root / "src" / "bitbully" / "bitbully_core.pyi"
    if not p.exists():
        raise FileNotFoundError(p)
    return p


STUB_PATHS: list[Path] = [find_stub()]

TARGETS: list[object] = (
    public_doc_objects_of(Board) + public_doc_objects_of(BitBully) + stub_doc_targets_of(*STUB_PATHS)
)


@pytest.mark.parametrize("obj", TARGETS, ids=target_id)
def test_docstring_code_examples(obj: object) -> None:
    """Execute fenced Python examples in docstrings and assert they are correct.

    For each target object's docstring:
      1) Extract fenced blocks (`iter_fenced_blocks`).
      2) Pair Python blocks with optional expected-output blocks (`pair_python_with_expected`).
      3) Execute each Python block in a fresh namespace and capture stdout/stderr.
      4) If an expected-output block exists, compare normalized stdout to it.
      5) Always assert that stderr is empty.

    Args:
        obj (object): Runtime object (class/function) or `StubTarget`.
    """
    doc = get_doc(obj)
    blocks = iter_fenced_blocks(doc)
    pairs = pair_python_with_expected(blocks)

    for idx, (py_code, expected) in enumerate(pairs, start=1):
        ns: dict[str, Any] = {}

        try:
            import bitbully as bb

            ns["bb"] = bb
        except Exception:
            pass

        stdout, stderr = exec_python_block_capture_stdout(py_code, ns)

        if expected is not None:
            assert normalize(stdout) == normalize(expected), (
                f"Docstring example #{idx} in {target_id(obj)} "
                f"did not match expected output.\n--- got ---\n{stdout}\n--- expected ---\n{expected}"
            )

        assert stderr == "", f"Docstring example #{idx} in {target_id(obj)} wrote to stderr:\n{stderr}"
