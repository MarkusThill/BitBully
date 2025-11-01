# BitBully: A fast and perfect-playing Connect-4 Agent for Python 3 & C/C++

<h1 align="center">
<img src="https://markusthill.github.io/assets/img/project_bitbully/bitbully-logo-full-800.webp" alt="bitbully-logo-full" width="400" >
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

- **Python**: Version 3.10 or higher, PyPy 3.10 or higher

---

## Build and Install

### From PyPI (Recommended)

The easiest way to install the BitBully package is via PyPI:

```bash
pip install bitbully
```

This will automatically download and install the pre-built package, including the Python bindings.

## Usage

### Start with a simple Widget on Colab

<a href="https://colab.research.google.com/github/MarkusThill/BitBully/blob/master/notebooks/gameWidget.ipynb" target="_parent"><img src="https://colab.research.google.com/assets/colab-badge.svg" alt="Open In Colab"/></a>

### BitBully Lib (recommended)

tbd

### BitBully Core (advanced)

Use the `BitBullyCore` and `BoardCore` classes directly in Python:

```python
from bitbully import bitbully_core
import time

board = bitbully_core.BoardCore()

# Yellow and red alternately play moves into column 3 (center column):
for _ in range(6):
    board.play(3)

print(board)

solver = bitbully_core.BitBullyCore()
start = time.perf_counter()
score = solver.mtdf(board, first_guess=0)
print(f"Time: {round(time.perf_counter() - start, 2)} seconds!")
print(f"Best score: {score}")
```

You can initialize a board using an array with shape `(7, 6)` (columns first) and solve it:

```python
from bitbully import bitbully_core

# Define a Connect-4 board as an array (7 columns x 6 rows)
# You may also define the board using a numpy array if numpy is installed
# 0 = Empty, 1 = Yellow, 2 = Red
board_array = [
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [1, 2, 1, 2, 1, 0],
    [0, 0, 0, 0, 0, 0],
    [2, 1, 2, 0, 0, 0],
    [0, 0, 0, 0, 0, 0]
]

# Convert the array to the BoardCore board
board = bitbully_core.BoardCore()
assert board.setBoard(board_array), "Invalid board!"

print(board)

# Solve the position
solver = bitbully_core.BitBullyCore()
score = solver.mtdf(board, first_guess=0)
print(f"Best score for the current board: {score}") # expected score: 1
```

Run the Bitbully solver with an opening book (here: 12-ply opening book with winning distances):

```python
from bitbully import bitbully_core as bbc
import importlib.resources

db_path = importlib.resources.files("bitbully").joinpath("assets/book_12ply_distances.dat")
bitbully = bbc.BitBullyCore(db_path)
b = bbc.BoardCore()  # Empty board
bitbully.scoreMoves(b)  # expected result: [-2, -1, 0, 1, 0, -1, -2]
```

Generate a random board with `n` tokens:

```python
from bitbully import bitbully_core as bbc

# Create a random board (and the move sequence that generated it)
b, move_list = bbc.BoardCore.randomBoard(12, True)
print(b)
print(move_list)
```

### Further Usage Examples for BitBully Core

Create all Positions with (up to) `n` tokens starting from Board `b`:

```python
from bitbully import bitbully_core as bbc

b = bbc.BoardCore()  # empty board
board_list_3ply = b.allPositions(3, True)  # All positions with exactly 3 tokens
len(board_list_3ply)  # should be 238 according to https://oeis.org/A212693
```

Find the game-theoretic value of a 12-ply position using an opening book:

```python
from bitbully import bitbully_core as bbc
import importlib.resources

db_path = importlib.resources.files("bitbully").joinpath("assets/book_12ply_distances.dat")
ob = bbc.OpeningBookCore(db_path)
b, move_list = bbc.BoardCore.randomBoard(12, True)  # get a board without an immediate threat for Yellow
assert ob.isInBook(b) or ob.isInBook(b.mirror())  # Either position `b` or its mirrored equivalent are in the DB
ob.getBoardValue(b)  # Get game-theoretic value (also checks mirrored board)
```

---

## Advanced Build and Install

### Prerequisites

- **Python**: Version 3.10 or higher
- **CMake**: Version 3.15 or higher
- **C++ Compiler**: A compiler supporting C++-17 (e.g., GCC, Clang, MSVC)
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

## Python API Docs

Please refer to the docs here: [https://markusthill.github.io/BitBully/](https://markusthill.github.io/BitBully/).

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
   pre-commit install --hook-type commit-msg --hook-type pre-push
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
