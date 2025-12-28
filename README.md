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
[![Coverage Status](https://coveralls.io/repos/github/MarkusThill/BitBully/badge.svg?branch=master)](https://coveralls.io/github/MarkusThill/BitBully?branch=master)
![Wheels](https://github.com/MarkusThill/BitBully/actions/workflows/wheels.yml/badge.svg)
![Doxygen](https://github.com/MarkusThill/BitBully/actions/workflows/doxygen.yml/badge.svg)
![CMake Build](https://github.com/MarkusThill/BitBully/actions/workflows/cmake-multi-platform.yml/badge.svg)
![Buy Me a Coffee](https://img.shields.io/badge/support-Buy_Me_A_Coffee-orange)

**BitBully** is a high-performance Connect-4 solver built using C++ and Python bindings, leveraging advanced algorithms
and optimized bitwise operations. It provides tools for solving and analyzing Connect-4 games efficiently, designed for
both developers and researchers.

> BitBully evaluates millions of nodes per second in pure C++ and supports
> constant-time opening book lookups for early-game positions.


<p align="center">
  <img src="https://markusthill.github.io/assets/img/project_bitbully/c4-1-1400.webp"
       alt="Connect4 opening"
       width="28%"
       style="margin: 0 12px;">
  <img src="https://markusthill.github.io/assets/img/project_bitbully/c4-2-1400.webp"
       alt="Connect4 mid-game"
       width="28%"
       style="margin: 0 12px;">
  <img src="https://markusthill.github.io/assets/img/project_bitbully/c4-3-1400.webp"
       alt="Connect4 victory"
       width="28%"
       style="margin: 0 12px;">
</p>

<p align="center">
  <em>
    From opening to victory: three key stages of a Connect&nbsp;4 match — early game,
    mid-game tension, and the final winning position.
  </em>
</p>


## Quickstart

```python
import bitbully as bb

agent = bb.BitBully()
board = bb.Board()

while not board.is_game_over():
    board.play(agent.best_move(board))

print(board)
print("Winner:", board.winner())
```

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Build and Install](#build-and-install)
- [Python API Docs](#python-api-docs)
- [Usage](#usage)
- [Advanced Build and Install](#advanced-build-and-install)
- [Contributing & Development](#contributing--development)
- [License](#license)
- [Contact](#contact)
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

### Who is this for?

- **Just want to play or analyze Connect-4 in Python?**
  → Read *Quickstart* + *Usage (High-level Python API)*

- **Interested in performance, algorithms, or C++ integration?**
  → See *Low-level C++ bindings (advanced)*

- **Working on research, solvers, or databases?**
  → See *Opening Books* and *BoardCore*


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

---

## Python API Docs

Please refer to the docs here: [https://markusthill.github.io/BitBully/](https://markusthill.github.io/BitBully/).

The docs for the opening databases can be found here: [https://markusthill.github.io/bitbully-databases/](https://markusthill.github.io/bitbully-databases/)

---

## Usage

> ⚠️ **Note**
> `bitbully_core` exposes low-level C++ bindings intended for advanced users.
> Most users should use the high-level `bitbully` Python API with the classes `Board` and `BitBully`.
>
> BitBully currently supports **standard Connect-4 (7 columns × 6 rows)**.
> Generalized board sizes are not supported.

### 🚀 BitBully: Getting Started with a Jupyter Notebook

This notebook introduces the main building blocks of **BitBully**:

- `Board`: represent and manipulate Connect Four positions
- `BitBully`: analyze positions and choose strong moves

All examples are designed to be copy-pasteable and easy to adapt for your own experiments.

Jupyter Notebook: [notebooks/getting_started.ipynb](https://github.com/MarkusThill/BitBully/blob/master/notebooks/getting_started.ipynb)

<a href="https://colab.research.google.com/github/MarkusThill/BitBully/blob/master/notebooks/getting_started.ipynb" target="_parent"><img src="https://colab.research.google.com/assets/colab-badge.svg" alt="Open In Colab"/></a>

### 🎮 Play a Game of Connect-4 with a simple Jupyter Notebook Widget
TODO: Screenshot here!

BitBully includes an interactive Connect-4 widget for Jupyter built with ipywidgets + Matplotlib.
`GuiC4` renders a 6x7 board using image sprites, supports click-to-play or column buttons, provides undo/redo, can trigger a computer move using the BitBully engine (optionally with an opening book database), and shows win/draw popups. It's intended for quick experimentation and demos inside notebooks (best with %matplotlib ipympl).

Jupyter Notebook: [notebooks/game_widget.ipynb](https://github.com/MarkusThill/BitBully/blob/master/notebooks/game_widget.ipynb)

<a href="https://colab.research.google.com/github/MarkusThill/BitBully/blob/master/notebooks/game_widget.ipynb" target="_parent"><img src="https://colab.research.google.com/assets/colab-badge.svg" alt="Open In Colab"/></a>

### High-level Python API (recommended)

#### Empty board + play moves incrementally

```python
import bitbully as bb

board = bb.Board()
assert board.play(3)          # single move (int)
assert board.play([2, 4, 3])  # multiple moves (list)
assert board.play("001122")   # multiple moves (string)

print(board)
```

#### Initialize directly from a move sequence

```python
import bitbully as bb

board_a = bb.Board([3, 3, 3, 1, 1])
board_b = bb.Board("33311")

assert board_a == board_b
print(board_a)
```

#### Create positions (moves, strings, arrays) and round-trip them

```python
import bitbully as bb

# From a move list
b1 = bb.Board([3, 3, 3, 1, 1])

# From a compact move string
b2 = bb.Board("33311")

assert b1 == b2
print(b1)

# From a 2D array (row-major 6x7 or column-major 7x6 both work)
arr = b1.to_array()  # default: column-major 7x6
b3 = bb.Board(arr)

assert b1 == b3
```

#### Legal moves and remaining moves

```python
import bitbully as bb

board = bb.Board("33333111")

print(board.legal_moves())                 # all legal columns
print(board.legal_moves(order_moves=True)) # ordered (center-first)
print("Moves left:", board.moves_left())
print("Tokens:", board.count_tokens())
```

#### Some board utilities

```python
import bitbully as bb

board = bb.Board("332311")
print(board)

print("Can win next (any):", board.can_win_next())
print("Can win next in col 4:", board.can_win_next(4))

assert board.play(4)  # play winning move
print(board)

print("Has win:", board.has_win())
print("Game over:", board.is_game_over())
print("Winner:", board.winner())  # 1
```

#### Solver Quickstart: evaluate a position and pick a move
```python
import bitbully as bb

agent = bb.BitBully()          # loads default opening book ("12-ply-dist")
board = bb.Board()             # empty board

print(board)

scores = agent.score_all_moves(board)
print("Move scores:", scores)

best_col = agent.best_move(board)
print("Best move:", best_col)
```

#### Play a small game loop (agent vs. itself)
```python
import bitbully as bb

agent = bb.BitBully()
board = bb.Board()

while not board.is_game_over():
    col = agent.best_move(board, tie_break="random")
    assert board.play(col)

print(board)
print("Winner:", board.winner())  # 1, 2, or None for draw
```

#### Tie-breaking strategies for `best_move`

```python
import bitbully as bb
import random

agent = bb.BitBully()
board = bb.Board("341")  # arbitrary position

print(board)

print("Center tie-break:", agent.best_move(board, tie_break="center"))
print("Leftmost tie-break:", agent.best_move(board, tie_break="leftmost"))

rng = random.Random(42) # optional own random generator
print("Random tie-break (seeded):", agent.best_move(board, tie_break="random", rng=rng))
```

#### Different Search Algorithms

```python
import bitbully as bb

agent = bb.BitBully()
board, _ = bb.Board.random_board(n_ply=14, forbid_direct_win=True)

s1 = agent.mtdf(board)
s2 = agent.negamax(board)
s3 = agent.null_window(board)

assert s1 == s2 == s3
print("Score:", s1)
```

---

### Low-level C++ bindings (advanced)

Use the `BitBullyCore` and `BoardCore` classes directly in Python:

#### BoardCore Examples

The low-level `BoardCore` API gives you full control over Connect-4 positions:
you can play moves, generate random boards, mirror positions, and query win
conditions or hashes.

##### Create and Print a Board

```python
import bitbully.bitbully_core as bbc

board = bbc.BoardCore()
print(board)          # Human-readable 7x6 board
print(board.movesLeft())   # 42 on an empty board
print(board.countTokens()) # 0 on an empty board
```

---

##### Play Moves and Check for Winning Positions

```python
import bitbully.bitbully_core as bbc

board = bbc.BoardCore()

# Play a small sequence of moves (columns 0–6)
for col in [3, 2, 3, 2, 3, 4, 3]:
    assert board.play(col)

print(board)

# Check if the side to move has an immediate winning move
print(board.canWin())      # False
print(board.hasWin())      # True, since the last move created 4-in-a-row
```

You can also check if a **specific column** is a winning move:

```python
board = bbc.BoardCore()
board.setBoard([3, 3, 3, 3, 2, 2, 4, 4])

print(board.canWin())  # True
print(board.canWin(1))  # True  – playing in column 1 wins
print(board.canWin(3))  # False – no win in column 3
```

---

##### Set a Board from a Move List or Array

```python
import bitbully.bitbully_core as bbc

board = bbc.BoardCore()

# From a move sequence (recommended)
assert board.setBoard([0, 1, 2, 3, 3, 2, 1, 0])

# Convert to 7x6 array (columns × rows)
array = board.toArray()
print(len(array), len(array[0]))  # 7 x 6

# From a 7x6 array of tokens (1 = Yellow, 2 = Red)
array_board = [[0 for _ in range(6)] for _ in range(7)]
array_board[3][0] = 1  # Yellow in center column bottom row
b2 = bbc.BoardCore()
assert b2.setBoard(array_board)
```

---

##### Generate Random Boards

```python
import bitbully.bitbully_core as bbc

board, moves = bbc.BoardCore.randomBoard(10, True)

print(board)   # Random, valid board
print(moves)   # List of 10 column indices
print(board.canWin())  # Usually False for random boards in this setup
```

---

##### Mirroring Boards and Symmetry

```python
import bitbully.bitbully_core as bbc

board = bbc.BoardCore()
board.setBoard([0, 1, 2])      # Left side

mirrored = board.mirror()      # Mirror around center column
print(board)
print(mirrored)

# Double-mirroring returns the original position
assert board == mirrored.mirror()
```

---

##### Hashing, Equality, and Copies

```python
import bitbully.bitbully_core as bbc

b1 = bbc.BoardCore()
b2 = bbc.BoardCore()

moves = [0, 1, 2, 3]
for m in moves:
    b1.play(m)
    b2.play(m)

assert b1 == b2
assert b1.hash() == b2.hash()
assert b1.uid() == b2.uid()

# Copying a board
b3 = b1.copy()           # or bbc.BoardCore(b1)
assert b3 == b1

b3.play(4)               # Modify the copy
assert b3 != b1
assert b3.hash() != b1.hash()
```

These examples are based on the internal test suite and show typical ways of
interacting with `BoardCore` programmatically.

#### BitBullyCore: Connect-4 Solver Examples

The `BitBullyCore` module provides a high-performance Connect-4 solver written in C++
and exposed to Python. You can evaluate positions, score all legal moves, or run the
full MTD(f) search.

---

##### Solve a Position with MTD(f)

```python
import bitbully.bitbully_core as bbc

# Construct a position: alternate moves into the center column
board = bbc.BoardCore()
for _ in range(6):
    board.play(3)  # Column 3

solver = bbc.BitBullyCore()
score = solver.mtdf(board, first_guess=0)

print("Best score:", score)
```

`mtdf` returns an integer score from the perspective of the **side to move**
(positive = winning, negative = losing).

---

##### Score All Moves in a Position

`scoreMoves(board)` returns a list of 7 integers:
the evaluated score for playing in each column (0–6).
Illegal moves (full columns) are still included in the list.

```python
import bitbully.bitbully_core as bbc

board = bbc.BoardCore()
board.setBoard([3, 4, 1, 1, 0, 2, 2, 2])

solver = bbc.BitBullyCore()
scores = solver.scoreMoves(board)

print("Move scores:", scores)
# Example output:
# [-3, -3, 1, -4, 3, -2, -2]
```

---

##### Using the Solver in a Loop (Move Selection)

```python
import bitbully.bitbully_core as bbc
import time

board = bbc.BoardCore()
solver = bbc.BitBullyCore()

for move in [3, 4, 1, 1, 0, 2, 2, 2]:  # Example opening
    board.play(move)

start = time.perf_counter()
scores = solver.scoreMoves(board)
best_move = max(range(7), key=lambda c: scores[c])
print(f"Time: {round(time.perf_counter() - start, 2)} seconds!")
print("Scores:", scores)
print("Best move suggestion:", best_move)
# best move is into column 4
```

---

##### Further Examples using the BitBully Solver

You can initialize a board using an array with shape `(7, 6)` (columns first) and solve it:

```python
from bitbully import bitbully_core

# Define a Connect-4 board as an array (7 columns x 6 rows)
# You may also define the board using a numpy array if numpy is installed
# 0 = Empty, 1 = Yellow, 2 = Red
# Here, the left column represents the bottom row of the board
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
import bitbully_databases as bbd
import importlib.resources

db_path = bbd.BitBullyDatabases.get_database_path("12-ply-dist")
bitbully = bbc.BitBullyCore(db_path)
b = bbc.BoardCore()  # Empty board
bitbully.scoreMoves(b)  # expected result: [-2, -1, 0, 1, 0, -1, -2]
```

#### Further Usage Examples for BitBully Core

Create all Positions with (up to) `n` tokens starting from Board `b`:

```python
from bitbully import bitbully_core as bbc

b = bbc.BoardCore()  # empty board
board_list_3ply = b.allPositions(3, True)  # All positions with exactly 3 tokens
len(board_list_3ply)  # should be 238 according to https://oeis.org/A212693
```

#### Opening Book Examples

BitBully Databases provide fast lookup tables (opening books) for Connect-4, allowing you to query
evaluated positions, check if a board is known, and retrieve win/loss/distance values.

##### Load an Opening Book

```python
import bitbully_databases as bbd
import bitbully.bitbully_core as bbc

# Load the 8-ply opening book (no distances)
db_path = bbd.BitBullyDatabases.get_database_path("8-ply")
book = bbc.OpeningBookCore(db_path, is_8ply=True, with_distances=False)

print(book.getBookSize())  # e.g., 34515
print(book.getNPly())      # -> 8
```

---

##### Accessing Entries

Each entry consists of `(key, value)` where:
- **key** is the Huffman-encoded board state
- **value** is the evaluation (win/loss/draw or distance)

```python
k, v = book.getEntry(0)
print(k, v)
```

---

##### Evaluating a Board Position

```python
import bitbully.bitbully_core as bbc

board = bbc.BoardCore()
board.setBoard([2, 3, 3, 3, 3, 3, 5, 5])  # Sequence of column moves

value = book.getBoardValue(board)
print("Evaluation:", value)
```

---

##### Check Whether a Position Is in the Opening Book

The books only contain one variant for mirror-symmetric positions:

```python
board = bbc.BoardCore()
board.setBoard([1, 3, 4, 3, 4, 4, 3, 3])

print(book.isInBook(board))              # e.g., False
print(book.isInBook(board.mirror()))     # e.g., True, checks symmetric position
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

## Contributing & Development

Whether you're fixing a bug, optimizing performance, or extending BitBully with new features, contributions are highly appreciated.
The full development guide provides everything you need to work on the project efficiently:

📘 **Complete Development Documentation**
https://markusthill.github.io/BitBully/develop/

It covers all essential workflows, including:

- **Repository setup**: cloning, submodules, virtual environments
- **Development environment**: installing `dev` dependencies, using editable mode
- **Code quality tools**: ruff, mypy/pyrefly, clang-format, pre-commit, commitizen
- **Building the project**: local wheels, CMake, cibuildwheel, sdist
- **Testing**: running pytest, filtering tests, coverage, CI integration
- **Release workflow**: semantic versioning, version bumping, tagging, PyPI/TestPyPI publishing
- **Debugging & tooling**: GDB, Doxygen, mkdocs, stub generation for pybind11
- **Platform notes**: Debian/Linux setup, gcov matching, MSVC quirks
- **Cheatsheets**: Git, submodules, CMake, Docker, Ruby/Jekyll, npm, environment management

If you're contributing code, please:

1. Follow the coding standards and formatting tools (ruff, mypy, clang-format).
2. Install and run pre-commit hooks before committing.
3. Write or update tests for all behavioral changes.
4. Use Commitizen for semantic commit messages and versioning.
5. Open an issue or discussion for major changes.

Pull requests are welcome — thank you for helping improve BitBully! 🚀

---

## License

This project is licensed under the [AGPL-3.0 license](LICENSE).

---

## Contact

If you have any questions or feedback, feel free to reach out:

- **Web**: [https://markusthill.github.io](https://markusthill.github.io)
- **GitHub**: [MarkusThill](https://github.com/MarkusThill)
- **LinkedIn**: [Markus Thill](https://www.linkedin.com/in/markus-thill-a4991090)

---

## Further Ressources
- [BitBully project summary on blog](https://markusthill.github.io/projects/0_bitbully/)
- BitBully Databases project [on GitHub](https://github.com/MarkusThill/bitbully-databases) and [project summary on my blog](https://markusthill.github.io/projects/1_bitbully_databases/)
- A blog post series on tree search algorithms for Connect-4:
  - [Initial steps](https://markusthill.github.io/blog/2025/connect-4-introduction-and-tree-search-algorithms/)
  - [Tree search algorithms](https://markusthill.github.io/blog/2025/connect-4-tree-search-algorithms/)

## Acknowledgments

Many of the concepts and techniques used in this project are inspired by the outstanding Connect-4 solvers developed by
Pascal Pons and John Tromp. Their work has been invaluable in shaping this effort:

- [http://blog.gamesolver.org/](http://blog.gamesolver.org/)
- [https://github.com/PascalPons/connect4](https://github.com/PascalPons/connect4)
- https://tromp.github.io/c4/Connect4.java
- https://github.com/gamesolver/fhourstones/

---
