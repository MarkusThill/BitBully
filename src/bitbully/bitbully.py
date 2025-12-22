"""This module provides the Connect Four AI agent "BitBully" with opening book support."""

from __future__ import annotations

from dataclasses import dataclass
from typing import Literal

from bitbully import bitbully_core

from .board import Board

OpeningBookName = Literal["default", "8-ply", "12-ply", "12-ply-dist"]


@dataclass(frozen=True, slots=True)
class SearchResult:
    """Container for search results returned by BitBully.

    Attributes:
        score (int): Evaluation score returned by the engine.
        nodes (int): Number of nodes visited during the search.
    """

    score: int
    nodes: int


class BitBully:
    """A Connect Four AI agent with optional opening book support.

    This class is a high-level Python wrapper around
    :class:`bitbully_core.BitBullyCore`. It integrates the packaged
    *BitBully Databases* opening books and exposes a clean API operating
    on :class:`~bitbully.Board` objects.

    Notes:
        - If an opening book is enabled, it is used automatically for
          early-game positions.
        - For deeper positions or positions outside the database horizon,
          the solver falls back to search-based evaluation.
    """

    def __init__(self, opening_book: OpeningBookName | None = "default") -> None:
        """Initialize the BitBully agent.

        Args:
            opening_book (OpeningBookName | None):
                Which opening book to load.

                - ``"default"``: Alias for ``"12-ply-dist"``.
                - ``"8-ply"``: 8-ply book with win/loss values.
                - ``"12-ply"``: 12-ply book with win/loss values.
                - ``"12-ply-dist"``: 12-ply book with win/loss *and distance* values.
                - ``None``: Disable opening-book usage entirely.
        """
        self.opening_book_type: OpeningBookName | None = opening_book

        if opening_book is None:
            self._core = bitbully_core.BitBullyCore()
            return

        # Lazy import so `bitbully` can be used without `bitbully-databases`
        from pathlib import Path

        import bitbully_databases as bbd

        db_path = bbd.BitBullyDatabases.get_database_path(opening_book)
        self._core = bitbully_core.BitBullyCore(Path(db_path))

    def __repr__(self) -> str:
        """Return a concise string representation of the BitBully agent."""
        return f"BitBully(opening_book={self.opening_book_type!r}, book_loaded={self.is_book_loaded()})"

    def is_book_loaded(self) -> bool:
        """Check whether an opening book is loaded.

        Returns:
            bool: ``True`` if an opening book is loaded in the underlying engine,
            otherwise ``False``.
        """
        return bool(self._core.isBookLoaded())

    def reset_transposition_table(self) -> None:
        """Clear the internal transposition table.

        This is useful when benchmarking or running multiple independent
        searches to avoid cross-contamination of cached results.
        """
        self._core.resetTranspositionTable()

    def get_node_counter(self) -> int:
        """Return the number of nodes visited since the last reset.

        Returns:
            int: Number of visited nodes accumulated by the engine.
        """
        return int(self._core.getNodeCounter())

    def reset_node_counter(self) -> None:
        """Reset the internal node counter."""
        self._core.resetNodeCounter()

    def score_next_moves(self, board: Board) -> list[int]:
        """Score all legal moves for the given board state.

        This is a wrapper around
        :meth:`bitbully_core.BitBullyCore.scoreMoves`.

        Args:
            board (Board): The current board state.

        Returns:
            list[int]:
                A list of 7 integers, one per column (0-6). Higher values
                generally indicate more favorable moves for the player
                to move. The exact scale and semantics depend on the
                engine configuration and opening-book usage.
        """
        return list(self._core.scoreMoves(board._board))

    def negamax(self, board: Board, *, alpha: int, beta: int, depth: int) -> SearchResult:
        """Evaluate a position using negamax search.

        Args:
            board (Board): The board position to evaluate.
            alpha (int): Alpha bound.
            beta (int): Beta bound.
            depth (int): Search depth in plies.

        Returns:
            SearchResult: The resulting evaluation score and the number of visited nodes.
        """
        score = int(
            self._core.negamax(
                board._board,
                alpha=alpha,
                beta=beta,
                depth=depth,
            )
        )
        return SearchResult(score=score, nodes=self.get_node_counter())

    def null_window(self, board: Board) -> SearchResult:
        """Evaluate a position using a null-window search.

        Null-window search is typically used as a subroutine in MTD(f)
        or for fast bound checking.

        Args:
            board (Board): The board position to evaluate.

        Returns:
            SearchResult: The resulting evaluation score and the number of visited nodes.
        """
        score = int(self._core.nullWindow(board._board))
        return SearchResult(score=score, nodes=self.get_node_counter())

    def mtdf(self, board: Board, *, first_guess: int = 0) -> SearchResult:
        """Evaluate a position using the MTD(f) algorithm.

        MTD(f) repeatedly applies null-window searches to converge to
        the exact minimax value efficiently.

        Args:
            board (Board): The board position to evaluate.
            first_guess (int): Initial guess for the score (often 0).

        Returns:
            SearchResult: The resulting evaluation score and the number of visited nodes.
        """
        score = int(self._core.mtdf(board._board, first_guess=first_guess))
        return SearchResult(score=score, nodes=self.get_node_counter())
