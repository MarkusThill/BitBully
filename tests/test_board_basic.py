"""Test basic board functionality."""

import pytest

import bitbully.bitbully_core as bbc


def test_empty_board_printable() -> None:
    """Verify that converting an empty Board to a string produces a non-empty, human-readable representation.

    This ensures that the Board class implements `__str__` correctly and
    does not return an empty string when no moves have been made.
    """
    b: bbc.Board = bbc.Board()
    s: str = str(b)
    assert isinstance(s, str)
    assert s != "", "Printing an empty board should return a non-empty string"


@pytest.mark.skip(
    reason="Temporarily skipping since numpy is compilied from scratch " "on certain runners which takes forever."
)
def test_set_and_get_board() -> None:
    """Validate that a 7x6 NumPy array can be set on a Board instance.

    A single yellow token is placed in the center column, and
    `Board.setBoard` is expected to accept this valid configuration.
    """
    import numpy as np

    arr: np.ndarray = np.zeros((7, 6), dtype=int)
    arr[3, 0] = 1  # Add a yellow token in the center column
    b: bbc.Board = bbc.Board()
    assert b.setBoard(arr), "Board.setBoard should accept a valid 7x6 array"


def test_all_positions() -> None:
    """Verify the correct number of positions with a specified ply depth.

    For a 3-ply depth, the number of possible positions starting from an empty
    board should be exactly 238, as documented in
    https://oeis.org/A212693.
    """
    b: bbc.Board = bbc.Board()  # Empty board
    board_list_3ply: list[bbc.Board] = b.allPositions(3, True)
    assert len(board_list_3ply) == 238, "Expected 238 positions for 3-ply search according to https://oeis.org/A212693"
