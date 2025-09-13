"""Test the opening book functionality."""

import importlib.resources

from bitbully import bitbully_core as bbc


def test_12_ply_with_distance() -> None:
    """Validate that BitBully correctly scores an empty Connect-4 board.

    This test loads the precomputed distance database
    `book_12ply_distances.dat`, creates an empty `Board`, and verifies
    that `BitBully.scoreMoves` returns the expected heuristic scores
    for each of the seven columns.
    """
    db_path = importlib.resources.files("bitbully").joinpath("assets/book_12ply_distances.dat")
    bitbully: bbc.BitBully = bbc.BitBully(db_path)
    b: bbc.Board = bbc.Board()  # Empty board
    assert bitbully.scoreMoves(b) == [
        -2,
        -1,
        0,
        1,
        0,
        -1,
        -2,
    ], "expected result: [-2, -1, 0, 1, 0, -1, -2]"
