import numpy as np
import bitbully.bitbully_core as bbc

def test_empty_board_printable():
    b = bbc.Board()
    s = str(b)
    assert isinstance(s, str)
    assert s != "", "Printing an empty board should return a non-empty string"

def test_set_and_get_board():
    # Prepare a small, valid board
    arr = np.zeros((7, 6), dtype=int)
    arr[3, 0] = 1  # Add a yellow token in center
    b = bbc.Board()
    assert b.setBoard(arr), "Board.setBoard should accept a valid 7x6 array"
    
def test_all_positions():
    b = bbc.Board()  # empty board
    board_list_3ply = b.allPositions(3, True)  # All positions with exactly 3 tokens
    assert len(board_list_3ply) == 238, "should be 238 according to https://oeis.org/A212693"