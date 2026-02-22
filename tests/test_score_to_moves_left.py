"""Tests for scoreToMovesLeft / score_to_moves_left."""

import pytest

import bitbully.bitbully_core as bbc
from bitbully import BitBully, Board

# ── Python reference implementation ──────────────────────────────────


def _py_score_to_moves_left(score: int, b: bbc.BoardCore) -> int:
    """Pure-Python reference (provided by the user).

    Args:
        score (int): The solver score.
        b (bbc.BoardCore): The board state for which the score was computed.

    Returns:
        int: Number of moves remaining until the game ends.
    """
    p = (b.movesLeft() + 1) % 2  # 1 -> yellow, 0 -> red
    sgn_score = 1 if score < 0 else 0
    mv_final_left = 2 * (abs(score) - 1) + (sgn_score ^ p) if score != 0 else 0
    return b.movesLeft() - mv_final_left


# ── Core binding tests (bbc.BitBullyCore.scoreToMovesLeft) ───────────


class TestScoreToMovesLeftCore:
    """Tests for the C++ binding BitBullyCore.scoreToMovesLeft."""

    def test_draw_empty_board(self) -> None:
        """Score 0 on an empty board means all 42 moves will be played."""
        b = bbc.BoardCore()
        assert bbc.BitBullyCore.scoreToMovesLeft(0, b) == 42

    def test_draw_after_moves(self) -> None:
        """Score 0 after some moves returns the remaining move count."""
        b = bbc.BoardCore()
        assert b.play("334411")
        assert bbc.BitBullyCore.scoreToMovesLeft(0, b) == b.movesLeft()

    def test_max_win_empty_board(self) -> None:
        """Maximum score (21) on an empty board means win in 1 move."""
        b = bbc.BoardCore()
        assert bbc.BitBullyCore.scoreToMovesLeft(21, b) == 1

    def test_max_loss_empty_board(self) -> None:
        """Minimum score (-21) on an empty board means lose in 2 moves."""
        b = bbc.BoardCore()
        assert bbc.BitBullyCore.scoreToMovesLeft(-21, b) == 2

    def test_barely_win_empty_board(self) -> None:
        """Score 1 on an empty board: yellow barely wins, 41 moves played."""
        b = bbc.BoardCore()
        assert bbc.BitBullyCore.scoreToMovesLeft(1, b) == 41

    def test_barely_lose_empty_board(self) -> None:
        """Score -1 on an empty board: yellow barely loses, 42 moves played."""
        b = bbc.BoardCore()
        assert bbc.BitBullyCore.scoreToMovesLeft(-1, b) == 42

    def test_after_one_move(self) -> None:
        """After 1 move (red to move, p=0), verify several scores."""
        b = bbc.BoardCore()
        assert b.play(3)
        assert bbc.BitBullyCore.scoreToMovesLeft(1, b) == 41  # red barely wins
        assert bbc.BitBullyCore.scoreToMovesLeft(-1, b) == 40  # red barely loses
        assert bbc.BitBullyCore.scoreToMovesLeft(21, b) == 1  # max win
        assert bbc.BitBullyCore.scoreToMovesLeft(-20, b) == 2  # max loss


# ── Binding vs. Python reference ─────────────────────────────────────


class TestBindingMatchesPythonReference:
    """Verify the C++ binding matches the pure-Python reference for all cases."""

    @pytest.mark.parametrize("score", list(range(-21, 22)))
    def test_empty_board_all_scores(self, score: int) -> None:
        """On an empty board, C++ and Python must agree for every valid score."""
        b = bbc.BoardCore()
        assert bbc.BitBullyCore.scoreToMovesLeft(score, b) == _py_score_to_moves_left(score, b)

    @pytest.mark.parametrize("score", list(range(-20, 22)))
    def test_after_one_move_all_scores(self, score: int) -> None:
        """After 1 move (odd parity), C++ and Python must agree."""
        b = bbc.BoardCore()
        assert b.play(3)
        assert bbc.BitBullyCore.scoreToMovesLeft(score, b) == _py_score_to_moves_left(score, b)

    @pytest.mark.parametrize(
        "move_sequence",
        [
            "33221144005566",  # 14 tokens
            "3333311115555",  # 13 tokens
            "3510131133424344",  # 16 tokens
            "343303342141440222",  # 18 tokens
        ],
    )
    def test_solved_positions(self, move_sequence: str) -> None:
        """After solving a position (>12 tokens), C++ and Python must agree on moves left."""
        b = bbc.BoardCore()
        assert b.play(move_sequence)
        solver = bbc.BitBullyCore()
        score = solver.mtdf(b, 0)
        assert bbc.BitBullyCore.scoreToMovesLeft(score, b) == _py_score_to_moves_left(score, b)

    @pytest.mark.parametrize(
        "move_sequence",
        [
            "3333311115555",  # 13 tokens
            "33221144005566",  # 14 tokens
            "343303342141440222",  # 18 tokens
        ],
    )
    def test_all_column_scores(self, move_sequence: str) -> None:
        """For each column score, C++ and Python must agree."""
        b = bbc.BoardCore()
        assert b.play(move_sequence)
        solver = bbc.BitBullyCore()
        scores = solver.scoreMoves(b)
        for col_score in scores:
            if col_score > -100:
                assert bbc.BitBullyCore.scoreToMovesLeft(col_score, b) == _py_score_to_moves_left(col_score, b), (
                    f"score={col_score}, moves_left={b.movesLeft()}"
                )


# ── High-level API tests (BitBully.score_to_moves_left) ─────────────


class TestHighLevelScoreToMovesLeft:
    """Tests for the Python wrapper BitBully.score_to_moves_left."""

    def test_draw_empty_board(self) -> None:
        """High-level wrapper: score 0 on empty board returns 42."""
        board = Board()
        assert BitBully.score_to_moves_left(0, board) == 42

    def test_max_win(self) -> None:
        """High-level wrapper: max score returns 1."""
        board = Board()
        assert BitBully.score_to_moves_left(21, board) == 1

    def test_with_solver(self) -> None:
        """Solve a position (>12 tokens) and verify moves left."""
        agent = BitBully(opening_book=None)
        board = Board("33221144005566")  # 14 tokens
        score = agent.mtdf(board)
        moves_left = BitBully.score_to_moves_left(score, board)
        assert 0 < moves_left <= board.moves_left()

    def test_matches_core_binding(self) -> None:
        """High-level wrapper must produce same result as core binding."""
        board = Board("33221144005566")  # 14 tokens
        agent = BitBully(opening_book=None)
        score = agent.mtdf(board)
        expected = bbc.BitBullyCore.scoreToMovesLeft(score, board.native)
        assert BitBully.score_to_moves_left(score, board) == expected


# ── Placeholder tests for user-provided positions ────────────────────


class TestScoreToMovesLeftUserPositions:
    """Placeholder tests: fill in expected values for your positions.

    All positions have >12 tokens so no opening book is needed.
    """

    @pytest.mark.parametrize(
        ("move_sequence", "expected_moves_left"),
        [
            ("33224", 2),
            ("3324556", 3),
            ("3433033", 29),
        ],
    )
    def test_user_position(self, move_sequence: str, expected_moves_left: int) -> None:
        """Solve a position and verify the expected number of moves left.

        Fill in the expected values for each move sequence.
        """
        b = bbc.BoardCore()
        assert b.play(move_sequence)
        solver = bbc.BitBullyCore()
        score = solver.mtdf(b, 0)
        moves_left = bbc.BitBullyCore.scoreToMovesLeft(score, b)
        assert moves_left == expected_moves_left, (
            f"Sequence: {move_sequence}, score: {score}, got moves_left={moves_left}, expected={expected_moves_left}"
        )
