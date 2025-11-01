"""Bitbully is a fast Connect-4 solver."""

import os
import typing
from typing import Sequence

import pybind11_stubgen.typing_ext

__all__: list[str] = ["BitBullyCore", "BoardCore", "OpeningBookCore"]


class BitBullyCore:
    @typing.overload
    def __init__(self) -> None: ...

    @typing.overload
    def __init__(self, openingBookPath: os.PathLike) -> None: ...

    def getNodeCounter(self) -> int:
        """Get the current node counter"""

    @typing.overload
    def isBookLoaded(self) -> bool:
        """Check, if opening book is loaded"""

    @typing.overload
    def isBookLoaded(self) -> bool:
        """Check, if opening book is loaded"""

    def mtdf(self, board: ..., first_guess: int) -> int:
        """MTD(f) algorithm"""

    def negamax(self, board: ..., alpha: int, beta: int, depth: int) -> int:
        """Negamax search"""

    def nullWindow(self, board: ...) -> int:
        """Null-window search"""

    def resetNodeCounter(self) -> None:
        """Reset the node counter"""

    def resetTranspositionTable(self) -> None:
        """Reset the transposition table"""

    def scoreMoves(self, board: ...) -> list[int]:
        """Evaluate all moves"""


class BoardCore:
    __hash__: typing.ClassVar[None] = None

    @staticmethod
    def isValid(
            board: typing.Annotated[
                list[typing.Annotated[list[int], pybind11_stubgen.typing_ext.FixedSize(6)]],
                pybind11_stubgen.typing_ext.FixedSize(7),
            ],
    ) -> bool:
        """Check, if a board is a valid one."""

    @staticmethod
    def randomBoard(nPly: int, forbidDirectWin: bool) -> tuple[BoardCore, list[int]]:
        """Create a random board with n tokens."""

    def __eq__(self, arg0: BoardCore) -> bool:
        """Check if two boards are equal"""

    @typing.overload
    def __init__(self) -> None: ...

    @typing.overload
    def __init__(self, arg0: BoardCore) -> None: ...

    def __ne__(self, arg0: BoardCore) -> bool:
        """Check if two boards are not equal"""

    def allPositions(self, upToNPly: int, exactlyN: bool) -> list[BoardCore]:
        """Generate all positions that can be reached from the current board with n tokens."""

    @typing.overload
    def canWin(self, column: int) -> bool:
        """Check, if current player can win by moving into column."""

    @typing.overload
    def canWin(self) -> bool:
        """Check, if current player can win with the next move."""

    def copy(self) -> BoardCore:
        """Create a deep copy of the board."""

    def countTokens(self) -> int:
        """Get the number of Tokens on the board"""

    def doubleThreat(self, moves: int) -> int:
        """Find double threats"""

    def findThreats(self, moves: int) -> int:
        """Find threats on the board"""

    def generateMoves(self) -> int:
        """Generate possible moves"""

    def generateNonLosingMoves(self) -> int:
        """Generate non-losing moves"""

    def hasWin(self) -> bool:
        """Check, if the player who performed the last move has a winning position (4 in a row)."""

    def hash(self) -> int:
        """Hash the current position and return hash value."""

    def isLegalMove(self, column: int) -> bool:
        """Check if a move is legal"""

    def mirror(self) -> BoardCore:
        """Get the mirrored board (mirror around center column)"""

    def movesLeft(self) -> int:
        """Get the number of moves left"""

    @typing.overload
    def play(self, column: int) -> bool:
        """Play a move by column index"""

    @typing.overload
    def play(self, moveSequence: list[int]) -> bool:
        """
        Play a sequence of moves by column index
        """

    @typing.overload
    def play(self, moveSequence: str) -> bool:
        """
        Play a sequence of moves by column index
        """

    def playMoveOnCopy(self, mv: int) -> BoardCore:
        """Play a move on a copy of the board and return the new board"""

    @typing.overload
    def setBoard(self, moveSequence: list[int]) -> bool:
        """Set the board using a 2D array"""

    @typing.overload
    def setBoard(
            self,
            moveSequence: typing.Annotated[
                list[typing.Annotated[list[int], pybind11_stubgen.typing_ext.FixedSize(6)]],
                pybind11_stubgen.typing_ext.FixedSize(7),
            ],
    ) -> bool:
        """Set the board using a 2D array"""

    @typing.overload
    def setBoard(
            self,
            moveSequence: typing.Annotated[
                list[typing.Annotated[list[int], pybind11_stubgen.typing_ext.FixedSize(7)]],
                pybind11_stubgen.typing_ext.FixedSize(6),
            ],
    ) -> bool:
        """Set the board using a 2D array"""

    @typing.overload
    def setBoard(self, moveSequence: str) -> bool:
        """Set the board using a sequence as string"""

    def sortMoves(self, moves: int) -> ...:
        """Sort moves based on priority"""

    def toArray(
            self,
    ) -> typing.Annotated[
        list[typing.Annotated[list[int], pybind11_stubgen.typing_ext.FixedSize(6)]],
        pybind11_stubgen.typing_ext.FixedSize(7),
    ]:
        """Convert the board to a 2D array representation"""

    def toHuffman(self) -> int:
        """Encode position into a huffman-code compressed sequence."""

    def toString(self) -> str:
        """Return a string representation of the board"""

    def uid(self) -> int:
        """Get the unique identifier for the board"""


class OpeningBookCore:
    @staticmethod
    def readBook(filename: os.PathLike, with_distances: bool = True, is_8ply: bool = False) -> list[tuple[int, int]]:
        """Read a book from a file."""

    @typing.overload
    def __init__(self, bookPath: os.PathLike, is_8ply: bool, with_distances: bool) -> None:
        """Initialize an OpeningBook with explicit settings."""

    @typing.overload
    def __init__(self, bookPath: os.PathLike) -> None:
        """Initialize an OpeningBook by inferring database type from file size."""

    def convertValue(self, value: int, board: BoardCore) -> int:
        """Convert a value to the internal scoring system."""

    def getBoardValue(self, board: BoardCore) -> int:
        """Get the value of a given board."""

    def getBook(self) -> list[tuple[int, int]]:
        """Return the raw book table."""

    def getBookSize(self) -> int:
        """Get the size of the book."""

    def getEntry(self, entryIdx: int) -> tuple[int, int]:
        """Get an entry from the book by index."""

    def getNPly(self) -> int:
        """Get the ply depth of the book."""

    def init(self, bookPath: os.PathLike, is_8ply: bool, with_distances: bool) -> None:
        """Reinitialize the OpeningBook with new settings."""

    def isInBook(self, board: BoardCore) -> bool:
        """Check, if the given board is in the opening book. Note, that usually boards are only present in one mirrored variant."""
