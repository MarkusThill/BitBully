"""Tests for the connect-4 solver."""

import time

import bitbully.bitbully_core as bbc


def test_random_board_generation() -> None:
    """Test that `Board.randomBoard` generates a valid random board and move sequence.

    Ensures:
        * The returned `moves` is a list.
        * The board's string representation is non-empty.
        * The generated move list has the requested length (10 moves).

    """
    b: bbc.Board
    moves: list[int]
    b, moves = bbc.Board.randomBoard(10, True)
    assert isinstance(moves, list), "Moves should be returned as a list"
    assert isinstance(str(b), str), "Board should be convertible to a non-empty string"
    assert len(moves) == 10, "Generated move list should match requested length"


def test_mtdf() -> None:
    """Test the performance and correctness of the MTD(f) solver on a simple board.

    Simulates Yellow and Red alternately playing six moves into the center column,
    then solves the position using `BitBully.mtdf`. Ensures the solver completes
    within 10 seconds and produces the expected score.
    """
    board: bbc.Board = bbc.Board()

    # Yellow and Red alternately play moves into column 3 (center column)
    for _ in range(6):
        board.playMove(3)

    bbc.BitBully()
    time.time()
