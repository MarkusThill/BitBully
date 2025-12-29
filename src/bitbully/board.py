"""This module defines the Board class for managing the state of a Connect Four game."""

from __future__ import annotations  # for forward references in type hints (Python 3.7+)

from collections.abc import Sequence
from typing import Any, ClassVar, cast

from . import bitbully_core
from .bitbully_core import BoardCore


class Board:
    """Represents the state of a Connect Four board. Mostly a thin wrapper around BoardCore."""

    # class-level constants
    N_COLUMNS: ClassVar[int] = bitbully_core.N_COLUMNS
    N_ROWS: ClassVar[int] = bitbully_core.N_ROWS

    Player = bitbully_core.Player

    def __init__(self, init_with: Sequence[Sequence[int]] | Sequence[int] | str | None = None) -> None:
        """Initializes a Board instance.

        Args:
            init_with (Sequence[Sequence[int]] | Sequence[int] | str | None):
                Optional initial board state. Accepts:
                - 2D array (list, tuple, numpy-array) with shape 7x6 or 6x7
                - 1D sequence of ints: a move sequence of columns (e.g., [0, 0, 2, 2, 3, 3])
                - String: A move sequence of columns as string (e.g., "002233")
                - None for an empty board

        Raises:
            ValueError: If the provided initial board state is invalid.

        Example:
            You can initialize an empty board in multiple ways:
            ```python
            import bitbully as bb

            # Create an empty board using the default constructor.
            board = bb.Board()  # Starts with no tokens placed.

            # Alternatively, initialize the board explicitly from a 2D list.
            # Each inner list represents a column (7 columns total, 6 rows each).
            # A value of 0 indicates an empty cell; 1 and 2 would represent player tokens.
            board = bb.Board([[0] * 6 for _ in range(7)])  # Equivalent to an empty board.

            # You can also set up a specific board position manually using a 6 x 7 layout,
            # where each inner list represents a row instead of a column.
            # (Both layouts are accepted by BitBully for convenience.)
            # For more complex examples using 2D arrays, see the examples below.
            board = bb.Board([[0] * 7 for _ in range(6)])  # Also equivalent to an empty board.

            # Display the board in text form.
            # The __repr__ method shows the current state (useful for debugging or interactive use).
            board
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            ```

        The recommended way to initialize an empty board is simply `Board()`.

        Example:
            You can also initialize a board with a sequence of moves:
            ```python
            import bitbully as bb

            # Initialize a board with a sequence of moves played in the center column.

            # The list [3, 3, 3] represents three moves in column index 3 (zero-based).
            # Moves alternate automatically between Player 1 (yellow, X) and Player 2 (red, O).
            # After these three moves, the center column will contain:
            #   - Row 0: Player 1 token (bottom)
            #   - Row 1: Player 2 token
            #   - Row 2: Player 1 token
            board = bb.Board([3, 3, 3])

            # Display the resulting board.
            # The textual output shows the tokens placed in the center column.
            board
            ```

            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  _  _  X  _  _  _
            ```

        Example:
            You can also initialize a board using a string containing a move sequence:
            ```python
            import bitbully as bb

            # Initialize a board using a compact move string.

            # The string "33333111" represents a sequence of eight moves:
            #   3 3 3 3 3 → five moves in the center column (index 3)
            #   1 1 1 → three moves in the second column (index 1)
            #
            # Moves are applied in order, alternating automatically between Player 1 (yellow, X)
            # and Player 2 (red, O), just as if you had called `board.play()` repeatedly.
            #
            # This shorthand is convenient for reproducing board states or test positions
            # without having to provide long move lists.

            board = bb.Board("33333111")

            # Display the resulting board.
            # The printed layout shows how the tokens stack in each column.
            board
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

        Example:
            You can also initialize a board using a 2D array (list of lists):
            ```python
            import bitbully as bb

            # Use a 6 x 7 list (rows x columns) to set up a specific board position manually.

            # Each inner list represents a row of the Connect-4 grid.
            # Convention:
            #   - 0 → empty cell
            #   - 1 → Player 1 token (yellow, X)
            #   - 2 → Player 2 token (red, O)
            #
            # The top list corresponds to the *top row* (row index 5),
            # and the bottom list corresponds to the *bottom row* (row index 0).
            # This layout matches the typical visual display of the board.

            board_array = [
                [0, 0, 0, 0, 0, 0, 0],  # Row 5 (top)
                [0, 0, 0, 1, 0, 0, 0],  # Row 4: Player 1 token in column 3
                [0, 0, 0, 2, 0, 0, 0],  # Row 3: Player 2 token in column 3
                [0, 2, 0, 1, 0, 0, 0],  # Row 2: tokens in columns 1 and 3
                [0, 1, 0, 2, 0, 0, 0],  # Row 1: tokens in columns 1 and 3
                [0, 2, 0, 1, 0, 0, 0],  # Row 0 (bottom): tokens stacked lowest
            ]

            # Create a Board instance directly from the 2D list.
            # This allows reconstructing arbitrary positions (e.g., from test data or saved states)
            # without replaying the move sequence.
            board = bb.Board(board_array)

            # Display the resulting board state in text form.
            board
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

        Example:
            You can also initialize a board using a 2D (7 x 6) array with columns as inner lists:
            ```python
            import bitbully as bb

            # Use a 7 x 6 list (columns x rows) to set up a specific board position manually.

            # Each inner list represents a **column** of the Connect-4 board, from left (index 0)
            # to right (index 6). Each column contains six entries — one for each row, from
            # bottom (index 0) to top (index 5).
            #
            # Convention:
            #   - 0 → empty cell
            #   - 1 → Player 1 token (yellow, X)
            #   - 2 → Player 2 token (red, O)
            #
            # This column-major layout matches the internal representation used by BitBully,
            # where tokens are dropped into columns rather than filled row by row.

            board_array = [
                [0, 0, 0, 0, 0, 0],  # Column 0 (leftmost)
                [2, 1, 2, 0, 0, 0],  # Column 1
                [0, 0, 0, 0, 0, 0],  # Column 2
                [1, 2, 1, 2, 1, 0],  # Column 3 (center)
                [0, 0, 0, 0, 0, 0],  # Column 4
                [0, 0, 0, 0, 0, 0],  # Column 5
                [0, 0, 0, 0, 0, 0],  # Column 6 (rightmost)
            ]

            # Create a Board instance directly from the 2D list.
            # This allows reconstructing any arbitrary position (e.g., test cases, saved games)
            # without replaying all moves individually.
            board = bb.Board(board_array)

            # Display the resulting board.
            # The text output shows tokens as they would appear in a real Connect-4 grid.
            board
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
        self._board = BoardCore()
        if init_with is not None and not self.reset_board(init_with):
            raise ValueError(
                "Invalid initial board state provided. Check the examples in the docstring for valid formats."
            )

    def __eq__(self, value: object) -> bool:
        """Checks equality between two Board instances.

        Notes:
            - Equality checks in BitBully compare the *exact board state* (bit patterns),
              not just the move history.
            - Two different move sequences can still yield the same position if they
              result in identical token configurations.
            - This is useful for comparing solver states, verifying test positions,
              or detecting transpositions in search algorithms.

        Args:
            value (object): The other Board instance to compare against.

        Returns:
            bool: True if both boards are equal, False otherwise.

        Raises:
            NotImplementedError: If the other value is not a Board instance.

        Example:
            ```python
            import bitbully as bb

            # Create two boards that should represent *identical* game states.
            board1 = bb.Board()
            assert board1.play("33333111")

            board2 = bb.Board()
            # Play the same position step by step using a different but equivalent sequence.
            # Internally, the final bitboard state will match `board1`.
            assert board2.play("31133331")

            # Boards with identical token placements are considered equal.
            # Equality (`==`) and inequality (`!=`) operators are overloaded for convenience.
            assert board1 == board2
            assert not (board1 != board2)

            # ------------------------------------------------------------------------------

            # Create two boards that differ by one move.
            board1 = bb.Board("33333111")
            board2 = bb.Board("33333112")  # One extra move in the last column (index 2)

            # Since the token layout differs, equality no longer holds.
            assert board1 != board2
            assert not (board1 == board2)
            ```
        """
        if not isinstance(value, Board):
            raise NotImplementedError("Can only compare with another Board instance.")
        return bool(self._board == value._board)

    def __ne__(self, value: object) -> bool:
        """Checks inequality between two Board instances.

        See the documentation for [Board.__eq__][src.bitbully.board.Board.__eq__] for details.

        Args:
            value (object): The other Board instance to compare against.

        Returns:
            bool: True if both boards are not equal, False otherwise.
        """
        return not self.__eq__(value)

    def __repr__(self) -> str:
        """Returns a string representation of the Board instance."""
        return f"{self._board}"

    def __str__(self) -> str:
        """Return a human-readable ASCII representation (same as to_string()).

        See the documentation for [Board.to_string][src.bitbully.board.Board.to_string] for details.
        """
        return self.to_string()

    def all_positions(self, up_to_n_ply: int, exactly_n: bool) -> list[Board]:
        """Find all positions reachable from the current position up to a given ply.

        This is a high-level wrapper around
        `bitbully_core.BoardCore.allPositions`.

        Starting from the **current** board, it generates all positions that can be
        reached by playing additional moves such that the resulting position has:

        - At most ``up_to_n_ply`` tokens on the board, if ``exactly_n`` is ``False``.
        - Exactly ``up_to_n_ply`` tokens on the board, if ``exactly_n`` is ``True``.

        Note:
            The number of tokens already present in the current position is taken
            into account. If ``up_to_n_ply`` is smaller than
            ``self.count_tokens()``, the result is typically empty.

            This function can grow combinatorially with ``up_to_n_ply`` and the
            current position, so use it with care for large depths.

        Args:
            up_to_n_ply (int):
                The maximum total number of tokens (ply) for generated positions.
                Must be between 0 and 42 (inclusive).
            exactly_n (bool):
                If ``True``, only positions with exactly ``up_to_n_ply`` tokens
                are returned. If ``False``, all positions with a token count
                between the current number of tokens and ``up_to_n_ply`` are
                included.

        Returns:
            list[Board]: A list of :class:`Board` instances representing all
            reachable positions that satisfy the ply constraint.

        Raises:
            ValueError: If ``up_to_n_ply`` is outside the range ``[0, 42]``.

        Example:
            Compute all positions at exactly 3 ply from the empty board:

            ```python
            import bitbully as bb

            # Start from an empty board.
            board = bb.Board()

            # Generate all positions that contain exactly 3 tokens.
            positions = board.all_positions(3, exactly_n=True)

            # According to OEIS A212693, there are exactly 238 distinct
            # reachable positions with 3 played moves in standard Connect-4.
            assert len(positions) == 238
            ```

            Reference:
                - Number of distinct positions at ply *n*:
                  https://oeis.org/A212693

        """
        if not 0 <= up_to_n_ply <= 42:
            raise ValueError(f"up_to_n_ply must be between 0 and 42 (inclusive), got {up_to_n_ply}.")

        # Delegate to the C++ core, which returns a list of BoardCore objects.
        core_positions = self._board.allPositions(up_to_n_ply, exactly_n)

        # Wrap each BoardCore in a high-level Board instance.
        positions: list[Board] = []
        for core_board in core_positions:
            b = Board()  # start with an empty high-level Board
            b._board = core_board  # replace its internal BoardCore
            positions.append(b)

        return positions

    def can_win_next(self, move: int | None = None) -> bool:
        """Checks if the current player can win in the next move.

        Args:
            move (int | None): Optional column to check for an immediate win. If None, checks all columns.

        Returns:
            bool: True if the current player can win next, False otherwise.

        See also: [`Board.has_win`][src.bitbully.board.Board.has_win].

        Example:
            ```python
            import bitbully as bb

            # Create a board from a move string.
            # The string "332311" represents a short sequence of alternating moves
            # that results in a nearly winning position for Player 1 (yellow, X).
            board = bb.Board("332311")

            # Display the current board state (see below)
            print(board)

            # Player 1 (yellow, X) — who is next to move — can win immediately
            # by placing a token in either column 0 or column 4.
            assert board.can_win_next(0)
            assert board.can_win_next(4)

            # However, playing in other columns does not result in an instant win.
            assert not board.can_win_next(2)
            assert not board.can_win_next(3)

            # You can also call `can_win_next()` without arguments to perform a general check.
            # It returns True if the current player has *any* winning move available.
            assert board.can_win_next()
            ```
            The board we created above looks like this:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  O  _  _  _
            _  O  _  O  _  _  _
            _  X  X  X  _  _  _
            ```
        """
        if move is None:
            return self._board.canWin()
        return bool(self._board.canWin(move))

    def copy(self) -> Board:
        """Creates a copy of the current Board instance.

        The `copy()` method returns a new `Board` object that represents the
        *same position* as the original at the time of copying. Subsequent
        changes to one board do **not** affect the other — they are completely
        independent.

        Returns:
            Board: A new Board instance that is a copy of the current one.

        Example:
            Create a board, copy it, and verify that both represent the same position:
            ```python
            import bitbully as bb

            # Create a board from a compact move string.
            board = bb.Board("33333111")

            # Create an independent copy of the current position.
            board_copy = board.copy()

            # Both boards represent the same position and are considered equal.
            assert board == board_copy
            assert hash(board) == hash(board_copy)
            assert board.to_string() == board_copy.to_string()

            # Display the board state.
            print(board)
            ```
            Expected output (both boards print the same position):
            ```text
            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  O  _  X  _  _  _
            _  X  _  O  _  _  _
            _  O  _  X  _  _  _
            ```

        Example:
            Modifying the copy does not affect the original:
            ```python
            import bitbully as bb

            board = bb.Board("33333111")

            # Create a copy of the current position.
            board_copy = board.copy()

            # Play an additional move on the copied board only.
            assert board_copy.play(0)  # Drop a token into the leftmost column.

            # Now the boards represent different positions.
            assert board != board_copy

            # The original board remains unchanged.
            print("Original:")
            print(board)

            print("Modified copy:")
            print(board_copy)
            ```
            Expected output:
            ```text
            Original:

            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  O  _  X  _  _  _
            _  X  _  O  _  _  _
            _  O  _  X  _  _  _

            Modified copy:

            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  O  _  X  _  _  _
            _  X  _  O  _  _  _
            X  O  _  X  _  _  _
            ```
        """
        new_board = Board()
        new_board._board = self._board.copy()
        return new_board

    def count_tokens(self) -> int:
        """Counts the total number of tokens currently placed on the board.

        This method simply returns how many moves have been played so far in the
        current position — that is, the number of occupied cells on the 7x6 grid.

        It does **not** distinguish between players; it only reports the total
        number of tokens, regardless of whether they belong to Player 1 or Player 2.

        Returns:
            int: The total number of tokens on the board (between 0 and 42).

        Example:
            Count tokens on an empty board:
            ```python
            import bitbully as bb

            board = bb.Board()  # No moves played yet.
            assert board.count_tokens() == 0

            # The board is completely empty.
            print(board)
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            ```

        Example:
            Count tokens after a few moves:
            ```python
            import bitbully as bb

            # Play three moves in the center column (index 3).
            board = bb.Board()
            assert board.play([3, 3, 3])

            # Three tokens have been placed on the board.
            assert board.count_tokens() == 3

            print(board)
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  _  _  X  _  _  _
            ```

        Example:
            Relation to the length of a move sequence:
            ```python
            import bitbully as bb

            moves = "33333111"  # 8 moves in total
            board = bb.Board(moves)

            # The number of tokens on the board always matches
            # the number of moves that have been played.
            # (as long as the input was valid)
            assert board.count_tokens() == len(moves)
            ```
        """
        return self._board.countTokens()

    def has_win(self) -> bool:
        """Checks if the current player has a winning position.

        Returns:
            bool: True if the current player has a winning position (4-in-a-row), False otherwise.

        Unlike `can_win_next()`, which checks whether the current player *could* win
        on their next move, the `has_win()` method determines whether a winning
        condition already exists on the board.
        This method is typically used right after a move to verify whether the game
        has been won.

        See also: [`Board.can_win_next`][src.bitbully.board.Board.can_win_next].

        Example:
            ```python
            import bitbully as bb

            # Initialize a board from a move sequence.
            # The string "332311" represents a position where Player 1 (yellow, X)
            # is one move away from winning.
            board = bb.Board("332311")

            # At this stage, Player 1 has not yet won, but can win immediately
            # by placing a token in either column 0 or column 4.
            assert not board.has_win()
            assert board.can_win_next(0)  # Check column 0
            assert board.can_win_next(4)  # Check column 4
            assert board.can_win_next()  # General check (any winning move)

            # Simulate Player 1 playing in column 4 — this completes
            # a horizontal line of four tokens and wins the game.
            assert board.play(4)

            # Display the updated board to visualize the winning position.
            print(board)

            # The board now contains a winning configuration:
            # Player 1 (yellow, X) has achieved a Connect-4.
            assert board.has_win()
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  O  _  _  _
            _  O  _  O  _  _  _
            _  X  X  X  X  _  _
            ```
        """
        return self._board.hasWin()

    def __hash__(self) -> int:
        """Returns a hash of the Board instance for use in hash-based collections.

        Returns:
            int: The hash value of the Board instance.

        Example:
            ```python
            import bitbully as bb

            # Create two boards that represent the same final position.
            # The first board is initialized directly from a move string.
            board1 = bb.Board("33333111")

            # The second board is built incrementally by playing an equivalent sequence of moves.
            # Even though the order of intermediate plays differs, the final layout of tokens
            # (and thus the internal bitboard state) will be identical to `board1`.
            board2 = bb.Board()
            board2.play("31133331")

            # Boards with identical configurations produce the same hash value.
            # This allows them to be used efficiently as keys in dictionaries or members of sets.
            assert hash(board1) == hash(board2)

            # Display the board's hash value.
            hash(board1)
            ```
            Expected output:
            ```text
            971238920548618160
            ```
        """
        return self._board.hash()

    def is_legal_move(self, move: int) -> bool:
        """Checks if a move (column) is legal in the current position.

        A move is considered *legal* if:

        - The column index is within the valid range (0-6), **and**
        - The column is **not full** (i.e. it still has at least one empty cell).

        This method does **not** check for tactical consequences such as
        leaving an immediate win to the opponent, nor does it stop being
        usable once a player has already won. It purely validates whether a
        token can be dropped into the given column according to the basic
        rules of Connect Four. You have to check for wins separately using
        [Board.has_win][src.bitbully.board.Board.has_win].


        Args:
            move (int): The column index (0-6) to check.

        Returns:
            bool: True if the move is legal, False otherwise.

        Example:
            All moves are legal on an empty board:
            ```python
            import bitbully as bb

            board = bb.Board()  # Empty 7x6 board

            # Every column index from 0 to 6 is a valid move.
            for col in range(7):
                assert board.is_legal_move(col)

            # Out-of-range indices are always illegal.
            assert not board.is_legal_move(-1)
            assert not board.is_legal_move(7)
            ```

        Example:
            Detecting an illegal move in a full column:
            ```python
            import bitbully as bb

            # Fill the center column (index 3) with six tokens.
            board = bb.Board()
            assert board.play([3, 3, 3, 3, 3, 3])

            # The center column is now full, so another move in column 3 is illegal.
            assert not board.is_legal_move(3)

            # Other columns are still available (as long as they are not full).
            assert board.is_legal_move(0)
            assert board.is_legal_move(6)

            print(board)
            ```
            Expected output:
            ```text
            _  _  _  O  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  _  _  X  _  _  _
            ```

        Example:
            This function only checks legality, not for situations where a player has won:
            ```python
            import bitbully as bb

            # Player 1 (yellow, X) wins  the game.
            board = bb.Board()
            assert board.play("1122334")

            # Even though Player 1 has already won, moves in non-full columns are still legal.
            for col in range(7):
                assert board.is_legal_move(col)

            print(board)
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  O  O  O  _  _  _
            _  X  X  X  X  _  _
            ```
        """
        return self._board.isLegalMove(move)

    def mirror(self) -> Board:
        """Returns a new Board instance that is the mirror image of the current board.

        This method reflects the board **horizontally** around its vertical center column:
        - Column 0 <-> Column 6
        - Column 1 <-> Column 5
        - Column 2 <-> Column 4
        - Column 3 stays in the center

        The player to move is not changed - only the spatial
        arrangement of the tokens is mirrored. The original board remains unchanged;
        `mirror()` always returns a **new** `Board` instance.

        Returns:
            Board: A new Board instance that is the mirror image of the current one.

        Example:
            Mirroring a simple asymmetric position:
            ```python
            import bitbully as bb

            # Play four moves along the bottom row.
            board = bb.Board()
            assert board.play("0123")  # Columns: 0, 1, 2, 3

            # Create a mirrored copy of the board.
            mirrored = board.mirror()

            print("Original:")
            print(board)

            print("Mirrored:")
            print(mirrored)
            ```

            Expected output:
            ```text
            Original:

            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            X  O  X  O  _  _  _

            Mirrored:

            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  O  X  O  X
            ```

        Example:
            Mirroring a position that is already symmetric:
            ```python
            import bitbully as bb

            # Central symmetry: one token in each outer column and in the center.
            board = bb.Board([1, 3, 5])

            mirrored = board.mirror()

            # The mirrored position is identical to the original.
            assert board == mirrored
            assert hash(board) == hash(mirrored)

            print(board)
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  X  _  O  _  X  _
            ```
        """
        new_board = Board()
        new_board._board = self._board.mirror()
        return new_board

    def moves_left(self) -> int:
        """Returns the number of moves left until the board is full.

        This is simply the number of *empty* cells remaining on the 7x6 grid.
        On an empty board there are 42 free cells, so:

        - At the start of the game: `moves_left() == 42`
        - After `n` valid moves: `moves_left() == 42 - n`
        - On a completely full board: `moves_left() == 0`

        This method is equivalent to:
        ```
        42 - board.count_tokens()
        ```
        but implemented efficiently in the underlying C++ core.

        Returns:
            int: The number of moves left (0-42).

        Example:
            Moves left on an empty board:
            ```python
            import bitbully as bb

            board = bb.Board()  # No tokens placed yet.
            assert board.moves_left() == 42
            assert board.count_tokens() == 0
            ```

        Example:
            Relation to the number of moves played:
            ```python
            import bitbully as bb

            # Play five moves in various columns.
            moves = [3, 3, 1, 4, 6]
            board = bb.Board()
            assert board.play(moves)

            # Five tokens have been placed, so 42 - 5 = 37 moves remain.
            assert board.count_tokens() == 5
            assert board.moves_left() == 37
            assert board.moves_left() + board.count_tokens() == 42
            ```
        """
        return self._board.movesLeft()

    def play(self, move: int | Sequence[int] | str) -> bool:
        """Plays one or more moves for the current player.

        The method updates the internal board state by dropping tokens
        into the specified columns. Input can be:
        - a single integer (column index 0 to 6),
        - an iterable sequence of integers (e.g., `[3, 1, 3]` or `range(7)`),
        - or a string of digits (e.g., `"33333111"`) representing the move order.

        Args:
            move (int | Sequence[int] | str):
                The column index or sequence of column indices where tokens should be placed.

        Returns:
            bool: True if the move was played successfully, False if the move was illegal.


        Example:
            Play a sequence of moves into the center column (column index 3):
            ```python
            import bitbully as bb

            board = bb.Board()
            assert board.play([3, 3, 3])  # returns True on successful move
            board
            ```

            Expected output:

            ```
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  _  _  X  _  _  _
            ```

        Example:
            Play a sequence of moves across all columns:
            ```python
            import bitbully as bb

            board = bb.Board()
            assert board.play(range(7))  # returns True on successful move
            board
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            X  O  X  O  X  O  X
            ```

        Example:
            Play a sequence using a string:
            ```python
            import bitbully as bb

            board = bb.Board()
            assert board.play("33333111")  # returns True on successful move
            board
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
        # Case 1: string -> pass through directly
        if isinstance(move, str):
            return self._board.play(move)

        # Case 2: int -> pass through directly
        if isinstance(move, int):
            return self._board.play(move)

        # From here on, move is a Sequence[...] (but not str or int).
        move_list: list[int] = [int(v) for v in cast(Sequence[Any], move)]
        return self._board.play(move_list)

    def play_on_copy(self, move: int) -> Board:
        """Return a new board with the given move applied, leaving the current board unchanged.

        Args:
            move (int):
                The column index (0-6) in which to play the move.

        Returns:
            Board:
                A new Board instance representing the position after the move.

        Raises:
            ValueError: If the move is illegal (e.g. column is full or out of range).

        Example:
            ```python
            import bitbully as bb

            board = bb.Board("333")  # Some existing position
            new_board = board.play_on_copy(4)

            # The original board is unchanged.
            assert board.count_tokens() == 3

            # The returned board includes the new move.
            assert new_board.count_tokens() == 4
            assert new_board != board
            ```
        """
        # Delegate to C++ (this returns a BoardCore instance)
        core_new = self._board.playMoveOnCopy(move)

        if core_new is None:
            # C++ signals illegal move by returning a null board
            raise ValueError(f"Illegal move: column {move}")

        # Wrap in a new high-level Board object
        new_board = Board()
        new_board._board = core_new
        return new_board

    def reset_board(self, board: Sequence[int] | Sequence[Sequence[int]] | str | None = None) -> bool:
        """Resets the board or sets (overrides) the board to a specific state.

        Args:
            board (Sequence[int] | Sequence[Sequence[int]] | str | None):
                The new board state. Accepts:
                - 2D array (list, tuple, numpy-array) with shape 7x6 or 6x7
                - 1D sequence of ints: a move sequence of columns (e.g., [0, 0, 2, 2, 3, 3])
                - String: A move sequence of columns as string (e.g., "002233...")
                - None: to reset to an empty board

        Returns:
            bool: True if the board was set successfully, False otherwise.

        Example:
            Reset the board to an empty state:
            ```python
            import bitbully as bb

            # Create a temporary board position from a move string.
            # The string "0123456" plays one token in each column (0-6) in sequence.
            board = bb.Board("0123456")

            # Reset the board to an empty state.
            # Calling `reset_board()` clears all tokens and restores the starting position.
            # No moves → an empty board.
            assert board.reset_board()
            board
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            ```

        Example:
            (Re-)Set the board using a move sequence string:
            ```python
            import bitbully as bb

            # This is just a temporary setup; it will be replaced below.
            board = bb.Board("0123456")

            # Set the board state directly from a move sequence.
            # The list [3, 3, 3] represents three consecutive moves in the center column (index 3).
            # Moves alternate automatically between Player 1 (yellow) and Player 2 (red).
            #
            # The `reset_board()` method clears the current position and replays the given moves
            # from an empty board — effectively overriding any existing board state.
            assert board.reset_board([3, 3, 3])

            # Display the updated board to verify the new position.
            board
            ```
            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  _  O  _  _  _
            _  _  _  X  _  _  _
            ```

        Example:
            You can also set the board using other formats, such as a 2D array or a string.
            See the examples in the [Board][src.bitbully.board.Board] docstring for details.

            ```python
            # Briefly demonstrate the different input formats accepted by `reset_board()`.
            import bitbully as bb

            # Create an empty board instance
            board = bb.Board()

            # Variant 1: From a list of moves (integers)
            # Each number represents a column index (0-6); moves alternate between players.
            assert board.reset_board([3, 3, 3])

            # Variant 2: From a compact move string
            # Equivalent to the list above — useful for quick testing or serialization.
            assert board.reset_board("33333111")

            # Variant 3: From a 2D list in row-major format (6 x 7)
            # Each inner list represents a row (top to bottom).
            # 0 = empty, 1 = Player 1, 2 = Player 2.
            board_array = [
                [0, 0, 0, 0, 0, 0, 0],  # Row 5 (top)
                [0, 0, 0, 1, 0, 0, 0],  # Row 4
                [0, 0, 0, 2, 0, 0, 0],  # Row 3
                [0, 2, 0, 1, 0, 0, 0],  # Row 2
                [0, 1, 0, 2, 0, 0, 0],  # Row 1
                [0, 2, 0, 1, 0, 0, 0],  # Row 0 (bottom)
            ]
            assert board.reset_board(board_array)

            # Variant 4: From a 2D list in column-major format (7 x 6)
            # Each inner list represents a column (left to right); this matches BitBully's internal layout.
            board_array = [
                [0, 0, 0, 0, 0, 0],  # Column 0 (leftmost)
                [2, 1, 2, 1, 0, 0],  # Column 1
                [0, 0, 0, 0, 0, 0],  # Column 2
                [1, 2, 1, 2, 1, 0],  # Column 3 (center)
                [0, 0, 0, 0, 0, 0],  # Column 4
                [2, 1, 2, 0, 0, 0],  # Column 5
                [0, 0, 0, 0, 0, 0],  # Column 6 (rightmost)
            ]
            assert board.reset_board(board_array)

            # Display the final board state in text form
            board
            ```

            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  X  _  O  _  _  _
            _  O  _  X  _  O  _
            _  X  _  O  _  X  _
            _  O  _  X  _  O  _
            ```
        """
        if board is None:
            return self._board.setBoard([])
        if isinstance(board, str):
            return self._board.setBoard(board)

        # From here on, board is a Sequence[...] (but not str).
        # Distinguish 2D vs 1D by inspecting the first element.
        if len(board) > 0 and isinstance(board[0], Sequence) and not isinstance(board[0], (str, bytes)):
            # Case 2: 2D -> list[list[int]]
            # Convert inner sequences to lists of ints
            grid: list[list[int]] = [[int(v) for v in row] for row in cast(Sequence[Sequence[Any]], board)]
            return self._board.setBoard(grid)

        # Case 3: 1D -> list[int]
        moves: list[int] = [int(v) for v in cast(Sequence[Any], board)]
        return self._board.setBoard(moves)

    def to_array(self, column_major_layout: bool = True) -> list[list[int]]:
        """Returns the board state as a 2D array (list of lists).

        This layout is convenient for printing, serialization, or converting
        to a NumPy array for further analysis.

        Args:
            column_major_layout (bool): Use column-major format if set to `True`,
                otherwise the row-major-layout is used.

        Returns:
            list[list[int]]: A 7x6 2D list representing the board state.

        Raises:
            NotImplementedError: If `column_major_layout` is set to `False`.

        Example:
            === "Column-major Format:"

                The returned array is in **column-major** format with shape `7 x 6`
                (`[column][row]`):

                - There are 7 inner lists, one for each column of the board.
                - Each inner list has 6 integers, one for each row.
                - Row index `0` corresponds to the **bottom row**,
                row index `5` to the **top row**.
                - Convention:
                - `0` -> empty cell
                - `1` -> Player 1 token (yellow, X)
                - `2` -> Player 2 token (red, O)

                ```python
                import bitbully as bb
                from pprint import pprint

                # Create a position from a move sequence.
                board = bb.Board("33333111")

                # Extract the board as a 2D list (rows x columns).
                arr = board.to_array()

                # Reconstruct the same position from the 2D array.
                board2 = bb.Board(arr)

                # Both boards represent the same position.
                assert board == board2
                assert board.to_array() == board2.to_array()

                # print ther result of `board.to_array()`:
                pprint(board.to_array())
                ```
                Expected output:
                ```text
                [[0, 0, 0, 0, 0, 0],
                [2, 1, 2, 0, 0, 0],
                [0, 0, 0, 0, 0, 0],
                [1, 2, 1, 2, 1, 0],
                [0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0]]
                ```

            === "Row-major Format:"

                ``` markdown
                TODO: This is not supported yet
                ```
        """
        if not column_major_layout:
            # TODO: Implement in C++
            raise NotImplementedError("Row-major Layout is yet to be implemented")

        return self._board.toArray()

    def to_string(self) -> str:
        """Returns a human-readable ASCII representation of the board.

        The returned string shows the **current board position** as a 6x7 grid,
        laid out exactly as it would appear when you print a `Board` instance:

        - 6 lines of text, one per row (top row first, bottom row last)
        - 7 entries per row, separated by two spaces
        - `_` represents an empty cell
        - `X` represents a token from Player 1 (yellow)
        - `O` represents a token from Player 2 (red)

        This is useful when you want to explicitly capture the board as a string
        (e.g., for logging, debugging, or embedding into error messages) instead
        of relying on `print(board)` or `repr(board)`.

        Returns:
            str: A multi-line ASCII string representing the board state.

        Example:
            Using `to_string()` on an empty board:
            ```python
            import bitbully as bb

            board = bb.Board("33333111")

            s = board.to_string()
            print(s)
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
        return self._board.toString()

    def uid(self) -> int:
        """Returns a unique identifier for the current board state.

        The UID is a deterministic integer computed from the internal bitboard
        representation of the position. It is **stable**, **position-based**, and
        uniquely tied to the exact token layout **and** the side to move.

        Key properties:

        - Boards with the **same** configuration (tokens + player to move) always
          produce the **same** UID.
        - Any change to the board (e.g., after a legal move) will almost always
          result in a **different** UID.
        - Copies of a board created via the copy constructor or `Board.copy()`
          naturally share the same UID as long as their states remain identical.

        Unlike `__hash__()`, the UID is not optimized for hash-table dispersion.
        For use in transposition tables, caching, or dictionary/set keys,
        prefer `__hash__()` since it provides a higher-quality hash distribution.

        Returns:
            int: A unique integer identifier for the board state.

        Example:
            UID is an integer and not None:
            ```python
            import bitbully as bb

            board = bb.Board()
            u = board.uid()

            assert isinstance(u, int)
            # Empty board has a well-defined, stable UID.
            assert board.uid() == u
            ```

        Example:
            UID changes when the position changes:
            ```python
            import bitbully as bb

            board = bb.Board()
            uid_before = board.uid()

            assert board.play(1)  # Make a move in column 1.

            uid_after = board.uid()
            assert uid_after != uid_before
            ```

        Example:
            Copies share the same UID while they are identical:
            ```python
            import bitbully as bb

            board = bb.Board("0123")

            # Create an independent copy of the same position.
            board_copy = board.copy()

            assert board is not board_copy  # Different objects
            assert board == board_copy  # Same position
            assert board.uid() == board_copy.uid()  # Same UID

            # After modifying the copy, they diverge.
            assert board_copy.play(4)
            assert board != board_copy
            assert board.uid() != board_copy.uid()
            ```

        Example:
            Different move sequences leading to the same position share the same UID:
            ```python
            import bitbully as bb

            board_1 = bb.Board("01234444")
            board_2 = bb.Board("44440123")

            assert board_1 is not board_2  # Different objects
            assert board_1 == board_2  # Same position
            assert board_1.uid() == board_2.uid()  # Same UID

            # After modifying the copy, they diverge.
            assert board_1.play(4)
            assert board_1 != board_2
            assert board_1.uid() != board_2.uid()
            ```
        """
        return self._board.uid()

    def current_player(self) -> int:
        """Returns the player whose turn it is to move.

        The current player is derived from the **parity** of the number of tokens
        on the board:

        - Player 1 (yellow, ``X``) moves first on an empty board.
        - After an even number of moves → it is Player 1's turn.
        - After an odd  number of moves → it is Player 2's turn.

        Returns:
            int:
                The player to move:

                - ``1`` → Player 1 (yellow, ``X``)
                - ``2`` → Player 2 (red, ``O``)

        Example:
            ```python
            import bitbully as bb

            # Empty board → Player 1 starts.
            board = bb.Board()
            assert board.current_player() == 1
            assert board.count_tokens() == 0

            # After one move, it's Player 2's turn.
            assert board.play(3)
            assert board.count_tokens() == 1
            assert board.current_player() == 2

            # After a second move, it's again Player 1's turn.
            assert board.play(4)
            assert board.count_tokens() == 2
            assert board.current_player() == 1
            ```
        """
        # Empty board: Player 1
        return 1 if self.count_tokens() % 2 == 0 else 2

    def is_full(self) -> bool:
        """Checks whether the board has any empty cells left.

        A Connect Four board has 42 cells in total (7 columns x 6 rows).
        This method returns ``True`` if **all** cells are occupied, i.e.
        when  [Board.moves_left][src.bitbully.board.Board.moves_left] returns ``0``.

        Returns:
            bool:
                ``True`` if the board is completely full
                (no more legal moves possible), otherwise ``False``.

        Example:
            ```python
            import bitbully as bb

            board = bb.Board()
            assert not board.is_full()
            assert board.moves_left() == 42
            assert board.count_tokens() == 0

            # Fill the board column by column.
            for _ in range(6):
                assert board.play("0123456")  # one token per column, per row

            # Now every cell is occupied.
            assert board.is_full()
            assert board.moves_left() == 0
            assert board.count_tokens() == 42
            ```
        """
        return self.moves_left() == 0

    def is_game_over(self) -> bool:
        """Checks whether the game has ended (win or draw).

        A game of Connect Four is considered **over** if:

        - One of the players has a winning position
          (see [Board.has_win][src.bitbully.board.Board.has_win]), **or**
        - The board is completely full and no further moves can be played
          (see [Board.is_full][src.bitbully.board.Board.is_full]).

        This method does **not** indicate *who* won; for that, use
        [Board.winner][src.bitbully.board.Board.winner].

        Returns:
            bool:
                ``True`` if the game is over (win or draw), otherwise ``False``.

        Example:
            Game over by a win:
            ```python
            import bitbully as bb

            # Player 1 (X) wins horizontally on the bottom row.
            board = bb.Board()
            assert board.play("0101010")

            assert board.has_win()
            assert board.is_game_over()
            assert board.winner() == 1
            ```

        Example:
            Game over by a draw (full board, no winner):
            ```python
            import bitbully as bb

            board, _ = bb.Board.random_board(42, forbid_direct_win=False)

            assert board.is_full()
            assert not board.has_win()
            assert board.is_game_over()
            assert board.winner() is None
            ```
        """
        return self.has_win() or self.is_full()

    def winner(self) -> int | None:
        """Returns the winning player, if the game has been won.

        This helper interprets the current board under the assumption that
        [Board.has_win][src.bitbully.board.Board.has_win] indicates **the last move** created a
        winning configuration. In that case, the winner is the *previous* player:

        - If it is currently Player 1's turn, then Player 2 must have just won.
        - If it is currently Player 2's turn, then Player 1 must have just won.

        If there is no winner (i.e. [Board.has_win][src.bitbully.board.Board.has_win] is ``False``),
        this method returns ``None``.

        Returns:
            int | None:
                The winning player, or ``None`` if there is no winner.

                - ``1`` → Player 1 (yellow, ``X``)
                - ``2`` → Player 2 (red, ``O``)
                - ``None`` → No winner (game still ongoing or draw)

        Example:
            Detecting a winner:
            ```python
            import bitbully as bb

            # Player 1 wins with a horizontal line at the bottom.
            board = bb.Board()
            assert board.play("1122334")

            assert board.has_win()
            assert board.is_game_over()

            # It is now Player 2's turn to move next...
            assert board.current_player() == 2

            # ...which implies Player 1 must be the winner.
            assert board.winner() == 1
            ```

        Example:
            No winner yet:
            ```python
            import bitbully as bb

            board = bb.Board()
            assert board.play("112233")  # no connect-four yet

            assert not board.has_win()
            assert not board.is_game_over()
            assert board.winner() is None
            ```
        """
        if not self.has_win():
            return None
        # Previous player = opposite of current_player
        return 2 if self.current_player() == 1 else 1

    @classmethod
    def from_moves(cls, moves: Sequence[int] | str) -> Board:
        """Creates a board by replaying a sequence of moves from the empty position.

        This is a convenience constructor around [Board.play][src.bitbully.board.Board.play].
        It starts from an empty board and applies the given move sequence, assuming
        it is **legal** (no out-of-range columns, no moves in full columns, etc.).

        Args:
            moves (Sequence[int] | str):
                The move sequence to replay from the starting position. Accepts:

                - A sequence of integers (e.g. ``[3, 3, 3, 1]``)
                - A string of digits (e.g. ``"3331"``)

                Each value represents a column index (0-6). Players alternate
                automatically between moves.

        Returns:
            Board:
                A new `Board` instance representing the final position
                after all moves have been applied.

        Example:
            ```python
            import bitbully as bb

            # Create a position directly from a compact move string.
            board = bb.Board.from_moves("33333111")

            # Equivalent to:
            # board = bb.Board()
            # assert board.play("33333111")

            print(board)
            assert board.count_tokens() == 8
            assert not board.has_win()
            ```
        """
        board = cls()
        assert board.play(moves)
        return board

    @classmethod
    def from_array(cls, arr: Sequence[Sequence[int]]) -> Board:
        """Creates a board directly from a 2D array representation.

        This is a convenience wrapper around the main constructor [board.Board][src.bitbully.board.Board]
        and accepts the same array formats:

        - **Row-major**: 6 x 7 (``[row][column]``), top row first.
        - **Column-major**: 7 x 6 (``[column][row]``), left column first.

        Values must follow the usual convention:

        - ``0`` → empty cell
        - ``1`` → Player 1 token (yellow, ``X``)
        - ``2`` → Player 2 token (red, ``O``)

        Args:
            arr (Sequence[Sequence[int]]):
                A 2D array describing the board state, either in row-major or
                column-major layout. See the examples in
                [Board][src.bitbully.board.Board] for details.

        Returns:
            Board:
                A new `Board` instance representing the given layout.

        Example:
            Using a 6 x 7 row-major layout:
            ```python
            import bitbully as bb

            board_array = [
                [0, 0, 0, 0, 0, 0, 0],  # Row 5 (top)
                [0, 0, 0, 1, 0, 0, 0],  # Row 4
                [0, 0, 0, 2, 0, 0, 0],  # Row 3
                [0, 2, 0, 1, 0, 0, 0],  # Row 2
                [0, 1, 0, 2, 0, 0, 0],  # Row 1
                [0, 2, 0, 1, 0, 0, 0],  # Row 0 (bottom)
            ]

            board = bb.Board.from_array(board_array)
            print(board)
            ```

        Example:
            Using a 7 x 6 column-major layout:
            ```python
            import bitbully as bb

            board_array = [
                [0, 0, 0, 0, 0, 0],  # Column 0
                [2, 1, 2, 1, 0, 0],  # Column 1
                [0, 0, 0, 0, 0, 0],  # Column 2
                [1, 2, 1, 2, 1, 0],  # Column 3
                [0, 0, 0, 0, 0, 0],  # Column 4
                [2, 1, 2, 0, 0, 0],  # Column 5
                [0, 0, 0, 0, 0, 0],  # Column 6
            ]

            board = bb.Board.from_array(board_array)

            # Round-trip via to_array:
            assert board.to_array() == board_array
            ```
        """
        return cls(arr)

    @staticmethod
    def random_board(n_ply: int, forbid_direct_win: bool) -> tuple[Board, list[int]]:
        """Generates a random board state by playing a specified number of random moves.

        If ``forbid_direct_win`` is ``True``, the generated position is guaranteed
        **not** to contain an immediate winning move for the player to move.

        Args:
            n_ply (int):
                Number of random moves to simulate (0-42).
            forbid_direct_win (bool):
                If ``True``, ensures the resulting board has **no immediate winning move**.

        Returns:
            tuple[Board, list[int]]:
                A pair ``(board, moves)`` where ``board`` is the generated position
                and ``moves`` are the exact random moves performed.

        Raises:
            ValueError: If `n_ply` is outside the valid range [0, 42].

        Example:
            Basic usage:
            ```python
            import bitbully as bb

            board, moves = bb.Board.random_board(10, forbid_direct_win=True)

            print("Moves:", moves)
            print("Board:")
            print(board)

            # The move list must match the requested ply.
            assert len(moves) == 10

            # No immediate winning move when forbid_direct_win=True.
            assert not board.can_win_next()
            ```

        Example:
            Using random boards in tests or simulations:
            ```python
            import bitbully as bb

            # Generate 50 random 10-ply positions.
            for _ in range(50):
                board, moves = bb.Board.random_board(10, forbid_direct_win=True)
                assert len(moves) == 10
                assert not board.has_win()  # Game should not be over
                assert board.count_tokens() == 10  # All generated boards contain exactly 10 tokens
                assert not board.can_win_next()  # Since `forbid_direct_win=True`, no immediate threat
            ```

        Example:
            Reconstructing the board manually from the move list:
            ```python
            import bitbully as bb

            b1, moves = bb.Board.random_board(8, forbid_direct_win=True)

            # Recreate the board using the move sequence:
            b2 = bb.Board(moves)

            assert b1 == b2
            assert b1.to_string() == b2.to_string()
            assert b1.uid() == b2.uid()
            ```

        Example:
            Ensure randomness by generating many distinct sequences:
            ```python
            import bitbully as bb

            seen = set()
            for _ in range(20):
                _, moves = bb.Board.random_board(5, False)
                seen.add(tuple(moves))

            # Very likely to see more than one unique sequence.
            assert len(seen) > 1
            ```
        """
        if not 0 <= n_ply <= 42:
            raise ValueError(f"n_ply must be between 0 and 42 (inclusive), got {n_ply}.")
        board_, moves = BoardCore.randomBoard(n_ply, forbid_direct_win)
        board = Board()
        board._board = board_

        return board, moves

    def to_huffman(self) -> int:
        """Encode the current board position into a Huffman-compressed byte sequence.

        This is a high-level wrapper around
        `bitbully_core.BoardCore.toHuffman`. The returned int encodes the
        exact token layout **and** the side to move using the same format as
        the BitBully opening databases.

        The encoding is:

        - Deterministic: the same position always yields the same byte sequence.
        - Compact: suitable for storage (of positions with little number of tokens),
          or lookups in the BitBully database format.

        Returns:
            int: A Huffman-compressed representation of the current board
            state.

        Raises:
            NotImplementedError:
                If the position does not contain exactly 8 or 12 tokens, as the
                  Huffman encoding is only defined for these cases.

        Example:
            Encode a position and verify that equivalent positions have the
            same Huffman code:

            ```python
            import bitbully as bb

            # Two different move sequences leading to the same final position.
            b1 = bb.Board("01234444")
            b2 = bb.Board("44440123")

            h1 = b1.to_huffman()
            h2 = b2.to_huffman()

            # Huffman encoding is purely position-based.
            assert h1 == h2

            print(f"Huffman code: {h1}")
            ```
            Expected output:
            ```text
            Huffman code: 10120112
            ```
        """
        token_count = self.count_tokens()
        if token_count != 8 and token_count != 12:
            raise NotImplementedError("to_huffman() is only implemented for positions with 8 or 12 tokens.")
        return self._board.toHuffman()

    def legal_moves(self, non_losing: bool = False, order_moves: bool = False) -> list[int]:
        """Returns a list of all legal moves (non-full columns) for the current board state.

        Args:
            non_losing (bool):
                If ``True``, only returns moves that do **not** allow the opponent
                to win immediately on their next turn. The list might be empty
                If ``False``, all legal moves are returned.
            order_moves (bool):
                If ``True``, the returned list is ordered to prioritize moves (potentially more promising first).

        Returns:
            list[int]: A list of column indices (0-6) where a token can be legally dropped.

        Example:
            ```python
            import bitbully as bb

            board = bb.Board()
            legal_moves = board.legal_moves()
            assert set(legal_moves) == set(range(7))  # All columns are initially legal
            assert set(legal_moves) == set(board.legal_moves(order_moves=True))
            board.legal_moves(order_moves=True) == [3, 2, 4, 1, 5, 0, 6]  # Center column prioritized
            ```

        Example:
            ```python
            import bitbully as bb

            board = bb.Board()
            board.play("3322314")
            print(board)
            assert board.legal_moves() == list(range(7))
            assert board.legal_moves(non_losing=True) == [5]
            ```

            Expected output:
            ```text
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  _  _  _  _
            _  _  _  X  _  _  _
            _  _  O  O  _  _  _
            _  O  X  X  X  _  _
            ```
        """
        return self._board.legalMoves(nonLosing=non_losing, orderMoves=order_moves)

    @property
    def native(self) -> BoardCore:
        """Return the underlying native board representation.

        This is intended for internal engine integrations and wrappers.
        Users should treat this as read-only.

        Returns:
            BoardCore:
                The underlying native `BoardCore` instance representing the board state.

        Notes:
        - The `native` property exposes the underlying engine representation.
        - This is intended for engine wrappers (e.g. BitBully) and should be
        treated as read-only by users.
        """
        return self._board

    def get_column_height(self, column: int) -> int:
        """Returns the height of a specific column on the board.

        The height of a column is defined as the number of tokens currently
        present in that column.

        Args:
            column (int): The column index (0-6) for which to retrieve the height.

        Returns:
            int: The height of the specified column (0-6).

        Raises:
            ValueError: If the `column` index is outside the valid range [0, 6].

        Example:
            ```python
            import bitbully as bb

            board = bb.Board("0011223")
            height = board.get_column_height(2)
            assert height == 2
            ```
        """
        if not (0 <= column <= 6):
            raise ValueError(f"Column index must be between 0 and 6, got {column}.")
        return self._board.getColumnHeight(column)

    def get_column_heights(self) -> list[int]:
        """Returns a list of heights for each column on the board.

        The height of a column is defined as the number of tokens currently
        present in that column.

        Returns:
            list[int]: A list of 7 integers, where each integer represents
            the height of the corresponding column (0-6).

        Example:
            ```python
            import bitbully as bb

            board = bb.Board("0011223")
            heights = board.get_column_heights()
            assert heights == [2, 2, 2, 1, 0, 0, 0]
            ```
        """
        return [self._board.getColumnHeight(col) for col in range(7)]
