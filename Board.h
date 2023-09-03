#ifndef XBITBULLY__BOARD_H_
#define XBITBULLY__BOARD_H_

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <sstream>

// TODO: Measure the time differences. Is it really worth the hassle here???
/*
 * // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
 * A generalization of the best bit counting method to integers of bit-widths
upto 128 (parameterized by type T) is this:

v = v - ((v >> 1) & (T)~(T)0/3);                           // temp
v = (v & (T)~(T)0/15*3) + ((v >> 2) & (T)~(T)0/15*3);      // temp
v = (v + (v >> 4)) & (T)~(T)0/255*15;                      // temp
c = (T)(v * ((T)~(T)0/255)) >> (sizeof(T) - 1) * CHAR_BIT; // count
*/
#if __GNUC__
#define uint64_t_popcnt __builtin_popcountll
#else
#if _MSC_VER
#include <intrin.h>
#define uint64_t_popcnt __popcnt64
#else
#define uint64_t_popcnt popCountBoard
#endif
#endif

namespace BitBully {

constexpr int CHAR_BIT = 8;

static constexpr uint64_t getMask(std::initializer_list<int> bits) {
  uint64_t bb{UINT64_C(0)};
  for (const auto i : bits) {
    bb |= (UINT64_C(1) << i);
  }
  return bb;
}

static constexpr bool isIllegalBit(int bitIdx) {
  constexpr int COLUMN_BIT_OFFSET = 9; // TODO: redundant in class below. Fix??
  constexpr int N_ROWS = 6;            // TODO: redundant in class below. Fix??
  constexpr int COLUMNS = 7;           // TODO: redundant in class below. Fix??
  return bitIdx >= COLUMN_BIT_OFFSET * COLUMNS ||
         (bitIdx % COLUMN_BIT_OFFSET) / N_ROWS;
}

static constexpr uint64_t illegalBitMask() {
  uint64_t bb{UINT64_C(0)};
  for (auto i = 0; i < CHAR_BIT * sizeof(uint64_t); ++i) {
    bb ^= (isIllegalBit(i) ? UINT64_C(1) << i : UINT64_C(0));
  }
  return bb;
}

class Board {
  friend class BoardTest;

public:
  Board();
  static const int N_COLUMNS = 7;
  static const int N_ROWS = 6;
  static const int COLUMN_BIT_OFFSET = 9;
  enum Player { P_EMPTY = 0, P_YELLOW = 1, P_RED = 2 };
  static const size_t N_VALID_BOARD_VALUES = 3; // P_EMPTY, P_YELLOW, P_RED
  using TBitBoard = uint64_t;
  using TMovesCounter = int;
  using TBoardArray = std::array<std::array<int32_t, N_ROWS>, N_COLUMNS>;

  void inline playMoveFastBB(TBitBoard mv) {
    assert(mv != BB_EMPTY); // TODO: Only in Debug mode
    assert((mv & BB_ILLEGAL) == BB_EMPTY);
    assert((m_bAll & mv) == BB_EMPTY);
    m_bActive ^= m_bAll; // Already, switch player

    // However, move is performed for current player (assuming, above switch is
    // not yet performed)
    m_bAll ^= mv; // bitwise xor and bitwise or are equivalent here
    m_movesLeft--;
  }

  Board inline playMoveOnCopy(TBitBoard mv) {
    Board b = *this;
    b.playMoveFastBB(mv);
    return b;
  }

  TBitBoard generateMoves();

  static constexpr int popCountBoard(uint64_t x) {
    int count = 0;
    while (x) {
      count += static_cast<int>(x & 1);
      x >>= 1;
    }
    return count;
  }

  int popCountBoard();

  bool isLegalMove(int column);

private:
  /* [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [59, 50, 41, 32, 23, 14,  5],
   * [58, 49, 40, 31, 22, 13,  4],
   * [57, 48, 39, 30, 21, 12,  3],
   * [56, 47, 38, 29, 20, 11,  2],
   * [55, 46, 37, 28, 19, 10,  1],
   * [54, 45, 36, 27, 18,  9,  0]
   */
  static constexpr auto BOTTOM_ROW_BITS = {54, 45, 36, 27, 18, 9, 0};
  static constexpr TBitBoard BB_BOTTOM_ROW = getMask(BOTTOM_ROW_BITS);
  static constexpr auto TOP_ROW_BITS = {59, 50, 41, 32, 23, 14, 5};
  static constexpr TBitBoard BB_TOP_ROW = getMask(TOP_ROW_BITS);
  static constexpr TBitBoard BB_ILLEGAL = illegalBitMask();
  static constexpr TBitBoard BB_ALL_LEGAL_TOKENS = ~BB_ILLEGAL;
  static constexpr TBitBoard BB_EMPTY{UINT64_C(0)};

public:
  bool setBoard(const TBoardArray &board);

  TBoardArray toArray();

  static bool isValid(const TBoardArray &board);

  bool playMove(int column);

  bool hasWin();

  std::string toString();

  inline TMovesCounter movesLeft() { return m_movesLeft; }

private:
  /* Having a bitboard that contains all stones and another one representing the
   * current active player has the advantage that we do not have to do any
   * branching to figure out which player's turn it is. After each move we
   * simply apply an XOR-operation to switch players. */
  /* [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [59, 50, 41, 32, 23, 14,  5],
   * [58, 49, 40, 31, 22, 13,  4],
   * [57, 48, 39, 30, 21, 12,  3],
   * [56, 47, 38, 29, 20, 11,  2],
   * [55, 46, 37, 28, 19, 10,  1],
   * [54, 45, 36, 27, 18,  9,  0]
   */
  TBitBoard m_bAll, m_bActive; // TODO: rename to m_bAllTokens, m_bActivePTokens
  TMovesCounter m_movesLeft;

  static TBitBoard winningPositions(TBitBoard x);

  auto static inline constexpr getColumnMask(int column) {
    assert(column >= 0 && column < N_COLUMNS);
    return (UINT64_C(1) << (column * COLUMN_BIT_OFFSET + N_ROWS)) -
           (UINT64_C(1) << (column * COLUMN_BIT_OFFSET));
  }

  static inline constexpr uint64_t getMaskColRow(int column, int row) {
    assert(column >= 0 && column < N_COLUMNS);
    assert(row >= 0 && row < N_ROWS);
    return UINT64_C(1) << (column * COLUMN_BIT_OFFSET + row);
  }

  static inline constexpr Player opponent(Player p) {
    return static_cast<Player>(3 - p);
  }

  void inline playMoveFast(int column) {
    assert(column >= 0 && column < N_COLUMNS);
    TBitBoard columnMask = getColumnMask(column);
    assert(uint64_t_popcnt(columnMask) == N_ROWS);
    auto mvMask = (m_bAll + BB_BOTTOM_ROW) & columnMask;
    playMoveFastBB(mvMask);
  }
};

} // namespace BitBully

#endif // XBITBULLY__BOARD_H_
