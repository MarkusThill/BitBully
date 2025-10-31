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
                - String: A move sequence of columns as string (e.g., "002233...")
                - None for an empty board

        Example:
            You can initialize an empty board in multiple ways:
            ```python
            import bitbully as bb

            board = bb.Board()  # Empty board
            board = bb.Board([[0] * 6 for _ in range(7)])  # also an empty board as 2D list
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
            And here we have another example:
            ```python
            import bitbully as bb

            board = bb.Board("002233...")  # String
            ```
        """
        self._board = bitbully_core.BoardCore()
        if init_with is not None:
            self.set_board(init_with)

    def __eq__(self, value: object) -> bool:
        """Checks equality between two Board instances.

        Args:
            value (object): The other Board instance to compare against.

        Returns:
            bool: True if both boards are equal, False otherwise.

        Raises:
            NotImplementedError: If the other value is not a Board instance.
        """
        if not isinstance(value, Board):
            raise NotImplementedError("Can only compare with another Board instance.")
        return bool(self._board == value._board)

    def __ne__(self, value: object) -> bool:
        """Checks inequality between two Board instances.

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
        """
        return self._board.hasWin()

    def __hash__(self) -> int:
        """Returns a hash of the Board instance for use in hash-based collections.

        Returns:
            int: The hash value of the Board instance.
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

    def set_board(self, board: Sequence[int] | Sequence[Sequence[int]] | str) -> bool:
        """Sets (overrides) the board to a specific state.

        Args:
            board (Sequence[int] | Sequence[Sequence[int]] | str):
                The new board state. Accepts:
                - 2D array (list, tuple, numpy-array) with shape 7x6 or 6x7
                - 1D sequence of ints: a move sequence of columns (e.g., [0, 0, 2, 2, 3, 3])
                - String: A move sequence of columns as string (e.g., "002233...")

        Returns:
            bool: True if the board was set successfully, False otherwise.
        """
        # TODO: also allow other types for `board`, e.g., numpy arrays and convert to a list of lists
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
