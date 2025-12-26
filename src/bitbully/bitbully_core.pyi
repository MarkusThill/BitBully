"""Low-level pybind11 bindings for BitBully (Connect-4 solver).

This module exposes the core C++ engine via pybind11. It provides:

- class [BoardCore][src.bitbully.bitbully_core.BoardCore]: Fast bitboard-based Connect-4 position representation.
- class [BitBullyCore][src.bitbully.bitbully_core.BoardCore] : Perfect-play solver (MTD(f), negamax, null-window) with optional opening book.
- class [OpeningBookCore][src.bitbully.bitbully_core.OpeningBookCore]: Opening book reader / lookup helper.
- enum [Player][src.bitbully.bitbully_core.Player]: Player enum used by the engine.
- data [N_COLUMNS][src.bitbully.bitbully_core.N_COLUMNS], data [N_ROWS][src.bitbully.bitbully_core.N_ROWS]: Board dimensions (standard Connect-4: 7x6).

Notes:
    These APIs are low-level and mirror the underlying C++ engine closely.
    Most users should prefer the high-level Python wrapper (e.g. ``bitbully.Board``,
    ``bitbully.BitBully``) unless they need maximum control or performance.
"""

import enum
import os
import typing

import pybind11_stubgen.typing_ext

__all__: list[str] = ["N_COLUMNS", "N_ROWS", "BitBullyCore", "BoardCore", "OpeningBookCore", "Player"]

N_COLUMNS: int
"""Number of columns of the standard Connect-4 board (7)."""

N_ROWS: int
"""Number of rows of the standard Connect-4 board (6)."""

class Player(enum.IntEnum):
    """Player identifiers used by the engine."""

    P_EMPTY: int
    """Empty cell marker (no token)."""

    P_YELLOW: int
    """Player 1 / Yellow token."""

    P_RED: int
    """Player 2 / Red token."""

class BitBullyCore:
    """Perfect-play Connect-4 solver implemented in C++.

    The solver evaluates positions from the perspective of the *side to move*.
    It supports multiple search methods and optional opening-book acceleration.

    Notes:
        - Column indices are 0..6 (left to right).
        - Scores are engine-defined integers; higher is better for the player to move.
        - When an opening book is loaded, early-game positions can be evaluated in
          constant time.
    """

    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, openingBookPath: os.PathLike) -> None: ...

    def getNodeCounter(self) -> int:
        """Return the number of visited nodes since the last reset.

        Returns:
            int: Number of search nodes visited since the last call to [BitBullyCore.resetNodeCounter][src.bitbully.bitbully_core.BitBullyCore.resetNodeCounter].
        """

    def isBookLoaded(self) -> bool:
        """Return whether an opening book is currently loaded.

        Returns:
            bool: ``True`` if a book is loaded, otherwise ``False``.
        """

    def mtdf(self, board: BoardCore, first_guess: int) -> int:
        """Evaluate a position using the MTD(f) algorithm.

        Args:
            board (BoardCore): Position to evaluate.
            first_guess (int): Initial guess for the score (often 0).

        Returns:
            int: Evaluation score for the side to move.
        """

    def negamax(self, board: BoardCore, alpha: int, beta: int, depth: int) -> int:
        """Evaluate a position using negamax (alpha-beta) search.

        Args:
            board (BoardCore): Position to evaluate.
            alpha (int): Alpha bound.
            beta (int): Beta bound.
            depth (int): Search depth in plies.

        Returns:
            int: Evaluation score for the side to move.
        """

    def nullWindow(self, board: BoardCore) -> int:
        """Evaluate a position using a null-window search.

        Args:
            board (BoardCore): Position to evaluate.

        Returns:
            int: Evaluation score for the side to move.
        """

    def resetNodeCounter(self) -> None:
        """Reset the internal node counter."""

    def resetTranspositionTable(self) -> None:
        """Clear the internal transposition table."""

    def scoreMove(self, board: BoardCore, column: int, first_guess: int) -> int:
        """Evaluate a single move in the given position.

        Args:
            board (BoardCore): Current position.
            column (int): Column index (0-6) of the move to evaluate.
            first_guess (int): Initial guess for the score (often 0).

        Returns:
            int: Evaluation score of playing the move in ``column``.
        """

    def scoreMoves(self, board: BoardCore) -> list[int]:
        """Evaluate all columns (0..6) in the given position.

        Args:
            board (BoardCore): Current position.

        Returns:
            list[int]: A list of length 7 with per-column scores. Illegal moves (full columns) are included and use an engine-defined sentinel value.
        """

    def loadBook(self, bookPath: os.PathLike[str] | str = ...) -> bool:
        """Load an opening book from a file path.

        Args:
            bookPath (os.PathLike[str] | str): Path to the opening book file.

        Returns:
            bool: ``True`` if the book was loaded successfully, otherwise ``False``.
        """

    def resetBook(self) -> None:
        """Unload the currently loaded opening book (if any)."""

class BoardCore:
    """Low-level Connect-4 board representation (bitboard-based).

    This class is optimized for speed and is the main input type for the solver.
    It supports playing moves, mirroring, hashing/UIDs, win checks, and move generation.

    Notes:
        - Column indices are 0..6 (left to right).
        - The side to move is part of the position state.
        - Many methods correspond 1:1 to C++ engine functions.
    """

    __hash__: typing.ClassVar[None] = None

    @staticmethod
    def isValid(
        board: typing.Annotated[
            list[typing.Annotated[list[int], pybind11_stubgen.typing_ext.FixedSize(6)]],
            pybind11_stubgen.typing_ext.FixedSize(7),
        ],
    ) -> bool:
        """Check whether a 7x6 column-major token grid is valid.

        Args:
            board (list[list[int]]): Column-major 7x6 grid (``board[col][row]``)
                with values typically in ``{0, 1, 2}`` (empty/yellow/red).

        Returns:
            bool: ``True`` if the array has the right shape and encodes a legal position according to engine rules.
        """

    @staticmethod
    def randomBoard(nPly: int, forbidDirectWin: bool) -> tuple[BoardCore, list[int]]:
        """Generate a random reachable position by playing random moves.

        Args:
            nPly (int): Number of moves (tokens) to play (0-42).
            forbidDirectWin (bool): If ``True``, ensure the generated position
                does not contain an immediate winning move for the side to move.

        Returns:
            tuple[BoardCore, list[int]]: ``(board, moves)`` where ``moves`` is the move sequence used to generate the board.
        """

    def __eq__(self, arg0: BoardCore) -> bool:
        """Compare two boards for exact position equality.

        Args:
            arg0 (BoardCore): Other board.

        Returns:
            bool: ``True`` if both boards represent the same position.
        """

    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, arg0: BoardCore) -> None: ...

    def __ne__(self, arg0: BoardCore) -> bool:
        """Compare two boards for inequality.

        Args:
            arg0 (BoardCore): Other board.

        Returns:
            bool: ``True`` if the boards differ, otherwise ``False``.
        """

    def allPositions(self, upToNPly: int, exactlyN: bool) -> list[BoardCore]:
        """Generate all reachable positions from the current board up to a ply limit.

        Args:
            upToNPly (int): Maximum total token count for generated positions.
            exactlyN (bool): If ``True``, return only positions with exactly
                ``upToNPly`` tokens. If ``False``, include all positions from the
                current ply up to ``upToNPly``.

        Returns:
            list[BoardCore]: List of generated positions.
        """

    @typing.overload
    def canWin(self, column: int) -> bool:
        """Check whether the side to move wins immediately by playing ``column``.

        Args:
            column (int): Column index (0-6) to test.

        Returns:
            bool: ``True`` if playing the move wins immediately, otherwise ``False``.
        """

    @typing.overload
    def canWin(self) -> bool:
        """Check whether the side to move has any immediate winning move.

        Returns:
            bool: ``True`` if a winning move exists, otherwise ``False``.
        """

    def copy(self) -> BoardCore:
        """Create a deep copy of the board.

        Returns:
            BoardCore: Independent copy of the current position.
        """

    def countTokens(self) -> int:
        """Return the number of tokens currently on the board.

        Returns:
            int: Token count (0-42).
        """

    def doubleThreat(self, moves: int) -> int:
        """Compute double-threat information (engine-specific).

        Args:
            moves (int): Move mask / move set parameter as expected by the engine.

        Returns:
            int: Engine-defined bitmask/encoding of detected double threats.
        """

    def findThreats(self, moves: int) -> int:
        """Compute threat information (engine-specific).

        Args:
            moves (int): Move mask / move set parameter as expected by the engine.

        Returns:
            int: Engine-defined bitmask/encoding of detected threats.
        """

    def legalMovesMask(self) -> int:
        """Return the legal moves as a bitmask.

        Returns:
            int: Bitmask encoding the set of legal columns.
        """

    def generateNonLosingMoves(self) -> int:
        """Return a bitmask of non-losing legal moves (engine definition).

        Returns:
            int: Bitmask encoding non-losing moves for the side to move.
        """

    def legalMoves(self, nonLosing: bool, orderMoves: bool) -> list[int]:
        """Return legal moves as a list of column indices.

        Args:
            nonLosing (bool): If ``True``, return only moves that do not allow the
                opponent to win immediately next turn.
            orderMoves (bool): If ``True``, order moves in an engine-defined
                heuristic order (typically center-first).

        Returns:
            list[int]: List of legal column indices.
        """

    def hasWin(self) -> bool:
        """Check whether the player who made the last move has a connect-four.

        Returns:
            bool: ``True`` if the previous player has a winning 4-in-a-row.
        """

    def hash(self) -> int:
        """Return a hash of the current position.

        Returns:
            int: Hash value suitable for hash tables / transposition tables.
        """

    def isLegalMove(self, column: int) -> bool:
        """Check whether playing in ``column`` is legal (in-range and not full).

        Args:
            column (int): Column index (0-6).

        Returns:
            bool: ``True`` if the move is legal, otherwise ``False``.
        """

    def mirror(self) -> BoardCore:
        """Return the horizontally mirrored position.

        Returns:
            BoardCore: Mirrored board (column 0 <-> 6, 1 <-> 5, 2 <-> 4).
        """

    def movesLeft(self) -> int:
        """Return the number of empty cells remaining.

        Returns:
            int: Remaining moves until the board is full (0-42).
        """

    @typing.overload
    def play(self, column: int) -> bool:
        """Play a move in the given column.

        Args:
            column (int): Column index (0-6).

        Returns:
            bool: ``True`` if the move was applied, ``False`` if it was illegal.
        """

    @typing.overload
    def play(self, moveSequence: list[int]) -> bool:
        """Play a sequence of moves.

        Args:
            moveSequence (list[int]): List of column indices (0-6).

        Returns:
            bool: ``True`` if all moves were applied, otherwise ``False``.
        """

    @typing.overload
    def play(self, moveSequence: str) -> bool:
        """Play a sequence of moves from a compact digit string.

        Args:
            moveSequence (str): String of digits, each digit is a column index.

        Returns:
            bool: ``True`` if all moves were applied, otherwise ``False``.
        """

    def playMoveOnCopy(self, mv: int) -> BoardCore:
        """Return a new board with ``mv`` applied, leaving the original unchanged.

        Args:
            mv (int): Column index (0-6).

        Returns:
            BoardCore: New board after the move. (Some builds may return a
                sentinel/invalid board if the move is illegal.)
        """

    def popCountBoard(self) -> int:
        """Return the number of occupied cells (popcount of the token bitboard).

        Returns:
            int: Number of occupied cells (0-42).
        """

    @typing.overload
    def setBoard(self, moveSequence: list[int]) -> bool:
        """Set the board by replaying a move sequence from the empty position.

        Args:
            moveSequence (list[int]): Move list (columns 0-6).

        Returns:
            bool: ``True`` if the resulting position is valid, otherwise ``False``.
        """

    @typing.overload
    def setBoard(
        self,
        moveSequence: typing.Annotated[
            list[typing.Annotated[list[int], pybind11_stubgen.typing_ext.FixedSize(6)]],
            pybind11_stubgen.typing_ext.FixedSize(7),
        ],
    ) -> bool:
        """Set the board from a 7x6 column-major token grid.

        Args:
            moveSequence (list[list[int]]): Column-major grid (7 columns x 6 rows).

        Returns:
            bool: ``True`` if the position is valid, otherwise ``False``.
        """

    @typing.overload
    def setBoard(
        self,
        moveSequence: typing.Annotated[
            list[typing.Annotated[list[int], pybind11_stubgen.typing_ext.FixedSize(7)]],
            pybind11_stubgen.typing_ext.FixedSize(6),
        ],
    ) -> bool:
        """Set the board from a 6x7 row-major token grid.

        Args:
            moveSequence (list[list[int]]): Row-major grid (6 rows x 7 columns).

        Returns:
            bool: ``True`` if the position is valid, otherwise ``False``.
        """

    @typing.overload
    def setBoard(self, moveSequence: str) -> bool:
        """Set the board by replaying a compact digit string of moves.

        Args:
            moveSequence (str): String of digits (columns 0-6).

        Returns:
            bool: ``True`` if the resulting position is valid, otherwise ``False``.
        """

    def toArray(
        self,
    ) -> typing.Annotated[
        list[typing.Annotated[list[int], pybind11_stubgen.typing_ext.FixedSize(6)]],
        pybind11_stubgen.typing_ext.FixedSize(7),
    ]:
        """Return the current position as a 7x6 column-major token grid.

        Returns:
            list[list[int]]: Column-major grid (7 columns x 6 rows).
        """

    def toHuffman(self) -> int:
        """Encode the current position into the engine's Huffman representation.

        Returns:
            int: Huffman-encoded position key used by the opening books.
        """

    def toString(self) -> str:
        """Return a human-readable ASCII rendering of the board.

        Returns:
            str: Multi-line 6x7 grid representation.
        """

    def uid(self) -> int:
        """Return a deterministic unique identifier for the current position.

        Returns:
            int: UID derived from the position (tokens + side to move).
        """

class OpeningBookCore:
    """Opening book reader and lookup helper.

    Opening books map a compact position key (Huffman encoding) to an engine score,
    optionally including distance-to-win information.
    """

    @staticmethod
    def readBook(filename: os.PathLike, with_distances: bool = True, is_8ply: bool = False) -> list[tuple[int, int]]:
        """Read an opening book file into a raw table.

        Args:
            filename (os.PathLike): Path to the book file.
            with_distances (bool): If ``True``, interpret values as including
                distance-to-win information (where supported).
            is_8ply (bool): If ``True``, interpret the file as an 8-ply book.

        Returns:
            list[tuple[int, int]]: List of ``(key, value)`` entries, where ``key``
                is the Huffman position key and ``value`` is the encoded evaluation.
        """

    @typing.overload
    def __init__(self, bookPath: os.PathLike, is_8ply: bool, with_distances: bool) -> None:
        """Initialize an opening book with explicit settings.

        Args:
            bookPath (os.PathLike): Path to the book file.
            is_8ply (bool): Whether this is an 8-ply book.
            with_distances (bool): Whether values include distance-to-win information.
        """

    @typing.overload
    def __init__(self, bookPath: os.PathLike) -> None:
        """Initialize an opening book by inferring its type from the file.

        Args:
            bookPath (os.PathLike): Path to the book file.
        """

    def convertValue(self, value: int, board: BoardCore) -> int:
        """Convert a stored book value to an engine score for the given board.

        Args:
            value (int): Raw value stored in the book table.
            board (BoardCore): Board used to interpret the value (side-to-move
                and ply can matter for distance-style values).

        Returns:
            int: Converted score in the engine's scoring convention.
        """

    def getBoardValue(self, board: BoardCore) -> int:
        """Lookup a board position in the opening book and return its value.

        Args:
            board (BoardCore): Position to query.

        Returns:
            int: Book value converted to the engine's scoring convention.
        """

    def getBook(self) -> list[tuple[int, int]]:
        """Return the raw opening book table.

        Returns:
            list[tuple[int, int]]: List of ``(key, value)`` entries.
        """

    def getBookSize(self) -> int:
        """Return the number of entries in the opening book.

        Returns:
            int: Number of stored positions.
        """

    def getEntry(self, entryIdx: int) -> tuple[int, int]:
        """Return a single raw entry by index.

        Args:
            entryIdx (int): Entry index (0-based).

        Returns:
            tuple[int, int]: The ``(key, value)`` pair at ``entryIdx``.
        """

    def getNPly(self) -> int:
        """Return the ply depth of the opening book.

        Returns:
            int: Ply depth (e.g., 8 or 12).
        """

    def init(self, bookPath: os.PathLike, is_8ply: bool, with_distances: bool) -> None:
        """Reinitialize the opening book with new settings.

        Args:
            bookPath (os.PathLike): Path to the book file.
            is_8ply (bool): Whether this is an 8-ply book.
            with_distances (bool): Whether values include distance-to-win information.
        """

    def isInBook(self, board: BoardCore) -> bool:
        """Check whether a position exists in the opening book.

        Args:
            board (BoardCore): Position to check.

        Returns:
            bool: ``True`` if the position (or its mirrored canonical form) is
                present in the book, otherwise ``False``.
        """
