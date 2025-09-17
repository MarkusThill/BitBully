"""Test the opening book functionality."""

import importlib.resources

import bitbully.bitbully_core as bbc
import pytest


@pytest.fixture(scope="session")
def openingbook_8ply() -> bbc.OpeningBook:
    """Session-scoped fixture for the 8-ply OpeningBook without distances.

    Returns:
        bbc.OpeningBook: The 8-ply OpeningBook instance.
    """
    db_path = importlib.resources.files("bitbully").joinpath("assets/book_8ply.dat")
    return bbc.OpeningBook(db_path, is_8ply=True, with_distances=False)


@pytest.fixture(scope="session")
def openingbook_12ply() -> bbc.OpeningBook:
    """Session-scoped fixture for the 12-ply OpeningBook without distances.

    Returns:
        bbc.OpeningBook: The 12-ply OpeningBook instance.
    """
    db_path = importlib.resources.files("bitbully").joinpath("assets/book_12ply.dat")
    return bbc.OpeningBook(db_path, is_8ply=False, with_distances=False)


@pytest.fixture(scope="session")
def openingbook_12ply_dist() -> bbc.OpeningBook:
    """Session-scoped fixture for the 12-ply OpeningBook with distances.

    Returns:
        bbc.OpeningBook: The 12-ply OpeningBook instance with distances.
    """
    db_path = importlib.resources.files("bitbully").joinpath("assets/book_12ply_distances.dat")
    return bbc.OpeningBook(db_path, is_8ply=False, with_distances=True)


@pytest.mark.parametrize("openingbook_fixture", ["openingbook_8ply", "openingbook_12ply", "openingbook_12ply_dist"])
def test_book_keys_are_sorted(request: pytest.FixtureRequest, openingbook_fixture: str) -> None:
    """Test that the keys (k) in the OpeningBook are strictly sorted in ascending order.

    Keys are interpreted as signed 32-bit integers.

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBook fixture to use.
    """
    ob = request.getfixturevalue(openingbook_fixture)
    last_key = float("-inf")
    for i in range(ob.getBookSize()):
        k, _ = ob.getEntry(i)
        assert last_key < k, f"Book key at index {i} is not greater than previous key: {last_key} >= {k}"
        last_key = k


@pytest.mark.parametrize(
    ("index", "expected"),
    [
        (0, (351484, 0)),
        (10, (614328, -1)),
        (100, (1244624, -1)),
        (1000, (2612040, 0)),
        (10000, (6958064, 0)),
        (34515 - 1, (16667232, 0)),  # ob.getBookSize() == 34515
    ],
)
def test_get_entry_valid_8ply(openingbook_8ply: bbc.OpeningBook, index: int, expected: tuple[int, int]) -> None:
    """Test that entries in the 8-ply OpeningBook at specific indices match the expected values.

    Args:
        openingbook_8ply (bbc.OpeningBook): The 8-ply OpeningBook instance.
        index (int): The index to test.
        expected (tuple[int, int]): The expected entry value.
    """
    entry = openingbook_8ply.getEntry(index)
    assert isinstance(entry, tuple)
    assert entry == expected


@pytest.mark.parametrize(
    ("index", "expected"),
    [
        (0, (-2124988676, 75)),
        (10, (-2124951620, 75)),
        (100, (-2122462468, -78)),
        (1000, (-2101449796, -72)),
        (10000, (-2055999688, 75)),
        (100000, (-1912785736, -92)),
        (1000000, (-1344544216, -72)),
        (2000000, (-571861640, 95)),
        (4000000, (1976257724, 73)),
        (4200899 - 1, (2138808968, 97)),
    ],
)
def test_get_entry_valid_12ply(openingbook_12ply_dist: bbc.OpeningBook, index: int, expected: tuple[int, int]) -> None:
    """Test that entries in the 12-ply OpeningBook with distances at specific indices match the expected values.

    Args:
        openingbook_12ply_dist (bbc.OpeningBook): The 12-ply OpeningBook instance with distances.
        index (int): The index to test.
        expected (tuple[int, int]): The expected entry value.
    """
    entry = openingbook_12ply_dist.getEntry(index)
    assert isinstance(entry, tuple)
    assert entry == expected


@pytest.mark.parametrize("openingbook_fixture", ["openingbook_8ply", "openingbook_12ply", "openingbook_12ply_dist"])
def test_get_entry_invalid(request: pytest.FixtureRequest, openingbook_fixture: str) -> None:
    """Test that an exception is raised for invalid indices (for all OpeningBooks).

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBook fixture to use.
    """
    openingbook = request.getfixturevalue(openingbook_fixture)
    with pytest.raises(TypeError):
        openingbook.getEntry(-1)
    with pytest.raises(IndexError):
        openingbook.getEntry(openingbook.getBookSize() + 1)


def test_get_book_size(openingbook_8ply: bbc.OpeningBook) -> None:
    """Test that the size of the 8-ply OpeningBook is correct.

    Args:
        openingbook_8ply (bbc.OpeningBook): The 8-ply OpeningBook instance.
    """
    size = openingbook_8ply.getBookSize()
    assert size == 34515


def test_get_book_returns_list(openingbook_8ply: bbc.OpeningBook) -> None:
    """Test that getBook() returns a list of the expected size.

    Args:
        openingbook_8ply (bbc.OpeningBook): The 8-ply OpeningBook instance.
    """
    book = openingbook_8ply.getBook()
    assert isinstance(book, list)
    assert len(book) == openingbook_8ply.getBookSize()


def test_get_board_value_known_position(openingbook_8ply: bbc.OpeningBook) -> None:
    """Test that the correct value is returned for a known position in the 8-ply OpeningBook.

    Args:
        openingbook_8ply (bbc.OpeningBook): The 8-ply OpeningBook instance.
    """
    move_list, expected_value = [2, 3, 3, 3, 3, 3, 5, 5], 0  # A known position in the book
    board = bbc.Board()
    board.setBoard(move_list)
    val = openingbook_8ply.getBoardValue(board)
    assert val == expected_value


def test_is_in_book(openingbook_8ply: bbc.OpeningBook) -> None:
    """Test that a known position or its mirrored variant is contained in the 8-ply OpeningBook.

    Args:
        openingbook_8ply (bbc.OpeningBook): The 8-ply OpeningBook instance.
    """
    # Only winning positions for RED are in the opening-book
    board = bbc.Board()
    board.setBoard([2, 3, 3, 3, 3, 3, 5, 5])  # A known position in the book
    # The board or its mirrored variant should be in the book
    assert openingbook_8ply.isInBook(board) or openingbook_8ply.isInBook(board.mirror())


def test_convert_value(openingbook_8ply: bbc.OpeningBook) -> None:
    """Test that convertValue() correctly converts the value for a random board.

    Args:
        openingbook_8ply (bbc.OpeningBook): The 8-ply OpeningBook instance.
    """
    board, _ = bbc.Board.randomBoard(8, True)
    v = openingbook_8ply.getBoardValue(board)
    converted = openingbook_8ply.convertValue(v, board)
    assert converted == v


def test_read_book_static_8ply() -> None:
    """Test static reading of the 8-ply OpeningBook and check the size."""
    import importlib.resources

    db_path = importlib.resources.files("bitbully").joinpath("assets/book_8ply.dat")
    book = bbc.OpeningBook.readBook(db_path, with_distances=False, is_8ply=True)
    assert isinstance(book, list)
    assert len(book) == 34515


@pytest.mark.parametrize(
    ("openingbook_fixture", "expected_nply"),
    [
        ("openingbook_8ply", 8),
        ("openingbook_12ply", 12),
        ("openingbook_12ply_dist", 12),
    ],
)
def test_get_n_ply(request: pytest.FixtureRequest, openingbook_fixture: str, expected_nply: int) -> None:
    """Test that getNPly() returns the correct ply value for different OpeningBooks.

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBook fixture to use.
        expected_nply (int): The expected ply value.
    """
    openingbook = request.getfixturevalue(openingbook_fixture)
    assert openingbook.getNPly() == expected_nply


def test_bitbully_12_ply_with_distance() -> None:
    """Validate that BitBully correctly scores an empty Connect-4 board.

    This test loads the precomputed distance database
    `book_12ply_distances.dat`, creates an empty `Board`, and verifies
    that `BitBully.scoreMoves` returns the expected heuristic scores
    for each of the seven columns.
    """
    db_path = importlib.resources.files("bitbully").joinpath("assets/book_12ply_distances.dat")
    bitbully: bbc.BitBully = bbc.BitBully(db_path)
    b: bbc.Board = bbc.Board()  # Empty board
    assert bitbully.scoreMoves(b) == [
        -2,
        -1,
        0,
        1,
        0,
        -1,
        -2,
    ], "expected result: [-2, -1, 0, 1, 0, -1, -2]"
