
# Getting Started

## Installing BitBully

Install BitBully via pip:


=== "Latest"
    ```bash
    pip install bitbully
    ```

=== "Version x.y.z"
    ```bash
    pip install bitbully==x.y.z
    ```

## Usage

### Simple Example

```python
from bitbully import Board, BitBully

# Create an empty Connect Four board
board = Board()  # Board is empty at the start

# Players YELLOW and RED alternately drop 5 tokens into the center column (column 3)
for _ in range(5):
    board.play_move(3)  # Place token in center column (index 3)

# RED places one token into the second column from the left (column 1)
board.play_move(1)

print(board)
# Expected output:
#  _  _  _  _  _  _  _
#  _  _  _  X  _  _  _
#  _  _  _  O  _  _  _
#  _  _  _  X  _  _  _
#  _  _  _  O  _  _  _
#  _  O  _  X  _  _  _

# Use the BitBully AI to evaluate the current board position
bb_agent = BitBully()
scores = bb_agent.score_next_moves(board)
print("Move scores:", scores)
# Expected output: [-2, 1, -2, -3, 0, 0, -3]

# Now YELLOW must also play in column 1 to win the game
# (column indices start at 0, so column 1 is the second from the left)
```
