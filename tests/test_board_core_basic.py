"""Test basic board functionality."""

import pytest

import bitbully.bitbully_core as bbc


def test_empty_board_printable() -> None:
    """Verify that converting an empty Board to a string produces a non-empty, human-readable representation.

    This ensures that the Board class implements `__str__` correctly and
    does not return an empty string when no moves have been made.
    """
    b: bbc.BoardCore = bbc.BoardCore()
    s: str = str(b)
    assert isinstance(s, str)
    assert s != "", "Printing an empty board should return a non-empty string"


def test_set_board() -> None:
    """Validate that a 7x6 array can be set on a Board instance.

    A single yellow token is placed in the center column, and
    `Board.setBoard` is expected to accept this valid configuration.
    """
    arr = [[0 for _ in range(6)] for _ in range(7)]
    arr[3][0] = 1  # Add a yellow token in the center column
    b: bbc.BoardCore = bbc.BoardCore()
    assert b.setBoard(arr), "Board.setBoard should accept a valid 7x6 list of lists"


def test_all_positions() -> None:
    """Verify the correct number of positions with a specified ply depth.

    For a 3-ply depth, the number of possible positions starting from an empty
    board should be exactly 238, as documented in
    https://oeis.org/A212693.
    """
    b: bbc.BoardCore = bbc.BoardCore()  # Empty board
    board_list_3ply: list[bbc.BoardCore] = b.allPositions(3, True)
    assert len(board_list_3ply) == 238, "Expected 238 positions for 3-ply search according to https://oeis.org/A212693"


def test_random_board_generation() -> None:
    """Test that `Board.randomBoard` generates a valid random board and move sequence.

    Ensures:
        * The returned `moves` is a list.
        * The board's string representation is non-empty.
        * The generated move list has the requested length (10 moves).

    """
    for _ in range(100):  # crreates 100 random boards to check for flakiness
        b: bbc.BoardCore
        moves: list[int]
        b, moves = bbc.BoardCore.randomBoard(10, True)
        assert isinstance(moves, list), "Moves should be returned as a list"
        assert isinstance(str(b), str), "Board should be convertible to a non-empty string"
        assert len(moves) == 10, "Generated move list should match requested length"
        assert b.canWin() is False, "Random board should not have an immediate winning move"


def test_can_win_1_basic() -> None:
    """Test that canWin method works."""
    b: bbc.BoardCore = bbc.BoardCore()
    assert not b.canWin(3), "Empty board should not have a winning move"
    b.play(3)  # Yellow
    b.play(2)  # Red
    b.play(3)  # Yellow
    b.play(2)  # Red
    b.play(3)  # Yellow
    b.play(4)  # Red
    print(b)
    assert b.canWin(3), "Yellow should be able to win by playing in column 3"


@pytest.mark.parametrize(
    ("move_sequence", "expected_win"),
    [
        ([3, 3, 3, 3, 2, 2, 4], False),
        ([3, 3, 3, 3, 2, 2, 4, 4], True),
        ([3, 3, 3, 4, 4, 2, 4, 2, 2], False),
        ([3, 3, 3, 4, 4, 2, 4, 2, 2, 1, 1, 4], True),
    ],
)
def test_can_win_1(move_sequence: list[int], expected_win: bool) -> None:
    """Test the canWin method for different board states and expected outcomes.

    Args:
        move_sequence (list[int]): The sequence of moves to set up the board.
        expected_win (bool): The expected result of canWin(3) for the given board.
    """
    b: bbc.BoardCore = bbc.BoardCore()
    assert b.setBoard(move_sequence)
    result: bool = b.canWin()
    assert result == expected_win, f"Expected: {expected_win}, got: {result}, for \n{b}"


@pytest.mark.parametrize(
    ("move_sequence", "expected_win_column", "expected_win"),
    [
        ([3, 3, 3, 3, 2, 2, 4, 4], 1, True),
        ([3, 3, 3, 3, 2, 2, 4, 4], 2, False),
        ([3, 3, 3, 3, 2, 2, 4, 4], 5, True),
        ([3, 3, 3, 3, 2, 2, 4, 4], 3, False),
        ([3, 3, 3, 4, 4, 2, 4, 2, 2, 1, 1, 4], 1, True),
        ([3, 3, 3, 4, 4, 2, 4, 2, 2, 1, 1], 1, False),
        ([3, 3, 2, 2, 3, 4, 3, 4, 5], 5, True),
        ([3, 3, 2, 2, 3, 4, 3, 4, 5], 1, False),
    ],
)
def test_can_win_2(move_sequence: list[int], expected_win_column: int, expected_win: bool) -> None:
    """Test the canWin method for a specific column and board state.

    Args:
        move_sequence (list[int]): The sequence of moves to set up the board.
        expected_win_column (int): The column to test for a winning move.
        expected_win (bool): The expected result of canWin(expected_win_column) for the given board.
    """
    b: bbc.BoardCore = bbc.BoardCore()
    assert b.setBoard(move_sequence)
    assert b.canWin(expected_win_column) == expected_win, f"Expected win column for: {expected_win_column}, for \n{b}"


def test_hash() -> None:
    """Test that the hashing function produces consistent results."""
    b1: bbc.BoardCore = bbc.BoardCore()
    b2: bbc.BoardCore = bbc.BoardCore()
    moves = [3, 2, 3, 2, 3, 4]
    for move in moves:
        assert b1.play(move)
        assert b2.play(move)
        assert b1.hash() == b2.hash(), "Hashes should be identical for identical board states"
    b1.play(1)
    assert b1.hash() != b2.hash(), "Hashes should differ after different moves"


def test_hash_empty() -> None:
    """Test that the hash of an empty board is consistent."""
    b1: bbc.BoardCore = bbc.BoardCore()
    b2: bbc.BoardCore = bbc.BoardCore()
    assert b1.hash() == b2.hash(), "Hashes of two empty boards should be identical"


def test_hash_reversed_moves() -> None:
    """Test that the hash function produces the same result for boards with moves played in mirrored order.

    This ensures that the hash is symmetric for mirrored move sequences.
    """
    b1: bbc.BoardCore = bbc.BoardCore()
    b2: bbc.BoardCore = bbc.BoardCore()
    moves = [0, 1, 2, 3, 3, 2, 1, 0]
    for m1, m2 in zip(moves, reversed(moves)):
        assert b1.play(m1)
        assert b2.play(m2)

    assert b1.hash() == b2.hash(), (
        f"Expected identical hashes for mirrored board states, but got:\n"
        f"Board 1: {b1}\nBoard 2: {b2}\n"
        f"Hash 1: {b1.hash()}\nHash 2: {b2.hash()}"
    )


def test_hash_diff_big() -> None:
    """Test that the hash function produces very different results for only slightly varying board states.

    This ensures that the hash function is sensitive to changes in the board state.
    """
    b1: bbc.BoardCore = bbc.BoardCore()
    b2: bbc.BoardCore = bbc.BoardCore()
    moves1 = [0, 1, 2, 3, 3, 2, 1, 0, 4, 5, 6]
    moves2 = [0, 1, 2, 3, 3, 2, 1, 0, 4, 5, 5]  # Last move is different
    assert b1.setBoard(moves1)
    assert b2.setBoard(moves2)

    assert abs(b1.hash() - b2.hash()) > 10000, (
        f"Expected different hashes for different board states, but got:\n"
        f"Board 1: {b1}\nBoard 2: {b2}\n"
        f"Hash 1: {b1.hash()}\nHash 2: {b2.hash()}"
    )


def test_has_win() -> None:
    """Test that hasWin() correctly detects when the last player has four in a row.

    The test sets up a board where Yellow wins with four in a row in column 3.
    """
    b: bbc.BoardCore = bbc.BoardCore()
    assert not b.hasWin(), "hasWin() should return False for an empty board (no player has 4 in a row)"
    assert b.play(3)  # Yellow
    assert b.play(2)  # Red
    assert b.play(3)  # Yellow
    assert b.play(2)  # Red
    assert b.play(3)  # Yellow
    assert b.play(4)  # Red
    assert b.play(3)  # Yellow
    assert b.hasWin(), f"hasWin() should return True when the last player to move has 4 in a row. Board state:\n{b}"


@pytest.mark.parametrize(
    ("move_sequence", "expected_win_column", "expected_win"),
    [
        ([3, 3, 3, 3, 2, 2, 4, 4], 1, True),
        ([3, 3, 3, 3, 2, 2, 4, 4], 2, False),
        ([3, 3, 3, 3, 2, 2, 4, 4], 5, True),
        ([3, 3, 3, 3, 2, 2, 4, 4], 3, False),
        ([3, 3, 3, 4, 4, 2, 4, 2, 2, 1, 1, 4], 1, True),
        ([3, 3, 3, 4, 4, 2, 4, 2, 2, 1, 1], 1, False),
        ([3, 3, 2, 2, 3, 4, 3, 4, 5], 5, True),
        ([3, 3, 2, 2, 3, 4, 3, 4, 5], 1, False),
    ],
)
def test_has_win_param(move_sequence: list[int], expected_win_column: int, expected_win: bool) -> None:
    """Parameterized test for hasWin() after playing a move in a specific column.

    Args:
        move_sequence (list[int]): The sequence of moves to set up the board.
        expected_win_column (int): The column to test for a winning move.
        expected_win (bool): The expected result of hasWin() after playing in expected_win_column.
    """
    b: bbc.BoardCore = bbc.BoardCore()
    assert b.setBoard(move_sequence)
    assert b.play(expected_win_column)
    result: bool = b.hasWin()
    assert result == expected_win, (
        f"hasWin() returned {result} after playing column {expected_win_column}, expected {expected_win}.\nBoard:\n{b}"
    )


def test_play_move_invalid() -> None:
    """Test that playMove correctly handles invalid moves."""
    b: bbc.BoardCore = bbc.BoardCore()
    assert not b.play(-1), "playMove should return False for invalid negative column"
    assert not b.play(7), "playMove should return False for invalid column greater than 6"
    for _ in range(6):
        assert b.play(0), "playMove should succeed for valid moves in column 0"
    assert not b.play(0), "playMove should return False when trying to play in a full column"


def test_play_move_valid() -> None:
    """Test that playMove correctly handles valid moves.

    Note that playMove does not test for winning positions, only for valid column and space availability.
    """
    b: bbc.BoardCore = bbc.BoardCore()
    for col in range(7):
        for _ in range(6):
            assert b.play(col), f"playMove should succeed for valid move in column {col}"
    for col in range(7):
        assert not b.play(col), "playMove should return False when trying to play in a full column"


def test_is_legal_move() -> None:
    """IsLegalMove should return True for valid moves and False for invalid or full columns."""
    b: bbc.BoardCore = bbc.BoardCore()
    # Test valid columns on empty board
    for col in range(7):
        assert b.isLegalMove(col), f"Column {col} should be legal on an empty board"
    # Test invalid columns
    assert not b.isLegalMove(-1), "Negative column should not be legal"
    assert not b.isLegalMove(7), "Column greater than 6 should not be legal"
    # Fill up column 0
    for _ in range(6):
        assert b.play(0)
    assert not b.isLegalMove(0), "Column 0 should not be legal when full"


def test_moves_left() -> None:
    """MovesLeft should return the correct number of moves left on the board."""
    b: bbc.BoardCore = bbc.BoardCore()
    # Empty board: 42 moves left (6 rows x 7 columns)
    assert b.movesLeft() == 42, f"Expected 42 moves left on empty board, got {b.movesLeft()}"
    # Play one move
    b.play(0)
    assert b.movesLeft() == 41, f"Expected 41 moves left after one move, got {b.movesLeft()}"
    # Fill up column 0
    for _ in range(5):
        b.play(0)
    assert b.movesLeft() == 36, f"Expected 36 moves left after filling column 0, got {b.movesLeft()}"
    # Fill up the rest of the board
    for col in range(1, 7):
        for _ in range(6):
            b.play(col)
    assert b.movesLeft() == 0, f"Expected 0 moves left on full board, got {b.movesLeft()}"


def test_count_tokens() -> None:
    """CountTokens should return the correct number of tokens on the board."""
    b: bbc.BoardCore = bbc.BoardCore()
    assert b.countTokens() == 0, f"Expected 0 tokens on empty board, got {b.countTokens()}"
    b.play(0)
    assert b.countTokens() == 1, f"Expected 1 token after one move, got {b.countTokens()}"
    b.play(1)
    b.play(2)
    assert b.countTokens() == 3, f"Expected 3 tokens after three moves, got {b.countTokens()}"
    # Fill up column 0
    for _ in range(5):
        b.play(0)
    assert b.countTokens() == 8, f"Expected 8 tokens after filling column 0, got {b.countTokens()}"


def test_mirror() -> None:
    """Mirror should return a board mirrored around the center column."""
    b: bbc.BoardCore = bbc.BoardCore()
    b_mirror_expected: bbc.BoardCore = bbc.BoardCore()
    # Play moves in columns 0, 1, 2
    b.setBoard([0, 1, 2])
    # The mirrored board should have moves in columns 6, 5, 4
    b_mirror_expected.setBoard([6, 5, 4])

    mirrored = b.mirror()
    assert isinstance(mirrored, bbc.BoardCore)
    # The mirrored board should not be equal to the original
    assert mirrored != b, (
        f"Mirrored board should differ from original if not symmetric. Got:\n{mirrored}\nOriginal:\n{b}"
    )
    # The mirrored board should have the same number of tokens
    assert mirrored.countTokens() == b.countTokens(), (
        f"Mirrored board should have same number of tokens. Got:\n{mirrored.countTokens()} vs {b.countTokens()}"
    )

    # The mirrored board should match the expected mirrored configuration
    assert mirrored == b_mirror_expected, f"Expected mirrored board:\n{b_mirror_expected}\nGot:\n{mirrored}"

    # If we mirror twice, we should get the original board back
    double_mirrored = mirrored.mirror()
    assert double_mirrored == b, f"Double mirroring should return the original board. Got:\n{double_mirrored}"


def test_mirror_symmetric() -> None:
    """Mirror should return the same board if it is symmetric around the center column."""
    b: bbc.BoardCore = bbc.BoardCore()
    # Play moves in columns 2, 3, 4
    b.setBoard([2, 3, 4])
    mirrored = b.mirror()
    assert mirrored == b, f"Mirrored board should equal original for symmetric board. Got:\n{mirrored}\nOriginal:\n{b}"
    assert mirrored.countTokens() == b.countTokens(), (
        f"Mirrored board should have same number of tokens. Got:\n{mirrored.countTokens()} vs {b.countTokens()}"
    )
    double_mirrored = mirrored.mirror()
    assert double_mirrored == b, f"Double mirroring should return the original board. Got:\n{double_mirrored}"


def test_mirror_empty() -> None:
    """Mirror should return the same empty board."""
    b: bbc.BoardCore = bbc.BoardCore()
    mirrored = b.mirror()
    assert mirrored == b, f"Mirrored empty board should equal original. Got:\n{mirrored}\nOriginal:\n{b}"
    assert mirrored.countTokens() == b.countTokens(), (
        f"Mirrored board should have same number of tokens. Got:\n{mirrored.countTokens()} vs {b.countTokens()}"
    )
    double_mirrored = mirrored.mirror()
    assert double_mirrored == b, f"Double mirroring should return the original board. Got:\n{double_mirrored}"


def test_equality() -> None:
    """Test the equality operator for Board instances."""
    b1: bbc.BoardCore = bbc.BoardCore()
    b2: bbc.BoardCore = bbc.BoardCore()
    assert b1 == b2, "Two empty boards should be equal"
    moves = [0, 1, 2, 3]
    for move in moves:
        b1.play(move)
        b2.play(move)
    assert b1 == b2, "Boards with identical move sequences should be equal"
    b2.play(4)
    assert b1 != b2, "Boards with different move sequences should not be equal"
    b3: bbc.BoardCore = bbc.BoardCore()
    assert b1 != b3, "Non-empty board should not equal empty board"


def test_to_array() -> None:
    """Test the toArray method for correct board representation."""
    b: bbc.BoardCore = bbc.BoardCore()
    expected_empty = [[0 for _ in range(6)] for _ in range(7)]
    assert b.toArray() == expected_empty, "Empty board should convert to a 7x6 array of zeros"

    moves = [0, 1, 2, 3, 3, 2, 1, 0]
    b.setBoard(moves)
    expected_filled = [[0 for _ in range(6)] for _ in range(7)]
    expected_filled[0][0] = 1  # Yellow
    expected_filled[1][0] = 2  # Red
    expected_filled[2][0] = 1  # Yellow
    expected_filled[3][0] = 2  # Red
    expected_filled[3][1] = 1  # Yellow
    expected_filled[2][1] = 2  # Red
    expected_filled[1][1] = 1  # Yellow
    expected_filled[0][1] = 2  # Red

    assert b.toArray() == expected_filled, f"Board should convert to correct array representation:\n{b}"


def test_set_board_array() -> None:
    """Test the setBoard method with a valid 7x6 array."""
    board_list = [[0 for _ in range(6)] for _ in range(7)]
    board_list[0][0] = 1  # Yellow
    board_list[1][0] = 2  # Red
    board_list[2][0] = 1  # Yellow
    board_list[3][0] = 2  # Red
    board_list[3][1] = 1  # Yellow
    board_list[2][1] = 2  # Red
    board_list[1][1] = 1  # Yellow
    board_list[0][1] = 2  # Red

    b: bbc.BoardCore = bbc.BoardCore()
    assert b.setBoard(board_list), "setBoard should accept a valid 7x6 list of lists"

    expected_moves = [0, 1, 2, 3, 3, 2, 1, 0]
    b_expected: bbc.BoardCore = bbc.BoardCore()
    assert b_expected.setBoard(expected_moves), "setBoard should accept a valid move list"

    assert b == b_expected, (
        f"Board set from list of lists should match board set from moves. Got:\n{b}\nExpected:\n{b_expected}"
    )


def test_set_board_array_invalid() -> None:
    """Test the setBoard method with invalid arrays."""
    b: bbc.BoardCore = bbc.BoardCore()
    invalid_board_1 = [[0 for _ in range(5)] for _ in range(7)]  # Too few rows
    invalid_board_2 = [[0 for _ in range(6)] for _ in range(6)]  # Too few columns
    invalid_board_3 = [[0 for _ in range(6)] for _ in range(8)]  # Too many rows
    invalid_board_4 = [[0 for _ in range(8)] for _ in range(6)]  # Too many columns
    invalid_board_5 = [[0 for _ in range(6)] for _ in range(7)]
    invalid_board_5[0][0] = 3  # Invalid token value

    with pytest.raises(TypeError):
        b.setBoard(invalid_board_1)
    with pytest.raises(TypeError):
        b.setBoard(invalid_board_2)
    with pytest.raises(TypeError):
        b.setBoard(invalid_board_3)
    with pytest.raises(TypeError):
        b.setBoard(invalid_board_4)

    assert not b.setBoard(invalid_board_5)


def test_uid() -> None:
    """Test the uid method for unique board identification."""
    b: bbc.BoardCore = bbc.BoardCore()
    assert b.uid() is not None, "UID should not be None"
    assert isinstance(b.uid(), int), "UID should be an integer"

    b1: bbc.BoardCore = bbc.BoardCore(b)

    assert b.play(1)
    assert b.uid() != 0, "UID should change after a move is played"

    assert b != b1, "Boards with different states should not be equal"


def test_copy_constructor() -> None:
    """Test the copy constructor of the Board class."""
    b1: bbc.BoardCore = bbc.BoardCore()
    moves = [0, 1, 2, 3]
    for move in moves:
        b1.play(move)

    b2: bbc.BoardCore = bbc.BoardCore(b1)  # Use copy constructor
    assert id(b1) != id(b2), "Copy should have a different memory address than the original"
    assert b1 == b2, "Board created with copy constructor should be equal to the original"
    assert b1.hash() == b2.hash(), "Hashes should be identical for boards created with copy constructor"
    assert b1.uid() == b2.uid(), "UIDs should be identical for boards created with copy constructor"

    b2.play(4)
    assert id(b1) != id(b2), "Copy should have a different memory address than the original"
    assert b1 != b2, "Boards should not be equal after modifying the copy"
    assert b1.hash() != b2.hash(), "Hashes should differ after modifying the copy"
    assert b2.movesLeft() == b1.movesLeft() - 1, "Moves left should decrease after modifying the copy"
    assert b2.countTokens() == b1.countTokens() + 1, "Token count should increase after modifying the copy"


def test_copy() -> None:
    """Test the copy constructor of the Board class."""
    b1: bbc.BoardCore = bbc.BoardCore()
    moves = [0, 1, 2, 3]
    for move in moves:
        b1.play(move)

    b2: bbc.BoardCore = b1.copy()
    assert id(b1) != id(b2), "Copy should have a different memory address than the original"

    assert b1 == b2, "Board created with copy constructor should be equal to the original"
    assert b1.hash() == b2.hash(), "Hashes should be identical for boards created with copy()"
    assert b1.uid() == b2.uid(), "UIDs should be identical for boards created with copy()"

    b2.play(4)
    assert id(b1) != id(b2), "Copy should have a different memory address than the original"
    assert b1 != b2, "Boards should not be equal after modifying the copy"
    assert b1.hash() != b2.hash(), "Hashes should differ after modifying the copy"
    assert b2.movesLeft() == b1.movesLeft() - 1, "Moves left should decrease after modifying the copy"
    assert b2.countTokens() == b1.countTokens() + 1, "Token count should increase after modifying the copy"
