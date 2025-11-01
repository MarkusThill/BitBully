"""This module defines the Board class for managing the state of a Connect Four game."""

from __future__ import annotations  # for forward references in type hints (Python 3.7+)

from collections.abc import Sequence
from typing import Any, cast

from bitbully import bitbully_core


class Board:
    """Represents the state of a Connect Four board. Mostly a thin wrapper around BoardCore."""

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
        self._board = bitbully_core.BoardCore()
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

        See the documentation for [`src.bitbully.Board.__eq__`][src.bitbully.Board.__eq__] for details.

        Args:
            value (object): The other Board instance to compare against.

        Returns:
            bool: True if both boards are not equal, False otherwise.
        """
        return not self.__eq__(value)

    def __repr__(self) -> str:
        """Returns a string representation of the Board instance."""
        return f"{self._board}"

    def all_positions(self, up_to_n_ply: int, exactly_n: bool) -> list[Board]:
        """Finds all positions on the board up to a certain ply.

        Args:
            up_to_n_ply (int): The maximum ply depth to search.
            exactly_n (bool): If True, only returns positions at exactly N ply.

        Returns:
            list[Board]: A list of Board instances representing all positions.
        """
        # TODO: Implement this method properly. Need to convert BoardCore instances to Board.
        # return self._board.all_positions(up_to_n_ply, exactly_n)
        return [Board()]

    def can_win_next(self, move: int | None = None) -> bool:
        """Checks if the current player can win in the next move.

        Args:
            move (int | None): Optional column to check for an immediate win. If None, checks all columns.

        Returns:
            bool: True if the current player can win next, False otherwise.

        See also: [`bitbully.Board.has_win`][src.bitbully.Board.has_win].

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

        Returns:
            Board: A new Board instance that is a copy of the current one.
        """
        new_board = Board()
        new_board._board = self._board.copy()
        return new_board

    def count_tokens(self) -> int:
        """Counts the total number of tokens on the board.

        Returns:
            int: The total number of tokens.
        """
        return self._board.countTokens()

    def get_legal_moves(self) -> list[int]:
        """Returns a list of legal moves (columns) that can be played.

        Returns:
            list[int]: A list of column indices (0-6) where a move can be played.

        Raises:
            NotImplementedError: If the method is not implemented yet.
        """
        raise NotImplementedError("get_legal_moves is not implemented yet.")

    def has_win(self) -> bool:
        """Checks if the current player has a winning position.

        Returns:
            bool: True if the current player has a winning position (4-in-a-row), False otherwise.

        Unlike `can_win_next()`, which checks whether the current player *could* win
        on their next move, the `has_win()` method determines whether a winning
        condition already exists on the board.
        This method is typically used right after a move to verify whether the game
        has been won.

        See also: [`bitbully.Board.can_win_next`][src.bitbully.Board.can_win_next].

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
            Board from above, expected output:
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
        """Checks if a move (column) is legal.

        Args:
            move (int): The column index (0-6) to check.

        Returns:
            bool: True if the move is legal, False otherwise.
        """
        return self._board.isLegalMove(move)

    def mirror(self) -> Board:
        """Returns a new Board instance that is the mirror image of the current board.

        Returns:
            Board: A new Board instance that is the mirror image.
        """
        new_board = Board()
        new_board._board = self._board.mirror()
        return new_board

    def moves_left(self) -> int:
        """Returns the number of moves left until the board is full.

        Returns:
            int: The number of moves left (0-42).
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
            See the examples in the [`bitbully.Board.__init__`][src.bitbully.Board.__init__] docstring for details.

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

    def to_array(self) -> list[list[int]]:
        """Returns the board state as a 2D array (list of lists).

        Returns:
            list[list[int]]: A 2D list representing the board state.
        """
        return self._board.toArray()

    def to_string(self) -> str:
        """Returns a string representation of the board to print on the command line.

        Returns:
            str: A string representing the board (e.g., "002233...").
        """
        return self._board.toString()

    def uid(self) -> int:
        """Returns a unique identifier for the current board state.

        Returns:
            int: A unique integer identifier for the board state.
        """
        return self._board.uid()

    @staticmethod
    def random_board(n_ply: int, forbid_direct_win: bool) -> None:
        """Generates a random board state by playing a specified number of random moves.

        Args:
            n_ply (int): The number of random moves to play on the board.
            forbid_direct_win (bool): If True, the board will have a state that would result in an immediate win.
        """
        bitbully_core.BoardCore.randomBoard(n_ply, forbid_direct_win)

    def reset(self) -> None:
        """Resets the board to an empty state."""
        self._board = bitbully_core.BoardCore()
