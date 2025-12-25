"""This module provides the Connect Four AI agent "BitBully" with opening book support."""

from __future__ import annotations

import operator
import os
import random
from pathlib import Path
from typing import (
    Literal,
    TypeAlias,
    TypeGuard,  # typing.TypeGuard on Python 3.11+
    get_args,
)

from . import Board, bitbully_core

OpeningBookName: TypeAlias = Literal["default", "8-ply", "12-ply", "12-ply-dist"]
"""Name of the opening book used by the BitBully engine.

Possible values:
- ``"default"``: Alias for ``"12-ply-dist"``.
- ``"8-ply"``: 8-ply opening book (win/loss only).
- ``"12-ply"``: 12-ply opening book (win/loss only).
- ``"12-ply-dist"``: 12-ply opening book with distance-to-win information.
"""

TieBreakStrategy: TypeAlias = Literal["center", "leftmost", "random"]
"""Strategy for breaking ties between equally good moves.

Possible values:
- ``"center"``: Prefer moves closer to the center column.
- ``"leftmost"``: Prefer the leftmost among the best moves.
- ``"random"``: Choose randomly among the best moves.
"""


def _is_opening_book_name(x: object) -> TypeGuard[OpeningBookName]:
    return x in get_args(OpeningBookName)


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
        board, _ = Board.random_board(n_ply=14, forbid_direct_win=True)
        print(board)

        # All three search methods should agree on the score
        score_mtdf = agent.mtdf(board)
        score_negamax = agent.negamax(board)
        score_null_window = agent.null_window(board)
        assert score_negamax == score_null_window == score_mtdf
        ```

    Example:
        ```python
        from bitbully import BitBully, Board

        board = Board()  # empty board
        agent = BitBully()
        scores = agent.score_all_moves(board)  # get scores for all moves
        assert len(scores) == 7  # there are 7 columns
        assert scores == {3: 1, 2: 0, 4: 0, 1: -1, 5: -1, 0: -2, 6: -2}  # center column is best
        print(scores)
        ```

        Expected Output:
        ```
        {3: 1, 2: 0, 4: 0, 1: -1, 5: -1, 0: -2, 6: -2}
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

        See Also: [`get_node_counter`][src.bitbully.solver.BitBully.get_node_counter] for usage.
        """
        self._core.resetNodeCounter()

    def score_move(self, board: Board, column: int, first_guess: int = 0) -> int:
        """Evaluate a single move for the given board state.

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

        Raises:
            ValueError: If the column is outside the valid range or if the column is full.

        Notes:
            - This is a wrapper around
              [`bitbully_core.BitBullyCore.scoreMove`][src.bitbully.bitbully_core.BitBullyCore.scoreMove].
        """
        if not board.is_legal_move(column):
            raise ValueError(f"Column {column} is either full or invalid; cannot score move.")

        return int(self._core.scoreMove(board.native, column, first_guess))

    def score_all_moves(self, board: Board) -> dict[int, int]:
        """Score all legal moves for the given board state.

        Args:
            board (Board): The current board state.

        Returns:
            dict[int, int]:
                A dictionary of up to 7 column-value pairs, one per reachable column (0-6).
                Higher values generally indicate better moves for the player to move. If a
                column is full, it will not be listed in the returned dictionary.

        Example:
            ```python
            from bitbully import BitBully, Board

            agent = BitBully()
            board = Board()
            scores = agent.score_all_moves(board)
            assert scores == {3: 1, 2: 0, 4: 0, 1: -1, 5: -1, 0: -2, 6: -2}  # Center column is best on an empty board
        ```

        Example:
            When a column is full, it is omitted from the results:
            ```python
            from bitbully import BitBully, Board

            agent = BitBully()
            board = Board(6 * "3")  # fill center column
            scores = agent.score_all_moves(board)
            assert scores == {2: 1, 4: 1, 1: 0, 5: 0, 0: -1, 6: -1}  # Column 3 is full and thus omitted
        ```
        """
        scores = self._core.scoreMoves(board.native)
        column_values = {
            col: val for (col, val) in enumerate(scores) if val > -100
        }  # invalid moves have score less than -100
        return dict(sorted(column_values.items(), key=operator.itemgetter(1), reverse=True))

    def best_move(
        self,
        board: Board,
        *,
        tie_break: TieBreakStrategy = "center",
        rng: random.Random | None = None,
    ) -> int:
        """Return the best legal move (column index) for the current player.

        All legal moves are scored using :meth:`score_all_moves`. The move(s)
        with the highest score are considered best, and ties are resolved
        according to ``tie_break``.

        Tie-breaking strategies:
            - ``"center"`` (default):
                Prefer the move closest to the center column (3). If still tied,
                choose the smaller column index.
            - ``"leftmost"``:
                Choose the smallest column index among tied moves.
            - ``"random"``:
                Choose uniformly at random among tied moves. An optional
                ``rng`` can be provided for reproducibility.

        Args:
            board (Board): The current board state.
            tie_break (TieBreakStrategy):
                Strategy used to resolve ties between equally scoring moves.
            rng (random.Random | None):
                Random number generator used when ``tie_break="random"``.
                If ``None``, the global RNG is used.

        Returns:
            int: The selected column index (0-6).

        Raises:
            ValueError: If there are no legal moves (board is full) or
                if an unknown tie-breaking strategy is specified.

        Example:
            ```python
            from bitbully import BitBully, Board
            import random

            agent = BitBully()
            board = Board()
            best_col = agent.best_move(board)
            assert best_col == 3  # Center column is best on an empty board
            ```

        Example:
            ```python
            from bitbully import BitBully, Board
            import random

            agent = BitBully()
            board = Board("341")  # some arbitrary position
            print(board)
            assert agent.best_move(board, tie_break="center") == 3  # Several moves are tied; center is preferred
            assert agent.best_move(board, tie_break="leftmost") == 1  # Leftmost among tied moves
            assert agent.best_move(board, tie_break="random") in {1, 3, 4}  # Random among tied moves

            rng = random.Random(42)  # use own random number generator
            assert agent.best_move(board, tie_break="random", rng=rng) in {1, 3, 4}
            ```
            Expected Output:
            ```
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  X  _  X  O  _  _
            ```
        """
        scores = self.score_all_moves(board)
        if not scores:
            raise ValueError("No legal moves available (board appears to be full).")

        best_score = max(scores.values())
        best_cols = [c for c, s in scores.items() if s == best_score]

        if len(best_cols) == 1:
            return best_cols[0]

        if tie_break == "center":
            # Prefer center column (3), then smaller index for stability.
            return min(best_cols, key=lambda c: (abs(c - 3), c))

        if tie_break == "leftmost":
            return min(best_cols)

        if tie_break == "random":
            if rng is None:
                return random.choice(best_cols)
            return rng.choice(best_cols)

        raise ValueError(f"Unknown tie-breaking strategy: {tie_break!r}")

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
                board.native,
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
        return int(self._core.nullWindow(board.native))

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
        return int(self._core.mtdf(board.native, first_guess=first_guess))

    def load_book(self, book: OpeningBookName | os.PathLike) -> None:
        """Load an opening book from a file path.

        This is a thin wrapper around
        [`bitbully_core.BitBullyCore.loadBook`][src.bitbully.bitbully_core.BitBullyCore.loadBook].

        Args:
            book (OpeningBookName | os.PathLike):
                Name/Identifier (see [`available_opening_books`][src.bitbully.solver.BitBully.available_opening_books])
                or path of the opening book to load.

        Raises:
            ValueError:
                If the book identifier/path is invalid or if loading the book fails.

        Example:
            ```python
            from bitbully import BitBully
            from pathlib import Path

            which_book = BitBully.available_opening_books()[0]  # e.g., "default"

            agent = BitBully(opening_book=None)  # start without book
            assert agent.is_book_loaded() is False
            agent.load_book(which_book)  # load "default" book
            assert agent.is_book_loaded() is True
            ```

        Example:
            ```python
            from bitbully import BitBully
            from pathlib import Path
            import bitbully_databases as bbd

            which_book = BitBully.available_opening_books()[2]  # e.g., "12-ply"
            db_path = bbd.BitBullyDatabases.get_database_path(which_book)

            agent = BitBully(opening_book=None)  # start without book
            assert agent.is_book_loaded() is False
            agent.load_book(db_path)
            assert agent.is_book_loaded() is True
            ```
        """
        self._core.resetBook()
        if _is_opening_book_name(book):
            import bitbully_databases as bbd

            db_path = bbd.BitBullyDatabases.get_database_path(book)
            self.opening_book_type = book
        elif isinstance(book, (os.PathLike, str)):
            db_path = Path(book)
            self.opening_book_type = None
        else:
            raise ValueError(f"Invalid book identifier or path: {book!r}")

        if str(db_path).strip() == "":
            raise ValueError(f"Invalid book path: {book!r}")

        if not self._core.loadBook(db_path):
            self.opening_book_type = None
            self._core.resetBook()
            raise ValueError(f"Failed to load opening book from path: {db_path}")

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
        self.opening_book_type = None

    @classmethod
    def available_opening_books(cls) -> tuple[OpeningBookName, ...]:
        """Return the available opening book identifiers.

        Returns:
            tuple[OpeningBookName, ...]:
                All supported opening book names, including ``"default"``.

        Example:
            ```python
            from bitbully import BitBully

            books = BitBully.available_opening_books()
            print(books)  # ('default', '8-ply', '12-ply', '12-ply-dist')
            ```

        """
        return get_args(OpeningBookName)
