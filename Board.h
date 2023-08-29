#ifndef XBITBULLY__BOARD_H_
#define XBITBULLY__BOARD_H_

#include <array>
#include <cstddef>
#include <cstdint>

namespace BitBully {

class Board {
  friend class BoardTest;

public:
  Board();
  static const int N_COLUMNS = 7;
  static const int N_ROWS = 6;
  enum Player { P_EMPTY = 0, P_YELLOW = 1, P_RED = 2 };
  static const size_t N_VALID_BOARD_VALUES = 3; // P_EMPTY, P_YELLOW, P_RED
  using TBitBoard = uint64_t;
  using TMovesCounter = int;
  using THeight = int;
  using THeightsArray = std::array<THeight, N_COLUMNS>;
  using TBoardArray = std::array<std::array<int32_t, N_ROWS>, N_COLUMNS>;

  bool setBoard(const TBoardArray &board);

  static THeightsArray getColumnHeights(const TBitBoard &bbYellow,
                                        const TBitBoard &bbRed);

  static TBoardArray toArray(const TBitBoard &bbYellow, const TBitBoard &bbRed);

  TBoardArray toArray();

  static bool isValid(const TBoardArray &board);

  bool playMove(int column);

  bool hasWin();

private:
  /* Having a bitboard that contains all stones and another one representing the
   * current active player has the advantage that we do not have to do any
   * branching to figure out which player's turn it is. After each move we
   * simply apply an XOR-operation to switch players. */
  TBitBoard m_bAll, m_bActive;
  TMovesCounter m_movesLeft;

  // Private Functions
private:
  static inline TBitBoard getMask(int col_idx, int row_idx) {
    const auto bit_idx = N_COLUMNS * N_ROWS - 1 - (col_idx * N_ROWS + row_idx);
    return UINT64_C(1) << bit_idx;
  }
  void playMoveFast(int column);
};

} // namespace BitBully

#endif // XBITBULLY__BOARD_H_
