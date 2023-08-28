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
  using THeight = int;
  using THeightsArray = std::array<THeight, N_COLUMNS>;
  using TBoardArray = std::array<std::array<int32_t, N_ROWS>, N_COLUMNS>;

  struct BitBoard {
    TBitBoard bbYellow, bbRed;
  } m_board;

  bool setBoard(const TBoardArray &board);

  bool setBoard(TBitBoard bbYellow, TBitBoard bbRed);

  inline bool setBoard(BitBoard bb) { return setBoard(bb.bbYellow, bb.bbRed); }

  void setBoardFast(TBitBoard bbYellow, TBitBoard bbRed);

  void setBoardFast(BitBoard bb);

  static THeightsArray getColumnHeights(const TBitBoard &bbYellow,
                                        const TBitBoard &bbRed);

  inline static THeightsArray getColumnHeights(const BitBoard &bb) {
    return getColumnHeights(bb.bbYellow, bb.bbRed);
  }

  static TBoardArray toArray(const TBitBoard &bbYellow, const TBitBoard &bbRed);

  static inline TBoardArray toArray(const BitBoard &bb) {
    return toArray(bb.bbYellow, bb.bbRed);
  }

  inline TBoardArray toArray() { return toArray(m_board); }

  inline auto toBitBoard() { return m_board; }

  static bool isValid(const TBoardArray &board);

  bool hasWinYellow();

  bool hasWinRed();

  bool playMove(int column);

  bool hasWin();

private:
  THeightsArray m_columnHeight;

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
