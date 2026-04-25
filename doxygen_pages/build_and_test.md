# Building and Testing the C++ Core {#build_and_test}

[TOC]

The C++ core is built with CMake. The instructions below assume Linux; the
project also builds on macOS and Windows.

## Prerequisites

- A C++17-capable compiler (`gcc` ≥ 9, `clang` ≥ 10, MSVC 2019+).
- CMake ≥ 3.16.
- (Optional) [Doxygen](https://www.doxygen.nl/) ≥ 1.13 and Graphviz to
  regenerate this documentation.

## Configure & build

```bash
git submodule update --init --recursive
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

The build produces:

- `build/bitbully_main` — the benchmark binary defined in
  [main.cpp](main_8cpp.html).
- `build/bitbully_core.*.so` (or `.pyd` on Windows) — the pybind11 module
  that backs the Python package.

## Tests

The repository ships [GoogleTest](https://github.com/google/googletest)
suites under `gtests/`. After building:

```bash
ctest --test-dir build --output-on-failure
```

## Regenerating the documentation

Run from the repository root:

```bash
doxygen Doxyfile
```

Doxygen writes its output into `docs/cpp/html/` so that mkdocs can pick it
up under the `C++ Docs` entry of the navigation sidebar.

To preview the combined Python + C++ site locally:

```bash
mkdocs serve
```

The Doxygen output (`docs/cpp/html/index.html`) is served at
`http://127.0.0.1:8000/cpp/html/index.html`.
