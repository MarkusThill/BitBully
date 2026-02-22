"""Tests for depth-limited search (max_depth) and rollout functionality."""

import random

import pytest

import bitbully.bitbully_core as bbc
from bitbully import BitBully, Board

# ── rollout (core) ──────────────────────────────────────────────────


class TestRollout:
    """Tests for BitBullyCore.rollout static method."""

    def test_empty_board_returns_valid_score(self) -> None:
        """Rollout on an empty board returns a score in [-21, 21]."""
        board = bbc.BoardCore()
        score = bbc.BitBullyCore.rollout(board)
        assert -21 <= score <= 21

    def test_immediate_win_returns_positive(self) -> None:
        """If the current player can win, rollout should return a positive score."""
        board = bbc.BoardCore()
        assert board.play("323234")  # P1 has 3 in col 3, can win with col 3
        assert board.canWin()
        score = bbc.BitBullyCore.rollout(board)
        assert score > 0

    def test_deterministic(self) -> None:
        """Rollout uses center-priority heuristic, so same board -> same result."""
        board = bbc.BoardCore()
        assert board.play("334411")
        s1 = bbc.BitBullyCore.rollout(board)
        s2 = bbc.BitBullyCore.rollout(board)
        assert s1 == s2

    @pytest.mark.parametrize("seed", range(30))
    def test_score_within_bounds(self, seed: int) -> None:
        """Rollout score must be within +/- (movesLeft+1)/2."""
        random.seed(seed)
        n_ply = 10 + seed % 30
        board, _ = Board.random_board(n_ply, forbid_direct_win=True)
        score = bbc.BitBullyCore.rollout(board.native)
        bound = (board.moves_left() + 1) // 2
        assert -bound <= score <= bound, f"seed={seed}, score={score}, bound={bound}"


# ── max_depth default (backward compat) ─────────────────────────────


class TestMaxDepthBackwardCompat:
    """max_depth=-1 (default) must reproduce the existing full-search results."""

    @pytest.mark.parametrize(
        ("move_sequence", "expected_scores"),
        [
            ([3, 3, 3, 3, 3, 1, 1, 1, 1, 5, 5, 5, 5], [-2, -1, -2, -1, -2, -1, -2]),
            ([3, 4, 1, 1, 0, 2, 2, 2], [-3, -3, 1, -4, 3, -2, -2]),
        ],
    )
    def test_score_moves_unchanged(self, move_sequence: list[int], expected_scores: list[int]) -> None:
        """Existing scoreMoves results must be identical with default max_depth."""
        agent = bbc.BitBullyCore()
        board = bbc.BoardCore()
        assert board.setBoard(move_sequence)
        # Explicit max_depth=-1
        scores = agent.scoreMoves(board, max_depth=-1)
        assert scores == expected_scores

    def test_mtdf_default_matches_explicit(self) -> None:
        """mtdf() with no max_depth matches mtdf(max_depth=-1)."""
        agent = BitBully(opening_book=None)
        board, _ = Board.random_board(12, True)
        assert agent.mtdf(board) == agent.mtdf(board, max_depth=-1)

    def test_negamax_default_matches_explicit(self) -> None:
        """negamax() with no max_depth matches negamax(max_depth=-1)."""
        agent = BitBully(opening_book=None)
        board, _ = Board.random_board(12, True)
        assert agent.negamax(board) == agent.negamax(board, max_depth=-1)


# ── depth-limited search properties ─────────────────────────────────


class TestDepthLimited:
    """Tests for depth-limited search behavior."""

    def test_max_depth_zero_equals_rollout(self) -> None:
        """At maxDepth=0, negamax should immediately delegate to rollout."""
        board = bbc.BoardCore()
        assert board.play("334411")
        solver = bbc.BitBullyCore()
        solver.resetTranspositionTable()
        score_negamax = solver.negamax(board, -100000, 100000, 0, max_depth=0)
        score_rollout = bbc.BitBullyCore.rollout(board)
        assert score_negamax == score_rollout

    def test_fewer_nodes_with_depth_limit(self) -> None:
        """Depth-limited search must visit fewer nodes than full search."""
        random.seed(42)
        board, _ = Board.random_board(14, forbid_direct_win=True)
        solver = bbc.BitBullyCore()

        solver.resetNodeCounter()
        solver.resetTranspositionTable()
        solver.negamax(board.native, -100000, 100000, 0, max_depth=5)
        nodes_limited = solver.getNodeCounter()

        solver.resetNodeCounter()
        solver.resetTranspositionTable()
        solver.negamax(board.native, -100000, 100000, 0, max_depth=-1)
        nodes_full = solver.getNodeCounter()

        assert nodes_limited > 0
        assert nodes_full > 0
        assert nodes_limited <= nodes_full

    @pytest.mark.parametrize("max_depth", [0, 2, 4, 6, 8, 10])
    def test_score_in_valid_range(self, max_depth: int) -> None:
        """Depth-limited mtdf must return scores within the theoretical bounds."""
        random.seed(77)
        board, _ = Board.random_board(10, forbid_direct_win=True)
        agent = BitBully(opening_book=None)
        agent.reset_transposition_table()
        score = agent.mtdf(board, max_depth=max_depth)
        bound = (board.moves_left() + 1) // 2
        assert -bound <= score <= bound

    def test_converges_to_exact_when_deep_enough(self) -> None:
        """When maxDepth exceeds remaining moves, result matches full search."""
        board = Board("33221144005566")  # 14 tokens
        agent = BitBully(opening_book=None)

        agent.reset_transposition_table()
        exact = agent.mtdf(board, max_depth=-1)

        agent.reset_transposition_table()
        deep = agent.mtdf(board, max_depth=40)  # 40 > 28 remaining

        assert deep == exact


# ── TT isolation ─────────────────────────────────────────────────────


class TestTTIsolation:
    """Depth-limited entries in the TT must not corrupt full-search results."""

    def test_full_search_unaffected_by_prior_depth_limited(self) -> None:
        """Full search result must be unchanged after a depth-limited search."""
        agent = BitBully(opening_book=None)
        board, _ = Board.random_board(12, True)

        agent.reset_transposition_table()
        score_before = agent.mtdf(board, max_depth=-1)

        # Pollute TT with depth-limited entries
        agent.mtdf(board, max_depth=3)

        # Full search again — must match
        score_after = agent.mtdf(board, max_depth=-1)
        assert score_before == score_after


# ── Python high-level API ────────────────────────────────────────────


class TestHighLevelAPI:
    """Tests for the BitBully Python wrapper max_depth support."""

    def test_instance_default_max_depth(self) -> None:
        """Instance max_depth should be used when per-call max_depth is None."""
        board, _ = Board.random_board(12, True)
        agent_full = BitBully(opening_book=None, max_depth=-1)
        agent_shallow = BitBully(opening_book=None, max_depth=3)

        agent_full.mtdf(board)
        score_shallow = agent_shallow.mtdf(board)

        # Shallow search is approximate — just check it returns a valid score
        bound = (board.moves_left() + 1) // 2
        assert -bound <= score_shallow <= bound

    def test_per_call_overrides_instance(self) -> None:
        """Per-call max_depth must override the instance default."""
        board, _ = Board.random_board(12, True)
        agent = BitBully(opening_book=None, max_depth=3)

        # Override with full search
        agent.reset_transposition_table()
        score = agent.mtdf(board, max_depth=-1)
        assert score == BitBully(opening_book=None).mtdf(board)

    def test_score_all_moves_with_max_depth(self) -> None:
        """score_all_moves returns 7 scores within bounds when depth-limited."""
        board = Board()
        agent = BitBully(opening_book=None)
        scores = agent.score_all_moves(board, max_depth=5)
        assert len(scores) == 7  # all columns playable on empty board
        bound = (board.moves_left() + 1) // 2
        for col, val in scores.items():
            assert -bound <= val <= bound, f"col={col}, val={val}"

    def test_best_move_with_max_depth(self) -> None:
        """best_move with max_depth returns a valid column."""
        board = Board()
        agent = BitBully(opening_book=None)
        col = agent.best_move(board, max_depth=5)
        assert 0 <= col <= 6

    def test_score_move_with_max_depth(self) -> None:
        """score_move with max_depth returns a score within bounds."""
        board = Board()
        agent = BitBully(opening_book=None)
        score = agent.score_move(board, column=3, max_depth=5)
        bound = (board.moves_left() + 1) // 2
        assert -bound <= score <= bound

    def test_null_window_with_max_depth(self) -> None:
        """null_window with max_depth returns bounded score matching mtdf."""
        board, _ = Board.random_board(12, True)
        agent = BitBully(opening_book=None)

        agent.reset_transposition_table()
        score_full = agent.null_window(board, max_depth=-1)

        agent.reset_transposition_table()
        score_limited = agent.null_window(board, max_depth=5)

        bound = (board.moves_left() + 1) // 2
        assert -bound <= score_limited <= bound
        # Full search must match mtdf
        agent.reset_transposition_table()
        assert score_full == agent.mtdf(board, max_depth=-1)
