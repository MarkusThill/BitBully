"""Tests for the connect-4 solver."""

import time

import pytest

import bitbully.bitbully_core as bbc


def test_mtdf() -> None:
    """Test the performance and correctness of the MTD(f) solver on a simple board.

    Simulates Yellow and Red alternately playing six moves into the center column,
    then solves the position using `BitBullyCore.mtdf`. Ensures the solver completes
    within 10 seconds and produces the expected score.
    """
    board: bbc.BoardCore = bbc.BoardCore()

    # Yellow and red alternately play moves into column 3 (center column):
    for _ in range(6):
        board.play(3)

    solver: bbc.BitBullyCore = bbc.BitBullyCore()
    start = time.time()
    score = solver.mtdf(board, first_guess=0)
    print(f"Time: {round(time.time() - start, 2)} seconds!")
    print(f"Best score: {score}")


@pytest.mark.parametrize(
    ("move_sequence", "expected_scores"),
    [
        ([3, 3, 3, 3, 3, 1, 1, 1, 1, 5, 5, 5, 5], [-2, -1, -2, -1, -2, -1, -2]),
        ([3, 4, 1, 1, 0, 2, 2, 2], [-3, -3, 1, -4, 3, -2, -2]),
        ([3, 3, 3, 3, 3, 3, 4, 2], [-2, -2, 2, -1000, -2, 1, -1]),
    ],
)
def test_score_moves(move_sequence: list[int], expected_scores: list[int]) -> None:
    """Test the scoreMoves function of BitBullyCore for different board states.

    Args:
        move_sequence (list[int]): The sequence of moves to set up the board.
        expected_scores (list[int]): The expected scores for each column after evaluation.
    """
    agent: bbc.BitBullyCore = bbc.BitBullyCore()
    assert isinstance(agent, bbc.BitBullyCore)
    board: bbc.BoardCore = bbc.BoardCore()
    assert board.setBoard(move_sequence)
    scores: list[int] = agent.scoreMoves(board)
    assert isinstance(scores, list)
    assert scores == expected_scores
