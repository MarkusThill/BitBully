"""Low-level pybind11 bindings for BitBully (Connect-4 solver).

This module exposes the core C++ engine via pybind11. It provides:

- class [BoardCore][src.bitbully.bitbully_core.BoardCore]: Fast bitboard-based Connect-4 position representation.
- class [BitBullyCore][src.bitbully.bitbully_core.BitBullyCore] : Perfect-play solver (MTD(f), negamax, null-window) with optional opening book.
- class [OpeningBookCore][src.bitbully.bitbully_core.OpeningBookCore]: Opening book reader / lookup helper.
- enum [Player][src.bitbully.bitbully_core.Player]: Player enum used by the engine.
- data [N_COLUMNS][src.bitbully.bitbully_core.N_COLUMNS], data [N_ROWS][src.bitbully.bitbully_core.N_ROWS]: Board dimensions (standard Connect-4: 7x6).

Notes:
    These APIs are low-level and mirror the underlying C++ engine closely.
    Most users should prefer the high-level Python wrapper (e.g. ``bitbully.Board``,
    ``bitbully.BitBully``) unless they need maximum control or performance.

Example:
    Create a board, score moves, and pick a best move:
    ```python
    import bitbully.bitbully_core as bbc

    board = bbc.BoardCore()
    assert board.play("334411")
    assert isinstance(str(board), str) and str(board) != ""

    solver = bbc.BitBullyCore()
    scores = solver.scoreMoves(board)

    # One score per column.
    assert len(scores) == 7

    # Pick best column by score (ties resolved by first max).
    best_col = max(range(7), key=scores.__getitem__)
    assert 0 <= best_col < 7
    ```

"""

import enum
import os
import typing

import pybind11_stubgen.typing_ext

__all__: list[str] = ["N_COLUMNS", "N_ROWS", "BitBullyCore", "BoardCore", "OpeningBookCore", "Player"]

N_COLUMNS: int
"""Number of columns of the standard Connect-4 board (7).

Example:
    Read the board dimensions:
    ```python
    import bitbully.bitbully_core as bbc

    assert bbc.N_COLUMNS == 7
    assert bbc.N_ROWS == 6
    ```
"""

N_ROWS: int
"""Number of rows of the standard Connect-4 board (6).

Example:
    Read the board dimensions:
    ```python
    import bitbully.bitbully_core as bbc

    assert bbc.N_COLUMNS == 7
    assert bbc.N_ROWS == 6
    ```
"""


class Player(enum.IntEnum):
    """Player identifiers used by the engine.

    Example:
        Inspect numeric values used by the engine:
        ```python
        import bitbully.bitbully_core as bbc

        assert int(bbc.Player.P_EMPTY) == 0
        assert int(bbc.Player.P_YELLOW) == 1
        assert int(bbc.Player.P_RED) == 2
        ```
    """

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

    Example:
        Score all legal moves in a position:
        ```python
        import bitbully.bitbully_core as bbc
        import bitbully_databases as bbd
        db_path = bbd.BitBullyDatabases.get_database_path("default")

        board = bbc.BoardCore()
        assert board.play(6 * "3")

        solver = bbc.BitBullyCore(db_path)
        scores = solver.scoreMoves(board)

        # Scores has length 7 (one per column).
        assert len(scores) == 7

        # Pick best column by score.
        best_col = max(range(7), key=scores.__getitem__)
        print("Best column:", best_col)
        ```
        Expected output:
        ```text
            Best column: 2
        ```
        """

    @typing.overload
    def __init__(self) -> None:
        """Create a solver without an opening book.

        Example:
            Construct a solver and evaluate a non-empty board:
            ```python
            import bitbully.bitbully_core as bbc

            solver = bbc.BitBullyCore()
            board = bbc.BoardCore()
            assert board.play("334411")
            assert solver.getNodeCounter() == 0

            score = solver.mtdf(board, first_guess=0)
            assert isinstance(score, int)
            assert solver.getNodeCounter() > 0
            ```
        """
        ...

    @typing.overload
    def __init__(self, openingBookPath: os.PathLike) -> None:
        """Create a solver and load an opening book from a path.

        Args:
            openingBookPath (os.PathLike): Path to an opening book file.

        Example:
            Load a book at construction time:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            solver = bbc.BitBullyCore(db_path)
            assert solver.isBookLoaded() is True
            ```
        """
        ...

    def getNodeCounter(self) -> int:
        """Return the number of visited nodes since the last reset.

        Returns:
            int: Number of search nodes visited since the last call to
                [BitBullyCore.resetNodeCounter][src.bitbully.bitbully_core.BitBullyCore.resetNodeCounter].

        Example:
            Count how many nodes a search visited:
            ```python
            import bitbully.bitbully_core as bbc

            solver = bbc.BitBullyCore()
            board = bbc.BoardCore()
            assert board.play("333331111555")
            assert solver.getNodeCounter() == 0

            _ = solver.mtdf(board, first_guess=0)
            assert solver.getNodeCounter() > 0

            solver.resetNodeCounter()
            assert solver.getNodeCounter() == 0
            ```
        """

    def isBookLoaded(self) -> bool:
        """Return whether an opening book is currently loaded.

        Returns:
            bool: ``True`` if a book is loaded, otherwise ``False``.

        Example:
            Check if the solver currently uses an opening book:
            ```python
            import bitbully.bitbully_core as bbc

            solver = bbc.BitBullyCore()
            assert solver.isBookLoaded() is False
            ```
        """

    def mtdf(self, board: "BoardCore", first_guess: int) -> int:
        """Evaluate a position using the MTD(f) algorithm.

        Args:
            board (BoardCore): Position to evaluate.
            first_guess (int): Initial guess for the score (often 0).

        Returns:
            int: Evaluation score for the side to move.

        Example:
            Evaluate a position using MTD(f) (six moves in the center column):
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            for _ in range(6):
                assert board.play(3)

            solver = bbc.BitBullyCore()
            score = solver.mtdf(board, first_guess=0)
            print("MTD(f) score:", score)

            assert isinstance(score, int)
            assert solver.getNodeCounter() > 0
            ```
            Expected output:
            ```text
                MTD(f) score: 1
            ```
        """

    def negamax(self, board: "BoardCore", alpha: int, beta: int, depth: int) -> int:
        """Evaluate a position using negamax (alpha-beta) search.

        Args:
            board (BoardCore): Position to evaluate.
            alpha (int): Alpha bound.
            beta (int): Beta bound.
            depth (int): Search depth in plies.

        Returns:
            int: Evaluation score for the side to move.

        Example:
            Run a negamax call with an alpha-beta window:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("334411")

            solver = bbc.BitBullyCore()
            score = solver.negamax(board, alpha=-1000, beta=1000, depth=0)
            print("Negamax score:", score)
            ```
            Expected output:
            ```text
                Negamax score: 18
            ```
        """

    def nullWindow(self, board: "BoardCore") -> int:
        """Evaluate a position using a null-window search.

        Args:
            board (BoardCore): Position to evaluate.

        Returns:
            int: Evaluation score for the side to move.

        Example:
            Use null-window search:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("334411")

            solver = bbc.BitBullyCore()
            score = solver.nullWindow(board)
            print("Null-window score:", score)
            ```
            Expected output:
            ```text
                Null-window score: 18
            ```
        """

    def resetNodeCounter(self) -> None:
        """Reset the internal node counter.

        Example:
            Reset node counter between searches:
            ```python
            import bitbully.bitbully_core as bbc
            board = bbc.BoardCore()
            assert board.play("333331111555")

            solver = bbc.BitBullyCore()
            _ = solver.mtdf(board, first_guess=0)

            assert solver.getNodeCounter() > 0
            solver.resetNodeCounter()
            assert solver.getNodeCounter() == 0
            ```
        """

    def resetTranspositionTable(self) -> None:
        """Clear the internal transposition table.

        Example:
            Clear cached results (useful for benchmarking):
            ```python
            import bitbully.bitbully_core as bbc

            solver = bbc.BitBullyCore()
            solver.resetTranspositionTable()
            ```
        """

    def scoreMove(self, board: "BoardCore", column: int, first_guess: int) -> int:
        """Evaluate a single move in the given position.

        Args:
            board (BoardCore): Current position.
            column (int): Column index (0-6) of the move to evaluate.
            first_guess (int): Initial guess for the score (often 0).

        Returns:
            int: Evaluation score of playing the move in ``column``.

        Example:
            Score one candidate move:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("334411")

            solver = bbc.BitBullyCore()
            score = solver.scoreMove(board, column=3, first_guess=0)
            print("Score for column 3:", score)
            ```
            Expected output:
            ```text
                Score for column 3: 3
            ```

        Example:
            Score one candidate move and verify it matches the per-column score vector:
            ```python
            import bitbully.bitbully_core as bbc

            solver = bbc.BitBullyCore()
            board = bbc.BoardCore()
            assert board.setBoard([3, 4, 1, 1, 0, 2, 2, 2])

            scores = solver.scoreMoves(board)

            # Column 4 is known to be best in this position.
            one = solver.scoreMove(board, column=4, first_guess=0)
            assert one == scores[4] == 3
            ```
        """

    def scoreMoves(self, board: "BoardCore") -> list[int]:
        """Evaluate all columns (0..6) in the given position.

        Args:
            board (BoardCore): Current position.

        Returns:
            list[int]: A list of length 7 with per-column scores. Illegal moves
                (full columns) are included and use an engine-defined sentinel value.

        Example:
            Score all moves and pick the best:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("334411")

            solver = bbc.BitBullyCore()
            scores = solver.scoreMoves(board)
            assert len(scores) == 7

            best_col = max(range(7), key=scores.__getitem__)
            assert 0 <= best_col < 7
            assert board.isLegalMove(best_col) is True
            ```

        Example:
            Score all columns and compare against a known expected score vector:
            ```python
            import bitbully.bitbully_core as bbc

            solver = bbc.BitBullyCore()
            board = bbc.BoardCore()

            move_sequence = [3, 4, 1, 1, 0, 2, 2, 2]
            assert board.setBoard(move_sequence)

            scores = solver.scoreMoves(board)
            assert scores == [-3, -3, 1, -4, 3, -2, -2]
            ```

        Example:
            Illegal moves are included and use the engine sentinel value (-1000):
            ```python
            import bitbully.bitbully_core as bbc

            solver = bbc.BitBullyCore()
            board = bbc.BoardCore()
            assert board.setBoard([3, 3, 3, 3, 3, 3, 4, 2])

            scores = solver.scoreMoves(board)
            assert scores == [-2, -2, 2, -1000, -2, 1, -1]

            # Column 3 is full here (sentinel score).
            assert board.isLegalMove(3) is False
            assert scores[3] == -1000
            ```
        """

    def loadBook(self, bookPath: os.PathLike[str] | str = ...) -> bool:
        """Load an opening book from a file path.

        Args:
            bookPath (os.PathLike[str] | str): Path to the opening book file.

        Returns:
            bool: ``True`` if the book was loaded successfully, otherwise ``False``.

        Example:
            Load a book and verify it is active:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            solver = bbc.BitBullyCore()
            ok = solver.loadBook(db_path)  # replace with your file
            if ok:
                assert solver.isBookLoaded() is True
            ```
        """

    def resetBook(self) -> None:
        """Unload the currently loaded opening book (if any).

        Example:
            Unload a book:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            solver = bbc.BitBullyCore()
            _ = solver.loadBook(db_path)  # replace with your file
            assert solver.isBookLoaded() is True
            solver.resetBook()
            assert solver.isBookLoaded() is False
            ```
        """


class BoardCore:
    """Low-level Connect-4 board representation (bitboard-based).

    This class is optimized for speed and is the main input type for the solver.
    It supports playing moves, mirroring, hashing/UIDs, win checks, and move generation.

    Notes:
        - Column indices are 0..6 (left to right).
        - The side to move is part of the position state.
        - Many methods correspond 1:1 to C++ engine functions.

    Example:
        Create a board, play a sequence, and print:
        ```python
        import bitbully.bitbully_core as bbc

        board = bbc.BoardCore()
        assert board.play("33333111")
        print(board.toString())
        ```
        Expected output:
        ```text
        _  _  _  _  _  _  _
        _  _  _  X  _  _  _
        _  _  _  O  _  _  _
        _  O  _  X  _  _  _
        _  X  _  O  _  _  _
        _  O  _  X  _  _  _
        ```
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
            bool: ``True`` if the array has the right shape and encodes a legal
                position according to engine rules.

        Example:
            Validate a board grid before setting it:
            ```python
            import bitbully.bitbully_core as bbc

            grid = [[0] * 6 for _ in range(7)]
            grid[3][0] = int(bbc.Player.P_YELLOW)
            assert bbc.BoardCore.isValid(grid) is True

            grid[3][0] = 3  # invalid token
            assert bbc.BoardCore.isValid(grid) is False
            ```
        """

    @staticmethod
    def randomBoard(nPly: int, forbidDirectWin: bool) -> tuple["BoardCore", list[int]]:
        """Generate a random reachable position by playing random moves.

        Args:
            nPly (int): Number of moves (tokens) to play (0-42).
            forbidDirectWin (bool): If ``True``, ensure the generated position
                does not contain an immediate winning move for the side to move.

        Returns:
            tuple[BoardCore, list[int]]: ``(board, moves)`` where ``moves`` is the
                move sequence used to generate the board.

        Example:
            Create a random 8-ply position and show the move sequence:
            ```python
            import bitbully.bitbully_core as bbc

            board, moves = bbc.BoardCore.randomBoard(nPly=8, forbidDirectWin=True)
            assert len(moves) == 8

            print("Moves:", moves)
            print(board)
            ```
        """

    def __eq__(self, arg0: "BoardCore") -> bool:
        """Compare two boards for exact position equality.

        Args:
            arg0 (BoardCore): Other board.

        Returns:
            bool: ``True`` if both boards represent the same position.

        Example:
            Compare two independently built boards:
            ```python
            import bitbully.bitbully_core as bbc

            b1 = bbc.BoardCore()
            b2 = bbc.BoardCore()
            assert b1.play("3344")
            assert b2.play("3344")

            assert b1 == b2
            ```
        """

    @typing.overload
    def __init__(self) -> None:
        """Create an empty board.

        Example:
            Create an empty board and confirm it has 0 tokens:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.countTokens() == 0
            assert board.movesLeft() == 42
            ```
        """
        ...

    @typing.overload
    def __init__(self, arg0: "BoardCore") -> None:
        """Copy-construct a board.

        Args:
            arg0 (BoardCore): Board to copy.

        Example:
            Create a copy and confirm both positions match:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("33333111")

            board_copy = bbc.BoardCore(board)
            assert board == board_copy
            assert board.uid() == board_copy.uid()
            ```
        """
        ...

    def __ne__(self, arg0: "BoardCore") -> bool:
        """Compare two boards for inequality.

        Args:
            arg0 (BoardCore): Other board.

        Returns:
            bool: ``True`` if the boards differ, otherwise ``False``.

        Example:
            Build two different positions and compare:
            ```python
            import bitbully.bitbully_core as bbc

            b1 = bbc.BoardCore()
            b2 = bbc.BoardCore()
            assert b1.play("3")
            assert b2.play("4")

            assert b1 != b2
            ```
        """

    def allPositions(self, upToNPly: int, exactlyN: bool) -> list["BoardCore"]:
        """Generate all reachable positions from the current board up to a ply limit.

        Args:
            upToNPly (int): Maximum total token count for generated positions.
            exactlyN (bool): If ``True``, return only positions with exactly
                ``upToNPly`` tokens. If ``False``, include all positions from the
                current ply up to ``upToNPly``.

        Returns:
            list[BoardCore]: List of generated positions.

        Example:
            Enumerate all positions with exactly 2 tokens from the empty board:
            ```python
            import bitbully.bitbully_core as bbc

            root = bbc.BoardCore()
            positions = root.allPositions(upToNPly=2, exactlyN=True)

            assert all(p.countTokens() == 2 for p in positions)
            print("Count:", len(positions))
            ```
            Expected output:
            ```text
                Count: 49
            ```
        """

    @typing.overload
    def canWin(self, column: int) -> bool:
        """Check whether the side to move wins immediately by playing ``column``.

        Args:
            column (int): Column index (0-6) to test.

        Returns:
            bool: ``True`` if playing the move wins immediately, otherwise ``False``.

        Example:
            Construct a position where the side to move wins by playing column 3:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play([3, 2, 3, 2, 3, 2])  # next move in 3 wins
            assert board.canWin(3) is True
            ```
        """

    @typing.overload
    def canWin(self) -> bool:
        """Check whether the side to move has any immediate winning move.

        Returns:
            bool: ``True`` if a winning move exists, otherwise ``False``.

        Example:
            Check if any immediate win exists:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play([3, 2, 3, 2, 3, 2])
            assert board.canWin() is True
            ```
        """

    def copy(self) -> "BoardCore":
        """Create a deep copy of the board.

        Returns:
            BoardCore: Independent copy of the current position.

        Example:
            Create a board, copy it, and verify that both represent the same position:
            ```python
            import bitbully.bitbully_core as bbc

            # Create a board from a compact move string.
            board = bbc.BoardCore()
            assert board.play("33333111")

            # Create an independent copy of the current position.
            board_copy = board.copy()

            # Both boards represent the same position and are considered equal.
            assert board == board_copy
            assert board.uid() == board_copy.uid()
            assert board.toString() == board_copy.toString()

            # Display the board state.
            print(board.toString())
            ```
            Expected output:
            ```text
              _  _  _  _  _  _  _
              _  _  _  X  _  _  _
              _  _  _  O  _  _  _
              _  O  _  X  _  _  _
              _  X  _  O  _  _  _
              _  O  _  X  _  _  _
            ```
        """

    def countTokens(self) -> int:
        """Return the number of tokens currently on the board.

        Returns:
            int: Token count (0-42).

        Example:
            Count tokens after playing a move string:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("3344")
            assert board.countTokens() == 4
            ```
        """

    def doubleThreat(self, moves: int) -> int:
        """Compute double-threat information (engine-specific).

        Args:
            moves (int): Move mask / move set parameter as expected by the engine.
                A typical input is the result of ``legalMovesMask()``.

        Returns:
            int: Engine-defined bitmask/encoding of detected double threats.
        """

    def findThreats(self, moves: int) -> int:
        """Compute threat information (engine-specific).

        Args:
            moves (int): Move mask / move set parameter as expected by the engine.
                A typical input is the result of ``legalMovesMask()``.

        Returns:
            int: Engine-defined bitmask/encoding of detected threats.
        """

    def legalMovesMask(self) -> int:
        """Return the legal moves as a bitmask.

        Returns:
            int: Bitmask encoding the set of legal moves (engine bitboard format).

        Example:
            Get the move mask and verify it is non-zero on non-full boards:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            mask = board.legalMovesMask()
            assert isinstance(mask, int)
            assert mask != 0
            ```
        """

    def generateNonLosingMoves(self) -> int:
        """Return a bitmask of non-losing legal moves (engine definition).

        Returns:
            int: Bitmask encoding non-losing moves for the side to move.

        Example:
            Generate non-losing moves and compare with all legal moves:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("334411")

            legal = board.legalMovesMask()
            non_losing = board.generateNonLosingMoves()

            # Non-losing is a subset of legal (bitwise).
            assert (non_losing & legal) == non_losing
            ```
        """

    def legalMoves(self, nonLosing: bool, orderMoves: bool) -> list[int]:
        """Return legal moves as a list of column indices.

        Args:
            nonLosing (bool): If ``True``, return only moves that do not allow the
                opponent to win immediately next turn (engine definition).
            orderMoves (bool): If ``True``, order moves in an engine-defined
                heuristic order (typically center-first).

        Returns:
            list[int]: List of legal column indices.

        Example:
            Get ordered legal moves (center-first):
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            cols = board.legalMoves(nonLosing=False, orderMoves=True)

            assert all(0 <= c < 7 for c in cols)
            print(cols)
            ```
            Expected output:
            ```text
            [3, 2, 4, 1, 5, 0, 6]
            ```
        """

    def hasWin(self) -> bool:
        """Check whether the player who made the last move has a connect-four.

        Returns:
            bool: ``True`` if the previous player has a winning 4-in-a-row.

        Example:
            Play a winning sequence and check that the last mover has a win:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play([3, 2, 3, 2, 3, 2, 3])
            assert board.hasWin() is True
            ```
        """

    def hash(self) -> int:
        """Return a hash of the current position.

        Returns:
            int: Hash value suitable for hash tables / transposition tables.

        Example:
            Hashes match for identical positions and differ after divergence:
            ```python
            import bitbully.bitbully_core as bbc

            b1 = bbc.BoardCore()
            b2 = bbc.BoardCore()
            assert b1.play("334411")
            assert b2.play("334411")
            assert b1.hash() == b2.hash()

            assert b1.play(1)
            assert b1.hash() != b2.hash()
            ```
        """

    def isLegalMove(self, column: int) -> bool:
        """Check whether playing in ``column`` is legal (in-range and not full).

        Args:
            column (int): Column index (0-6).

        Returns:
            bool: ``True`` if the move is legal, otherwise ``False``.

        Example:
            Check legality before playing:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            if board.isLegalMove(3):
                assert board.play(3)
            ```
        """

    def mirror(self) -> "BoardCore":
        """Return the horizontally mirrored position.

        Returns:
            BoardCore: Mirrored board (column 0 <-> 6, 1 <-> 5, 2 <-> 4).

        Example:
            Mirror twice returns the original position:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.setBoard([0, 1, 2])  # simple asymmetric position

            mirrored = board.mirror()
            assert mirrored != board
            assert mirrored.countTokens() == board.countTokens()
            assert mirrored.mirror() == board
            ```
        """

    def movesLeft(self) -> int:
        """Return the number of empty cells remaining.

        Returns:
            int: Remaining moves until the board is full (0-42).

        Example:
            Confirm moves left after some moves:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("3344")
            assert board.movesLeft() == 42 - 4
            ```
        """

    @typing.overload
    def play(self, column: int) -> bool:
        """Play a move in the given column.

        Args:
            column (int): Column index (0-6).

        Returns:
            bool: ``True`` if the move was applied, ``False`` if it was illegal.

        Example:
            Play a single move:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play(3) is True
            ```
        """

    @typing.overload
    def play(self, moveSequence: list[int]) -> bool:
        """Play a sequence of moves.

        Args:
            moveSequence (list[int]): List of column indices (0-6).

        Returns:
            bool: ``True`` if all moves were applied, otherwise ``False``.

        Example:
            Play a move list:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play([3, 2, 3, 2]) is True
            ```
        """

    @typing.overload
    def play(self, moveSequence: str) -> bool:
        """Play a sequence of moves from a compact digit string.

        Args:
            moveSequence (str): String of digits, each digit is a column index.

        Returns:
            bool: ``True`` if all moves were applied, otherwise ``False``.

        Example:
            Play a move string:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("332211") is True
            ```
        """

    def playMoveOnCopy(self, mv: int) -> "BoardCore":
        """Return a new board with ``mv`` applied, leaving the original unchanged.

        Args:
            mv (int): Column index (0-6).

        Returns:
            BoardCore: New board after the move.

        Example:
            Try a move without mutating the original board:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            board2 = board.playMoveOnCopy(3)

            assert board.countTokens() == 0
            assert board2.countTokens() == 1
            ```
        """

    def popCountBoard(self) -> int:
        """Return the number of occupied cells (popcount of the token bitboard).

        Returns:
            int: Number of occupied cells (0-42).

        Example:
            Compare popCountBoard() with countTokens():
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("334411")
            assert board.popCountBoard() == board.countTokens()
            ```
        """

    @typing.overload
    def setBoard(self, moveSequence: list[int]) -> bool:
        """Set the board by replaying a move sequence from the empty position.

        Args:
            moveSequence (list[int]): Move list (columns 0-6).

        Returns:
            bool: ``True`` if the resulting position is valid, otherwise ``False``.

        Example:
            Reset a board to a known sequence:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.setBoard([3, 3, 2, 2]) is True
            ```
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

        Example:
            Set the board from a column-major grid:
            ```python
            import bitbully.bitbully_core as bbc

            grid = [[0] * 6 for _ in range(7)]
            grid[3][0] = int(bbc.Player.P_YELLOW)

            board = bbc.BoardCore()
            assert board.setBoard(grid) is True
            ```
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

        Example:
            Set the board from a row-major grid:
            ```python
            import bitbully.bitbully_core as bbc

            grid = [[0] * 7 for _ in range(6)]
            grid[5][3] = int(bbc.Player.P_YELLOW)

            board = bbc.BoardCore()
            assert board.setBoard(grid) is True
            ```
        """

    @typing.overload
    def setBoard(self, moveSequence: str) -> bool:
        """Set the board by replaying a compact digit string of moves.

        Args:
            moveSequence (str): String of digits (columns 0-6).

        Returns:
            bool: ``True`` if the resulting position is valid, otherwise ``False``.

        Example:
            Set board from a compact move string:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.setBoard("334411") is True
            ```
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

        Example:
            Convert to an array and inspect dimensions:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("3")

            arr = board.toArray()
            assert len(arr) == 7
            assert len(arr[0]) == 6
            ```
        """

    def toHuffman(self) -> int:
        """Encode the current position into the engine's Huffman representation.

        Returns:
            int: Huffman-encoded position key used by the opening books.

        Example:
            Compute a Huffman key (only defined for certain positions):
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()

            # Huffman encoding is only defined for some positions in this engine
            # (e.g., even number of tokens and <= 12 ply in your C++ code).
            assert board.play("33331111")  # 8 tokens
            key = board.toHuffman()
            print("Huffman key:", key)
            ```
            Expected output:
            ```text
                Huffman key: 6133600
            ```
        """

    def toString(self) -> str:
        """Return a human-readable ASCII rendering of the board.

        Returns:
            str: Multi-line 6x7 grid representation.

        Example:
            Print a board:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("33333111")
            print(board.toString())
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  O  _  X  _  _  _
            _  X  _  O  _  _  _
            _  O  _  X  _  _  _
            ```
        """

    def uid(self) -> int:
        """Return a deterministic unique identifier for the current position.

        Returns:
            int: UID derived from the position (tokens + side to move).

        Example:
            Use ``uid()`` as a stable key for caching:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("334411")

            cache: dict[int, str] = {}
            cache[board.uid()] = board.toString()
            assert board.uid() in cache
            ```
        """

    def getColumnHeight(self, column: int) -> int:
        """Return the number of tokens in the given column.

        Args:
            column (int): Column index (0-6).

        Returns:
            int: Number of tokens in the column (0-6).

        Example:
            Check column heights after some moves:
            ```python
            import bitbully.bitbully_core as bbc

            board = bbc.BoardCore()
            assert board.play("3332211" + 6 * "5")

            assert board.getColumnHeight(0) == 0
            assert board.getColumnHeight(1) == 2
            assert board.getColumnHeight(2) == 2
            assert board.getColumnHeight(3) == 3
            assert board.getColumnHeight(4) == 0
            assert board.getColumnHeight(5) == 6
            assert board.getColumnHeight(6) == 0
            ```
        """


class OpeningBookCore:
    """Opening book reader and lookup helper.

    Opening books map a compact position key (Huffman encoding) to an engine score,
    optionally including distance-to-win information.

    Example:
        Load a book and check whether a position is contained:
        ```python
        import bitbully.bitbully_core as bbc
        import bitbully_databases as bbd
        db_path = bbd.BitBullyDatabases.get_database_path("default")

        board = bbc.BoardCore()
        assert board.play("333331111555")

        book = bbc.OpeningBookCore(db_path)  # replace with your file
        assert book.isInBook(board)
        ```
    """

    @staticmethod
    def readBook(
        filename: os.PathLike,
        with_distances: bool = True,
        is_8ply: bool = False,
    ) -> list[tuple[int, int]]:
        """Read an opening book file into a raw table.

        Args:
            filename (os.PathLike): Path to the book file.
            with_distances (bool): If ``True``, interpret values as including
                distance-to-win information (where supported).
            is_8ply (bool): If ``True``, interpret the file as an 8-ply book.

        Returns:
            list[tuple[int, int]]: List of ``(key, value)`` entries.

        Example:
            Read the raw book table:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            table = bbc.OpeningBookCore.readBook(
                db_path,
                with_distances=True,
                is_8ply=False,
            )
            print("Entries:", len(table))
            ```
            Expected output:
            ```text
                Entries: 4200899
            ```
        """

    @typing.overload
    def __init__(self, bookPath: os.PathLike, is_8ply: bool, with_distances: bool) -> None:
        """Initialize an opening book with explicit settings.

        Args:
            bookPath (os.PathLike): Path to the book file.
            is_8ply (bool): Whether this is an 8-ply book.
            with_distances (bool): Whether values include distance-to-win information.

        Example:
            Load an 8-ply book with win/loss-only values:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("8-ply")

            book = bbc.OpeningBookCore(
                db_path,
                is_8ply=True,
                with_distances=False,
            )
            print("Book size:", book.getBookSize())
            ```
            Expected output:
            ```text
                Book size: 34515
            ```
        """
        ...

    @typing.overload
    def __init__(self, bookPath: os.PathLike) -> None:
        """Initialize an opening book by inferring its type from the file.

        Args:
            bookPath (os.PathLike): Path to the book file.

        Example:
            Let the engine infer book type:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            book = bbc.OpeningBookCore(db_path)
            print("NPly:", book.getNPly())
            ```
            Expected output:
            ```text
                NPly: 12
            ```
        """
        ...

    def convertValue(self, value: int, board: "BoardCore") -> int:
        """Convert a stored book value to an engine score for the given board.

        Args:
            value (int): Raw value stored in the book table.
            board (BoardCore): Board used to interpret the value.

        Returns:
            int: Converted score in the engine's scoring convention.

        Example:
            Convert a raw entry value using the current board context:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            board = bbc.BoardCore()
            book = bbc.OpeningBookCore(db_path)  # replace with your file

            key, raw_val = book.getEntry(0)
            score = book.convertValue(raw_val, board)
            print("Converted score:", score)
            ```
            Expected output:
            ```text
                Converted score: 9
            ```
        """

    def getBoardValue(self, board: "BoardCore") -> int:
        """Lookup a board position in the opening book and return its value.

        Args:
            board (BoardCore): Position to query.

        Returns:
            int: Book value converted to the engine's scoring convention.

        Example:
            Query a position's book value:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            board = bbc.BoardCore()
            assert board.play("333331111555")

            book = bbc.OpeningBookCore(db_path)  # replace with your file
            if book.isInBook(board):
                print("Value:", book.getBoardValue(board))
            ```
            Expected output:
            ```text
                Value: 1
            ```
        """

    def getBook(self) -> list[tuple[int, int]]:
        """Return the raw opening book table.

        Returns:
            list[tuple[int, int]]: List of ``(key, value)`` entries.

        Example:
            Access the raw table and inspect the first entry:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            book = bbc.OpeningBookCore(db_path)  # replace with your file
            raw = book.getBook()

            key0, val0 = raw[0]
            print("First entry:", key0, val0)
            ```
            Expected output:
            ```text
                First entry: -2124988676 75
            ```
        """

    def getBookSize(self) -> int:
        """Return the number of entries in the opening book.

        Returns:
            int: Number of stored positions.

        Example:
            Print the number of stored positions:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            book = bbc.OpeningBookCore(db_path)  # replace with your file
            print(book.getBookSize())
            ```
            Expected output:
            ```text
                4200899
            ```
        """

    def getEntry(self, entryIdx: int) -> tuple[int, int]:
        """Return a single raw entry by index.

        Args:
            entryIdx (int): Entry index (0-based).

        Returns:
            tuple[int, int]: The ``(key, value)`` pair at ``entryIdx``.

        Example:
            Read a single entry:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            book = bbc.OpeningBookCore(db_path)  # replace with your file
            key, val = book.getEntry(0)

            print("Entry 0:", key, val)
            ```
            Expected output:
            ```text
                Entry 0: -2124988676 75
            ```
        """

    def getNPly(self) -> int:
        """Return the ply depth of the opening book.

        Returns:
            int: Ply depth (e.g., 8 or 12).

        Example:
            Print the book depth:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("default")

            book = bbc.OpeningBookCore(db_path)  # replace with your file
            print("NPly:", book.getNPly())
            ```
            Expected output:
            ```text
                NPly: 12
            ```
        """

    def init(self, bookPath: os.PathLike, is_8ply: bool, with_distances: bool) -> None:
        """Reinitialize the opening book with new settings.

        Args:
            bookPath (os.PathLike): Path to the book file.
            is_8ply (bool): Whether this is an 8-ply book.
            with_distances (bool): Whether values include distance-to-win information.

        Example:
            Reinitialize an existing instance:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd
            db_path = bbd.BitBullyDatabases.get_database_path("12-ply-dist")

            book = bbc.OpeningBookCore(db_path)  # replace with your file
            book.init(db_path, is_8ply=False, with_distances=True)
            ```
        """

    def isInBook(self, board: "BoardCore") -> bool:
        """Check whether a position exists in the opening book.

        Args:
            board (BoardCore): Position to check.

        Returns:
            bool: ``True`` if the position (or its mirrored canonical form) is present in the book, otherwise ``False``.

        Example:
            Check membership before lookup:
            ```python
            import bitbully.bitbully_core as bbc
            import bitbully_databases as bbd

            db_path = bbd.BitBullyDatabases.get_database_path("default")

            board = bbc.BoardCore()
            assert board.play("334411")

            book = bbc.OpeningBookCore(db_path)

            in_book = book.isInBook(board)
            if in_book:
                _ = book.getBoardValue(board)

            assert isinstance(in_book, bool)
            ```

        """
