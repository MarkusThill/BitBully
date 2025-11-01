"""Tests that all Python code examples in docstrings execute without error and produce the expected output."""

from __future__ import annotations

import inspect
import re
import textwrap
from contextlib import redirect_stderr, redirect_stdout
from io import StringIO
from typing import Any

import pytest

from bitbully import Board

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


def iter_fenced_blocks(doc: str | None) -> list[tuple[str, str]]:
    """Extract all fenced code blocks from a Markdown-style docstring.

    This function scans a docstring for fenced code blocks (e.g., those enclosed
    in triple backticks) and returns a list of tuples containing
    each block's language identifier and its dedented body. If no language tag
    is provided after the opening fence, the language is returned as an empty
    string (`''`).

    The body of each block is automatically dedented and trimmed, so it can be
    executed or compared consistently regardless of indentation levels.

    Args:
        doc (str | None): The input docstring or Markdown text to parse.
            If `None` or empty, an empty list is returned.

    Returns:
        list[tuple[str, str]]: A list of `(language, body)` tuples, where:
            - `language` is a lowercased string such as `"python"`, `"text"`, or `""`
              (empty string if no language tag is provided).
            - `body` is the dedented and trimmed content of the fenced block.

    Example:
        ```python
        doc = '''
        Example:

        ```python
        print("Hello, world!")
        ```

        ```text
        Hello, world!
        ```
        '''
        iter_fenced_blocks(doc)
        # [
        #   ('python', 'print("Hello, world!")'),
        #   ('text', 'Hello, world!')
        # ]
        ```
    """
    if not doc:
        return []
    blocks: list[tuple[str, str]] = []
    for m in FENCE_RE.finditer(doc):
        lang = (m.group("lang") or "").strip().lower()
        raw = m.group("body")
        # Dedent and trim one leading/trailing blank line if present
        body = textwrap.dedent(raw).lstrip("\n").rstrip()
        blocks.append((lang, body))
    return blocks


def pair_python_with_expected(blocks: list[tuple[str, str]]) -> list[tuple[str, str | None]]:
    """Pair Python code blocks with their corresponding expected output blocks.

    This function processes a list of `(language, content)` tuples—such as those
    extracted from Markdown fenced code blocks—and returns pairs consisting of
    a Python code block (`python` or `py`) and its subsequent non-Python block
    (`''`, `'text'`, `'txt'`, or `'none'`), if one exists. The non-Python block
    is treated as the *expected* stdout for the preceding Python block.

    Args:
        blocks (list[tuple[str, str]]): A list of `(language, content)` pairs.
            Each element represents a code block, where `language` is a short
            identifier such as `'python'`, `'text'`, or an empty string, and
            `content` is the block's text content.

    Returns:
        list[tuple[str, str | None]]: A list of pairs `(python_code, expected_output)`,
        where `expected_output` may be `None` if no matching text block follows
        the Python code.
    """
    pairs: list[tuple[str, str | None]] = []
    i = 0
    while i < len(blocks):
        lang, body = blocks[i]
        if lang in {"python", "py"}:
            expected = None
            if i + 1 < len(blocks) and blocks[i + 1][0] in {"", "text", "txt", "none"}:
                expected = blocks[i + 1][1]
                i += 1  # consume expected
            pairs.append((body, expected))
        i += 1
    return pairs


# ---------- Execution helpers ----------


def _is_expression(src: str) -> bool:
    try:
        compile(src, "<expr>", "eval")
        return True
    except SyntaxError:
        return False


def exec_python_block_capture_stdout(code: str, ns: dict) -> tuple[str, str]:
    """Execute a Python code block and capture its stdout and stderr.

    This function executes a multi-line string containing Python code
    within the provided namespace (`ns`). If the last non-empty line is
    a pure expression (e.g., `board`), it is evaluated instead of executed,
    and its resulting value is printed so it appears in the captured stdout.

    This behavior mimics interactive Python sessions or doctest-style
    code evaluation, where the final expression is automatically displayed.

    Args:
        code (str): A string containing the Python code to execute.
        ns (dict): A namespace (typically a dict) used for variable
            definitions and lookups during code execution.

    Returns:
        tuple[str, str]: A tuple containing:
            - **stdout**: The text written to standard output.
            - **stderr**: The text written to standard error.
    """
    # split lines & strip trailing empties
    lines = code.splitlines()
    while lines and not lines[-1].strip():
        lines.pop()

    body = "\n".join(lines[:-1]) if len(lines) > 1 else ""
    tail = lines[-1].strip() if lines else ""

    out, err = StringIO(), StringIO()
    with redirect_stdout(out), redirect_stderr(err):
        if body:
            exec(compile(body, "<doc-python-body>", "exec"), ns, ns)
        if tail:
            if _is_expression(tail):
                val = eval(compile(tail, "<doc-python-expr>", "eval"), ns, ns)
                if val is not None:
                    print(val)
            else:
                exec(compile(tail, "<doc-python-tail>", "exec"), ns, ns)
    return out.getvalue(), err.getvalue()


def normalize(s: str) -> str:
    """Normalize a string by trimming leading/trailing whitespace and line endings.

    This function removes outer whitespace from the entire string and also
    trims trailing and leading spaces from each individual line, making
    string comparisons (e.g., in doctests or assertions) more robust.

    Args:
        s (str): The input string to normalize.

    Returns:
        str: The normalized string with consistent line formatting.
    """
    lines = [ln.strip() for ln in s.strip().splitlines()]
    return "\n".join(lines)


def public_doc_objects_of(cls: type, skip_private: bool = False) -> list[object]:
    """Return the class object  + all public methods.

    class object (for class docstring) and all public methods
    (functions, staticmethods, classmethods) that *have* docstrings with code.
    """
    objs: list[object] = []
    if cls.__doc__:
        objs.append(cls)
    for name, member in inspect.getmembers(cls):
        if skip_private and name.startswith("_"):
            continue
        # include if it has a docstring
        doc = getattr(member, "__doc__", None)
        if not doc:
            continue
        objs.append(member)
    return objs


# ---------- The test ----------

TARGETS = public_doc_objects_of(Board)


@pytest.mark.parametrize("obj", TARGETS, ids=lambda o: getattr(o, "__qualname__", str(o)))
def test_docstring_code_examples(obj: object) -> None:
    """Run fenced Python examples found in object docstrings and verify their outputs.

    The test scans the docstring of ``obj`` for fenced code blocks (via
    ``iter_fenced_blocks``). Each Python block (``python``/``py``) is paired with
    the *next* non-Python text block (``''``, ``'text'``, ``'txt'``, ``'none'``),
    which, when present, is treated as the expected stdout. The Python block is
    executed in an isolated namespace using ``exec_python_block_capture_stdout``.
    If the last line of the block is a pure expression, its value is printed so
    it appears on stdout (doctest-like behavior).

    For each pair:
      * If an expected block exists, compare normalized stdout to that expected text.
      * Regardless, assert that nothing was written to stderr.

    Args:
        obj (object): The object (module, class, function, etc.) whose docstring
            will be inspected for fenced code examples.
    """
    doc: str = inspect.getdoc(obj) or ""
    blocks: list[tuple[str, str]] = iter_fenced_blocks(doc)
    pairs: list[tuple[str, str | None]] = pair_python_with_expected(blocks)

    for idx, (py_code, expected) in enumerate(pairs, start=1):
        # fresh namespace per example to keep them independent
        ns: dict[str, Any] = {}

        # convenience import for shorter snippets (optional)
        try:
            import bitbully as bb

            ns["bb"] = bb
        except Exception:
            pass

        stdout, stderr = exec_python_block_capture_stdout(py_code, ns)

        # if there is an expected block, compare stdout with it
        if expected is not None:
            assert normalize(stdout) == normalize(expected), (
                f"Docstring example #{idx} in {getattr(obj, '__qualname__', obj)} "
                f"did not match expected output.\n--- got ---\n{stdout}\n--- expected ---\n{expected}"
            )

        # even without expected, ensure the example didn't error
        assert stderr == "", (
            f"Docstring example #{idx} in {getattr(obj, '__qualname__', obj)} wrote to stderr:\n{stderr}"
        )
