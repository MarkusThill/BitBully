from bitbully import bitbully_core as bbc
import importlib.resources

def test_12_ply_with_distatance():
    db_path = importlib.resources.files("bitbully").joinpath("assets/book_12ply_distances.dat")
    bitbully = bbc.BitBully(db_path)
    b = bbc.Board()  # Empty board
    assert [-2, -1, 0, 1, 0, -1, -2] == bitbully.scoreMoves(b),  "expected result: [-2, -1, 0, 1, 0, -1, -2]"