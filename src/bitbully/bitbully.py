"""BitBully: A Connect Four AI agent with opening book support."""

# from importlib.resources.abc import Traversable
from typing import Literal

from bitbully import bitbully_core

from .board import Board


class BitBully:
    """A Connect Four AI agent with opening book support."""

    def __init__(self, opening_book: Literal["default", "8-ply", "12-ply", "12-ply-dist"] | None = "default") -> None:
        """Initializes the BitBully agent with the specified opening book.

        Args:
            opening_book (Literal["default", "8-ply", "12-ply", "12-ply-dist"] | None):
                The type of opening book to use. Options are:
                - "default": Uses the default 12-ply distance-based opening book.
                - "8-ply": Uses an 8-ply opening book.
                - "12-ply": Uses a 12-ply opening book.
                - "12-ply-dist": Uses a 12-ply distance-based opening book.
                - None: No opening book will be used.
        """
        from pathlib import Path

        import bitbully_databases as bbd

        self.opening_book_type = opening_book

        if opening_book:
            db_path = bbd.BitBullyDatabases.get_database_path(opening_book)
            self.bitbully_agent = bitbully_core.BitBullyCore(Path(db_path))
        else:
            self.bitbully_agent = bitbully_core.BitBullyCore()

    def score_next_moves(self, board: Board) -> list[int]:
        """Scores all possible moves for the given board state.

        Args:
            board (Board):
                The current board state.

        Returns:
            list[int]: A list of scores for each column (0-6).
        """
        return self.bitbully_agent.scoreMoves(board._board)
