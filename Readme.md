# BitBully: A fast and perfect-playing Connect-4 Agent for Python 3 & C/C++

<h1 align="center">
<img src="bitbully-logo-full.png" alt="bitbully-logo-full.png" width="400" >
</h1><br>

![GitHub Repo stars](https://img.shields.io/github/stars/MarkusThill/BitBully)
![GitHub forks](https://img.shields.io/github/forks/MarkusThill/BitBully)
![Python](https://img.shields.io/badge/language-Python-blue.svg)
![Python](https://img.shields.io/badge/language-C++-yellow.svg)
[![Python](https://img.shields.io/pypi/pyversions/bitbully.svg)](https://badge.fury.io/py/bitbully)
![Docs](https://img.shields.io/badge/docs-online-brightgreen)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit)](https://github.com/pre-commit/pre-commit)
![PyPI - Version](https://img.shields.io/pypi/v/bitbully)
![PyPI - Downloads](https://img.shields.io/pypi/dm/bitbully)
![PyPI - License](https://img.shields.io/pypi/l/bitbully)
![Coveralls](https://coveralls.io/repos/github/OWNER/REPO/badge.svg)
![Wheels](https://github.com/MarkusThill/BitBully/actions/workflows/wheels.yml/badge.svg)
![Doxygen](https://github.com/MarkusThill/BitBully/actions/workflows/doxygen.yml/badge.svg)
![Doxygen](https://github.com/MarkusThill/BitBully/actions/workflows/cmake-multi-platform.yml/badge.svg)
![Buy Me a Coffee](https://img.shields.io/badge/support-Buy_Me_A_Coffee-orange)

# BitBully

**BitBully** is a high-performance Connect-4 solver built using C++ and Python bindings, leveraging advanced algorithms
and optimized bitwise operations. It provides tools for solving and analyzing Connect-4 games efficiently, designed for
both developers and researchers.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Build and Install](#build-and-install)
- [Usage](#usage)
- [Examples](#examples)
- [Python API](#python-api)
- [Testing and CI](#testing-and-ci)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## Features

- **Fast Solver**: Implements MTD(f) and null-window search algorithms for Connect-4.
- **Bitboard Representation**: Efficiently manages board states using bitwise operations.
- **Advanced Features**: Includes transposition tables, threat detection, and move prioritization.
- **Python Bindings**: Exposes core functionality through the `bitbully_core` Python module using `pybind11`.
- **Cross-Platform**: Build and run on Linux, Windows, and macOS.
- **Open-Source**: Fully accessible codebase for learning and contribution.

---

## Installation

### Prerequisites

- **Python**: Version 3.8 or higher, PyPy 3.10 or higher

---

## Build and Install

### From PyPI (Recommended)

The easiest way to install the BitBully package is via PyPI:

```bash
pip install bitbully
```

This will automatically download and install the pre-built package, including the Python bindings.

## Usage

### Python Library

Use the `BitBully` and `Board` classes directly in Python:

```python
from bitbully import bitbully_core
import time

board = bitbully_core.Board()
board.playMove(3)  # Yellow plays a move in column 3 (center column)
board.playMove(3)  # Red plays a move in column 3 (center column)
print(board)

solver = bitbully_core.BitBully()
start = time.time()
score = solver.mtdf(board, first_guess=0)
print(f"Time: {round(time.time() - start, 2)} seconds!")
print(f"Best score: {score}")
```

You can initialize a board using a Numpy array with shape `(7, 6)` (columns first) and solve it:

```python
import numpy as np
from bitbully import bitbully_core

# Define a Connect-4 board as a Numpy array (7 columns x 6 rows)
# 0 = Empty, 1 = Yellow, 2 = Red
board_array = np.array([
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [1, 2, 1, 2, 1, 0],
    [0, 0, 0, 0, 0, 0],
    [2, 1, 2, 0, 0, 0],
    [0, 0, 0, 0, 0, 0]
])

# Convert the Numpy array to the BitBully board
board = bitbully_core.Board()
assert board.setBoard(board_array), "Invalid board!"

print(board)

# Solve the position
solver = bitbully_core.BitBully()
score = solver.mtdf(board, first_guess=0)
print(f"Best score for the current board: {score}")
```

---

---

## Advanced Build and Install

### Prerequisites

- **Python**: Version 3.8 or higher
- **CMake**: Version 3.15 or higher
- **C++ Compiler**: A compiler supporting C++17 (e.g., GCC, Clang, MSVC)
- **Python Development Headers**: Required for building the Python bindings

### From Source

1. Clone the repository:
   ```bash
   git clone https://github.com/MarkusThill/BitBully.git
   cd BitBully
   git submodule update --init --recursive # – Initialize and update submodules.
   ```

2. Build and install the Python package:
   ```bash
   pip install .
   ```

### Building Static Library with CMake

1. Create a build directory and configure the project:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```

2. Build the a static library:
   ```bash
   cmake --build . --target cppBitBully
   ```

---

## Python API (Detailed Docs will follow)

### `BitBully` Class

- **`mtdf(board, first_guess)`**: Executes the MTD(f) algorithm.
- **`nullWindow(board)`**: Performs a null-window search.
- **`resetTranspositionTable()`**: Clears the transposition table.
- **`getNodeCounter()`**: Returns the number of nodes evaluated.
- **`resetNodeCounter()`**: Resets the node counter.

### `Board` Class

- **Game State Management**:
    - `playMove(column)` / `playMoveFastBB(move)`
    - `setBoard(array)` / `setBoard(move_sequence)`
    - `generateMoves()` / `generateNonLosingMoves()`
- **Analysis and Queries**:
    - `canWin()` / `hasWin()`
    - `findThreats(moves)` / `doubleThreat(moves)`
- **Utilities**:
    - `toString()` / `toArray()`
    - `hash()` / `uid()`
    - `mirror()`

---

## Testing and CI

### Running Tests

Run unit tests using `pytest`:

```bash
pytest
```

### GitHub Actions

This project uses GitHub Actions to build and test the library. The CI workflow includes:

- Building wheels for Linux and Windows using `cibuildwheel`.
- Building source distributions (`sdist`).
- Optionally uploading artifacts to PyPI.

---

## Contributing

Contributions are welcome! Follow these steps:

1. Fork the repository.
2. Create a new branch for your changes:
   ```bash
   git checkout -b feature-name
   ```
3. Install pre-commit hooks:
   ```
   TODO
   ```
3. Commit your changes:
   ```bash
   git commit -m "feat: Add feature or fix description"
   ```
4. Push to your branch:
   ```bash
   git push origin feature-name
   ```
5. Open a pull request.

---

## License

This project is licensed under the [AGPL-3.0 license](LICENSE).

---

## Contact

If you have any questions or feedback, feel free to reach out:

- **Web**: [https://markusthill.github.io](https://markusthill.github.io)
- **GitHub**: [MarkusThill](https://github.com/MarkusThill)
- **LinkedIn**: [Markus Thill](https://www.linkedin.com/in/markus-thill-a4991090)

## Acknowledgments

Many of the concepts and techniques used in this project are inspired by the outstanding Connect-4 solvers developed by
Pascal Pons and John Tromp. Their work has been invaluable in shaping this effort:

- [http://blog.gamesolver.org/](http://blog.gamesolver.org/)
- [https://github.com/PascalPons/connect4](https://github.com/PascalPons/connect4)
- https://tromp.github.io/c4/Connect4.java
- https://github.com/gamesolver/fhourstones/

<h1 align="center">
<img src="https://raw.githubusercontent.com/MarkusThill/snk/refs/heads/manual-run-output/only-svg/github-contribution-grid-snake.svg" alt="https://raw.githubusercontent.com/MarkusThill/snk/refs/heads/manual-run-output/only-svg/github-contribution-grid-snake.svg" width="90%" >
</h1><br>
