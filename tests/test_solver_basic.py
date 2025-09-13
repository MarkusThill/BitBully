import bitbully.bitbully_core as bbc
import time

def test_random_board_generation():
    b, moves = bbc.Board.randomBoard(10, True)
    assert isinstance(moves, list)
    assert isinstance(str(b), str)
    # Check that move sequence length is correct
    assert len(moves) == 10, "Generated move list should match requested length"


from bitbully import bitbully_core

def test_mtdf():
    board = bitbully_core.Board()

    # Yellow and red alternately play moves into column 3 (center column):
    for _ in range(6):
        board.playMove(3)

    solver = bitbully_core.BitBully()
    t_start = time.time()
    score = solver.mtdf(board, first_guess=0)
    t_end = time.time()
    duration = t_end - t_start
    assert duration < 10, f"Should not take longer than 10 seconds. But took me {duration} sceonds."
    assert score == 1, "Fatal: the score is wrong!"