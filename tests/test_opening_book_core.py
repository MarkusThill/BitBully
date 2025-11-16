"""Test the opening book functionality."""

import bitbully_databases as bbd
import pytest

import bitbully.bitbully_core as bbc


@pytest.fixture(scope="session")
def openingbook_8ply() -> bbc.OpeningBookCore:
    """Session-scoped fixture for the 8-ply OpeningBookCore without distances.

    Returns:
        bbc.OpeningBookCore: The 8-ply OpeningBookCore instance.
    """
    db_path = bbd.BitBullyDatabases.get_database_path("8-ply")
    return bbc.OpeningBookCore(db_path, is_8ply=True, with_distances=False)


@pytest.fixture(scope="session")
def openingbook_12ply() -> bbc.OpeningBookCore:
    """Session-scoped fixture for the 12-ply OpeningBookCore without distances.

    Returns:
        bbc.OpeningBookCore: The 12-ply OpeningBookCore instance.
    """
    db_path = bbd.BitBullyDatabases.get_database_path("12-ply")
    return bbc.OpeningBookCore(db_path, is_8ply=False, with_distances=False)


@pytest.fixture(scope="session")
def openingbook_12ply_dist() -> bbc.OpeningBookCore:
    """Session-scoped fixture for the 12-ply OpeningBookCore with distances.

    Returns:
        bbc.OpeningBookCore: The 12-ply OpeningBookCore instance with distances.
    """
    db_path = bbd.BitBullyDatabases.get_database_path("12-ply-dist")
    return bbc.OpeningBookCore(db_path, is_8ply=False, with_distances=True)


@pytest.mark.parametrize("openingbook_fixture", ["openingbook_8ply", "openingbook_12ply", "openingbook_12ply_dist"])
def test_book_keys_are_sorted(request: pytest.FixtureRequest, openingbook_fixture: str) -> None:
    """Test that the keys (k) in the OpeningBookCore are strictly sorted in ascending order.

    Keys are interpreted as signed 32-bit integers.

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBookCore fixture to use.
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
def test_get_entry_valid_8ply(openingbook_8ply: bbc.OpeningBookCore, index: int, expected: tuple[int, int]) -> None:
    """Test that entries in the 8-ply OpeningBookCore at specific indices match the expected values.

    Args:
        openingbook_8ply (bbc.OpeningBookCore): The 8-ply OpeningBookCore instance.
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
def test_get_entry_valid_12ply_dist(
    openingbook_12ply_dist: bbc.OpeningBookCore, index: int, expected: tuple[int, int]
) -> None:
    """Test that entries in the 12-ply OpeningBookCore with distances at specific indices match the expected values.

    Args:
        openingbook_12ply_dist (bbc.OpeningBookCore): The 12-ply OpeningBookCore instance with distances.
        index (int): The index to test.
        expected (tuple[int, int]): The expected entry value.
    """
    entry = openingbook_12ply_dist.getEntry(index)
    assert isinstance(entry, tuple)
    assert entry == expected


@pytest.mark.parametrize(
    ("index", "expected"),
    [
        (0, (-2124976388, -1)),
        (10, (-2124431688, -1)),
        (100, (-2108174596, -1)),
        (1000, (-2097718536, -1)),
        (10000, (-2027967752, -1)),
        (100000, (-1825638740, -1)),
        (1000000, (-411277128, -1)),
        (1735945 - 1, (2138748968, 0)),
    ],
)
def test_get_entry_valid_12ply(openingbook_12ply: bbc.OpeningBookCore, index: int, expected: tuple[int, int]) -> None:
    """Test that entries in the 12-ply OpeningBookCore with distances at specific indices match the expected values.

    Args:
        openingbook_12ply (bbc.OpeningBookCore): The 12-ply OpeningBookCore instance with distances.
        index (int): The index to test.
        expected (tuple[int, int]): The expected entry value.
    """
    entry = openingbook_12ply.getEntry(index)
    assert isinstance(entry, tuple)
    assert entry == expected


@pytest.mark.parametrize("openingbook_fixture", ["openingbook_8ply", "openingbook_12ply", "openingbook_12ply_dist"])
def test_get_entry_invalid(request: pytest.FixtureRequest, openingbook_fixture: str) -> None:
    """Test that an exception is raised for invalid indices (for all OpeningBookCores).

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBookCore fixture to use.
    """
    openingbook = request.getfixturevalue(openingbook_fixture)
    with pytest.raises(TypeError):
        openingbook.getEntry(-1)
    with pytest.raises(IndexError):
        openingbook.getEntry(openingbook.getBookSize())
    with pytest.raises(IndexError):
        openingbook.getEntry(openingbook.getBookSize() + 1)


@pytest.mark.parametrize(
    ("openingbook_fixture", "expected_size"),
    [
        ("openingbook_8ply", 34515),
        ("openingbook_12ply", 1735945),
        ("openingbook_12ply_dist", 4200899),
    ],
)
def test_get_book_size(request: pytest.FixtureRequest, openingbook_fixture: str, expected_size: int) -> None:
    """Test that the size of the OpeningBookCore is correct for different variants.

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBookCore fixture to use.
        expected_size (int): The expected size of the OpeningBookCore.
    """
    openingbook = request.getfixturevalue(openingbook_fixture)
    size = openingbook.getBookSize()
    assert size == expected_size


@pytest.mark.parametrize("openingbook_fixture", ["openingbook_8ply", "openingbook_12ply", "openingbook_12ply_dist"])
def test_get_book_returns_list(request: pytest.FixtureRequest, openingbook_fixture: str) -> None:
    """Test that getBook() returns a list of the expected size.

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBookCore fixture to use.
    """
    openingbook = request.getfixturevalue(openingbook_fixture)
    book = openingbook.getBook()
    assert isinstance(book, list)
    assert len(book) == openingbook.getBookSize()


@pytest.mark.parametrize(
    ("openingbook_fixture", "move_sequence", "expected_value"),
    [
        ("openingbook_8ply", [2, 3, 3, 3, 3, 3, 5, 5], 0),
        ("openingbook_12ply", [2, 3, 3, 3, 3, 2, 1, 2, 2, 5, 4, 4], 0),
        ("openingbook_12ply", [3, 4, 1, 1, 0, 2, 2, 1, 1, 4, 4, 2], 1),
        ("openingbook_12ply", [2, 3, 3, 3, 3, 2, 1, 2, 2, 5, 1, 6], -1),
        ("openingbook_12ply_dist", [2, 3, 3, 3, 3, 2, 1, 2, 2, 5, 4, 4], 0),
        ("openingbook_12ply_dist", [3, 4, 1, 1, 0, 2, 2, 1, 1, 4, 4, 2], 2),
        ("openingbook_12ply_dist", [2, 3, 3, 3, 3, 2, 1, 2, 2, 5, 1, 6], -10),
    ],
)
def test_get_board_value_known_position(
    request: pytest.FixtureRequest, openingbook_fixture: str, move_sequence: list[int], expected_value: int
) -> None:
    """Test that the correct value is returned for a known position in the OpeningBookCore.

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBookCore fixture to use.
        move_sequence (list[int]): The list of moves representing the board position.
        expected_value (int): The expected value for the given position.
    """
    openingbook = request.getfixturevalue(openingbook_fixture)
    # move_list, expected_value = move_sequence, expected_value
    board = bbc.BoardCore()
    assert board.setBoard(move_sequence)
    val = openingbook.getBoardValue(board)
    assert val == expected_value


@pytest.mark.parametrize(
    ("openingbook_fixture", "move_sequence", "expected_value"),
    [
        ("openingbook_8ply", [2, 3, 3, 3, 3, 3, 5, 5], True),
        ("openingbook_8ply", [1, 3, 4, 3, 4, 4, 3, 3], True),
        ("openingbook_8ply", [3, 3, 3, 3, 3, 1, 1, 1], False),
        ("openingbook_12ply", [2, 3, 3, 3, 3, 2, 1, 2, 2, 5, 4, 4], True),
        ("openingbook_12ply", [3, 4, 1, 1, 0, 2, 2, 1, 1, 4, 4, 2], False),
        ("openingbook_12ply", [2, 3, 3, 3, 3, 2, 1, 2, 2, 5, 1, 6], True),
        ("openingbook_12ply_dist", [2, 3, 3, 3, 3, 2, 1, 2, 2, 5, 4, 4], True),
        ("openingbook_12ply_dist", [3, 4, 1, 1, 0, 2, 2, 1, 1, 4, 4, 2], True),
        ("openingbook_12ply_dist", [2, 3, 3, 3, 3, 2, 1, 2, 2, 5, 1, 6], True),
    ],
)
def test_is_in_book(
    request: pytest.FixtureRequest, openingbook_fixture: str, move_sequence: list[int], expected_value: bool
) -> None:
    """Test that a known position or its mirrored variant is contained in the OpeningBookCore.

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBookCore fixture to use.
        move_sequence (list[int]): The list of moves representing the board position.
        expected_value (bool): The expected result for isInBook(board).
    """
    openingbook = request.getfixturevalue(openingbook_fixture)
    board = bbc.BoardCore()
    assert board.setBoard(move_sequence)

    # Positions with wins for YELLOW are not in the opening-book
    assert (openingbook.isInBook(board) or openingbook.isInBook(board.mirror())) == expected_value


def test_convert_value(openingbook_8ply: bbc.OpeningBookCore) -> None:
    """Test that convertValue() correctly converts the value for a random board.

    Args:
        openingbook_8ply (bbc.OpeningBookCore): The 8-ply OpeningBookCore instance.
    """
    board, _ = bbc.BoardCore.randomBoard(8, True)
    v = openingbook_8ply.getBoardValue(board)
    converted = openingbook_8ply.convertValue(v, board)
    assert converted == v


def test_read_book_static_8ply() -> None:
    """Test static reading of the 8-ply OpeningBookCore and check the size."""
    db_path = bbd.BitBullyDatabases.get_database_path("8-ply")
    book = bbc.OpeningBookCore.readBook(db_path, with_distances=False, is_8ply=True)
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
    """Test that getNPly() returns the correct ply value for different OpeningBookCores.

    Args:
        request (pytest.FixtureRequest): The pytest fixture request object.
        openingbook_fixture (str): The name of the OpeningBookCore fixture to use.
        expected_nply (int): The expected ply value.
    """
    openingbook = request.getfixturevalue(openingbook_fixture)
    assert openingbook.getNPly() == expected_nply
