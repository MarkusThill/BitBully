"""This module provides the Connect Four AI agent "BitBully" with opening book support."""

from __future__ import annotations

from typing import Literal, TypeAlias

from . import bitbully_core
from .board import Board

OpeningBookName: TypeAlias = Literal["default", "8-ply", "12-ply", "12-ply-dist"]
"""Name of the opening book used by the BitBully engine.

Possible values:
- ``"default"``: Alias for ``"12-ply-dist"``.
- ``"8-ply"``: 8-ply opening book (win/loss only).
- ``"12-ply"``: 12-ply opening book (win/loss only).
- ``"12-ply-dist"``: 12-ply opening book with distance-to-win information.
"""


class BitBully:
    """A Connect Four AI agent with optional opening book support.

    Todo:
    - We have to describe the scoring scheme (range of values and their meaning).

    This class is a high-level Python wrapper around
    [`bitbully_core.BitBullyCore`][src.bitbully.bitbully_core.BitBullyCore].
    It integrates the packaged *BitBully Databases* opening books and
    operates on [`bitbully.Board`][src.bitbully.board.Board] objects.

    Notes:
        - If an opening book is enabled, it is used automatically for
          early-game positions.
        - For deeper positions or positions outside the database horizon,
          the engine falls back to search-based evaluation.

    Example:
        ```python
        from bitbully import BitBully, Board

        agent = BitBully()
        board, _ = Board.random_board(14, forbid_direct_win=True)
        print(board)

        # All three search methods should agree on the score
        score_mtdf = agent.mtdf(board)
        score_negamax = agent.negamax(board)
        score_null_window = agent.null_window(board)
        assert score_negamax == score_null_window == score_mtdf
        ```

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

        TODO: Example for initialization with different books.

        """
        self.opening_book_type: OpeningBookName | None = opening_book

        if opening_book is None:
            self._core = bitbully_core.BitBullyCore()
            return

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
            bool: ``True`` if an opening book is loaded, otherwise ``False``.

        Example:
            ```python
            from bitbully import BitBully

            agent = BitBully()  # per default, the 12-ply-dist book is loaded
            assert agent.is_book_loaded() is True

            # Unload the book
            agent.reset_book()
            assert agent.is_book_loaded() is False
            ```
        """
        return bool(self._core.isBookLoaded())

    def reset_transposition_table(self) -> None:
        """Clear the internal transposition table."""
        self._core.resetTranspositionTable()

    def get_node_counter(self) -> int:
        """Return the number of nodes visited since the last reset.

        Returns:
            int: Number of visited nodes.

        Example:
            ```python
            from bitbully import BitBully, Board

            agent = BitBully()
            board = Board()
            _ = agent.score_all_moves(board)
            print(f"Nodes visited: {agent.get_node_counter()}")

            # Note that has to be reset manually:
            agent.reset_node_counter()
            assert agent.get_node_counter() == 0
            ```
        """
        return int(self._core.getNodeCounter())

    def reset_node_counter(self) -> None:
        """Reset the internal node counter.

        Example:
        See [`get_node_counter`][src.bitbully.bitbully.BitBully.get_node_counter] for usage.
        """
        self._core.resetNodeCounter()

    def score_move(self, board: Board, column: int, first_guess: int = 0) -> int:
        """Evaluate a single move for the given board state.

        This is a wrapper around
        [`bitbully_core.BitBullyCore.scoreMove`][src.bitbully.bitbully_core.BitBullyCore.scoreMove].

        Args:
            board (Board): The current board state.
            column (int): Column index (0-6) of the move to evaluate.
            first_guess (int): Initial guess for the score (often 0).

        Returns:
            int: The evaluation score of the move.

        Example:
            ```python
            from bitbully import BitBully, Board

            agent = BitBully()
            board = Board()
            score = agent.score_move(board, column=3)
            assert score == 1  # Score for the center column on an empty board
            ```
        """
        return int(self._core.scoreMove(board._board, column, first_guess))

    def score_all_moves(self, board: Board) -> list[int]:
        """Score all legal moves for the given board state.

        Args:
            board (Board): The current board state.

        Returns:
            list[int]:
                A list of 7 integers, one per column (0-6). Higher values
                generally indicate better moves for the player to move.

        Example:
            ```python
            from bitbully import BitBully, Board

            agent = BitBully()
            board = Board()
            scores = agent.score_all_moves(board)
            assert scores == [-2, -1, 0, 1, 0, -1, -2]  # Center column is best on an empty board
        ```
        """
        return list(self._core.scoreMoves(board._board))

    def negamax(self, board: Board, alpha: int = -1000, beta: int = 1000, depth: int = 0) -> int:
        """Evaluate a position using negamax search.

        Args:
            board (Board): The board position to evaluate.
            alpha (int): Alpha bound.
            beta (int): Beta bound.
            depth (int): Search depth in plies.

        Returns:
            int: The evaluation score returned by the engine.

        Example:
            ```python
            from bitbully import BitBully, Board

            agent = BitBully()
            board = Board()
            score = agent.negamax(board)
            assert score == 1  # Expected score for an empty board
            ```
        """
        return int(
            self._core.negamax(
                board._board,
                alpha=alpha,
                beta=beta,
                depth=depth,
            )
        )

    def null_window(self, board: Board) -> int:
        """Evaluate a position using a null-window search.

        Args:
            board (Board): The board position to evaluate.

        Returns:
            int: The evaluation score.

        Example:
            ```python
            from bitbully import BitBully, Board

            agent = BitBully()
            board = Board()
            score = agent.null_window(board)
            assert score == 1  # Expected score for an empty board
            ```
        """
        return int(self._core.nullWindow(board._board))

    def mtdf(self, board: Board, first_guess: int = 0) -> int:
        """Evaluate a position using the MTD(f) algorithm.

        Args:
            board (Board): The board position to evaluate.
            first_guess (int): Initial guess for the score (often 0).

        Returns:
            int: The evaluation score.

        Example:
            ```python
            from bitbully import BitBully, Board

            agent = BitBully()
            board = Board()
            score = agent.mtdf(board)
            assert score == 1  # Expected score for an empty board
            ```
        """
        return int(self._core.mtdf(board._board, first_guess=first_guess))

    def load_book(self, book_path: str) -> bool:
        """Load an opening book from a file path.

        This is a thin wrapper around
        [`bitbully_core.BitBullyCore.loadBook`][src.bitbully.bitbully_core.BitBullyCore.loadBook].

        Args:
            book_path (str):
                Path to the opening book file. If empty or invalid, no book
                is loaded.

        Returns:
            bool:
                ``True`` if the book was loaded successfully,
                ``False`` otherwise.

        Example:
            ```python
            from bitbully import BitBully
            from pathlib import Path
            import bitbully_databases as bbd

            db_path = bbd.BitBullyDatabases.get_database_path("default")

            agent = BitBully(opening_book=None)  # start without book
            assert agent.is_book_loaded() is False
            success = agent.load_book(db_path)
            assert agent.is_book_loaded() is True
            ```
        """
        return bool(self._core.loadBook(book_path))

    def reset_book(self) -> None:
        """Unload the currently loaded opening book (if any).

        This resets the engine to *search-only* mode until another
        opening book is loaded.

        Example:
            ```python
            from bitbully import BitBully

            agent = BitBully()  # per default, the 12-ply-dist book is loaded
            assert agent.is_book_loaded() is True
            agent.reset_book()
            assert agent.is_book_loaded() is False
            ```
        """
        self._core.resetBook()
