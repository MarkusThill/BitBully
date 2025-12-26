
# Getting Started

## Installing BitBully

Install BitBully via pip:


=== "Latest"
    ```bash
    pip install bitbully
    ```

=== "With GUI Support (Jupyter Notebboks)"
    ```bash
    pip install bitbully[gui]
    ```

=== "Version x.y.z"
    ```bash
    pip install bitbully==x.y.z
    ```

## Usage

### Minimal Example: create a board and let BitBully pick a move

```python
import bitbully as bb

# Create an empty Connect-4 board
board = bb.Board()

# Create the BitBully AI (loads the default opening book)
agent = bb.BitBully()

print(board)

# Ask the AI to evaluate all legal moves
scores = agent.score_all_moves(board)
print("Move scores:", scores)

# Pick the best move and play it
best_col = agent.best_move(board)
board.play(best_col)

print("After best move:")
print(board)
```

### Play a few moves manually

```python
import bitbully as bb

board = bb.Board()

# Moves can be played as:
board.play(3)          # single column
board.play([2, 4, 3])  # list of columns
board.play("001122")   # compact move string

print(board)
```

### Simple example with a concrete position

```python
import bitbully as bb

# Create a position from a move sequence
# Players alternate automatically
board = bb.Board("33331")

print(board)

agent = bb.BitBully()

scores = agent.score_all_moves(board)
print("Move scores:", scores)

best_col = agent.best_move(board)
print("Best move:", best_col)
```

### Let the AI play a full game against itself

```python
import bitbully as bb

board = bb.Board()
agent = bb.BitBully()

while not board.is_game_over():
    col = agent.best_move(board, tie_break="random")
    board.play(col)

print(board)
print("Winner:", board.winner())  # 1, 2, or None (draw)
```

### Understanding the board state

```python
import bitbully as bb

board = bb.Board("332311")

print(board)

print("Current player:", board.current_player())
print("Legal moves:", board.legal_moves())
print("Can win next:", board.can_win_next())
print("Has win:", board.has_win())
print("Game over:", board.is_game_over())
```

### Notes for beginners

- **Column indices** range from `0` to `6` (left to right).
- `Board.play(...)` automatically alternates between players.
- `BitBully.best_move(...)` returns the selected **column index**.
- The default `BitBully` agent plays **optimal (perfect) Connect-4**.
