#ifndef XBITBULLY__BOARD_H_
#define XBITBULLY__BOARD_H_

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <map>
#include <sstream>

// TODO: Move function definitions to .cpp file!
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

#ifndef CHAR_BIT
constexpr int CHAR_BIT = 8;
#endif

static constexpr uint64_t getMask(const std::initializer_list<int> bits) {
  uint64_t bb{UINT64_C(0)};
  for (const auto i : bits) {
    // return 0, if index is out of range (0-63)
    if (i < 0 || i >= 64) {
      return UINT64_C(0);
    }
    bb |= (UINT64_C(1) << i);
  }
  return bb;
}

static constexpr bool isIllegalBit(int bitIdx) {
  constexpr int COLUMN_BIT_OFFSET = 9;  // TODO: redundant in class below. Fix??
  constexpr int N_ROWS = 6;             // TODO: redundant in class below. Fix??
  constexpr int COLUMNS = 7;            // TODO: redundant in class below. Fix??
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
  static constexpr int N_COLUMNS = 7;
  static constexpr int N_ROWS = 6;
  static constexpr int COLUMN_BIT_OFFSET = 9;
  enum Player { P_EMPTY = 0, P_YELLOW = 1, P_RED = 2 };
  static constexpr size_t N_VALID_BOARD_VALUES = 3;  // P_EMPTY, P_YELLOW, P_RED
  using TBitBoard = uint64_t;
  using TMovesCounter = int;
  using TBoardArray = std::array<std::array<int32_t, N_ROWS>, N_COLUMNS>;

  void inline playMoveFastBB(TBitBoard mv) {
    assert(mv != BB_EMPTY);
    assert((mv & BB_ILLEGAL) == BB_EMPTY);
    assert((m_bAll & mv) == BB_EMPTY);
    m_bActive ^= m_bAll;  // Already, switch player

    // However, move is performed for current player (assuming, above switch is
    // not yet performed)
    m_bAll ^= mv;  // bitwise xor and bitwise or are equivalent here
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

  inline auto popCountBoard() const { return uint64_t_popcnt(m_bAll); }

  bool isLegalMove(int column);

  static uint64_t hash(uint64_t x) {
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
  }

  uint64_t hash() const { return hash(m_bActive) ^ hash(m_bAll); }

  static TBitBoard nextMove(TBitBoard allMoves) {
    for (auto p : BB_MOVES_PRIO_LIST) {
      if (const TBitBoard pvMv = allMoves & p) {
        allMoves = pvMv;
        break;
      }
    }
    return lsb(allMoves);
  }

  bool operator==(const Board &b) const {
    const bool equal = (b.m_bAll == m_bAll && b.m_bActive == m_bActive);

    // Assert that if board is equal that also movesLeft are equal
    assert(equal && (b.m_movesLeft == m_movesLeft) || !equal);
    return equal;
  }

  // TBitBoard movesUnderOwnThreats(TBitBoard moves);

  // std::pair<unsigned long, unsigned long> findOddEvenThreats();

  TBitBoard findOddThreats(TBitBoard moves);

 private:
  /* [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ 5, 14, 23, 32, 41, 50, 59],
   * [ 4, 13, 22, 31, 40, 49, 58],
   * [ 3, 12, 21, 30, 39, 48, 57],
   * [ 2, 11, 20, 29, 38, 47, 56],
   * [ 1, 10, 19, 28, 37, 46, 55],
   * [ 0,  9, 18, 27, 36, 45, 54]
   */
  static constexpr auto BOTTOM_ROW_BITS = {54, 45, 36, 27, 18, 9, 0};
  static constexpr TBitBoard BB_BOTTOM_ROW = getMask(BOTTOM_ROW_BITS);
  static constexpr auto TOP_ROW_BITS = {59, 50, 41, 32, 23, 14, 5};
  static constexpr TBitBoard BB_TOP_ROW = getMask(TOP_ROW_BITS);
  static constexpr TBitBoard BB_ILLEGAL = illegalBitMask();
  static constexpr TBitBoard BB_ALL_LEGAL_TOKENS = ~BB_ILLEGAL;
  static constexpr TBitBoard BB_EMPTY{UINT64_C(0)};
  static constexpr TBitBoard BB_MOVES_PRIO1 = getMask({29, 30});
  static constexpr TBitBoard BB_MOVES_PRIO2 = getMask({31, 21, 20, 28, 38, 39});
  static constexpr TBitBoard BB_MOVES_PRIO3 = getMask({40, 32, 22, 19, 27, 37});
  static constexpr TBitBoard BB_MOVES_PRIO4 = getMask({47, 48, 11, 12});
  static constexpr TBitBoard BB_MOVES_PRIO5 =
      getMask({49, 41, 23, 13, 10, 18, 36, 46});
  static constexpr TBitBoard BB_MOVES_PRIO6 = getMask({45, 50, 14, 9});
  static constexpr auto BB_MOVES_PRIO_LIST = {BB_MOVES_PRIO1, BB_MOVES_PRIO2,
                                              BB_MOVES_PRIO3, BB_MOVES_PRIO4,
                                              BB_MOVES_PRIO5, BB_MOVES_PRIO6};

 public:
  bool setBoard(const TBoardArray &board);

  TBoardArray toArray();

  static bool isValid(const TBoardArray &board);

  bool playMove(int column);

  bool canWin();

  std::string toString();

  inline TMovesCounter movesLeft() { return m_movesLeft; }

  Board mirror() const;

  Board::TBitBoard findThreats(TBitBoard moves);

  static inline TBitBoard lsb(TBitBoard x) {
    auto mvMask = x - UINT64_C(1);
    return ~mvMask & x;
  }

  TBitBoard generateNonLosingMoves() {
    // Mostly inspired by Pascal's Code
    // This function might return an empty bitboard. In this case, the active
    // player will lose, since all possible moves will lead to a defeat.
    TBitBoard moves = generateMoves();
    TBitBoard threats = winningPositions(m_bActive ^ m_bAll, true);
    TBitBoard directThreats = threats & moves;
    if (directThreats) {
      // no way we can neutralize more than one direct threat...
      moves = directThreats & (directThreats - 1) ? UINT64_C(0) : directThreats;
    }

    // No token under an opponents threat.
    return moves & ~(threats >> 1);
  }

  TBitBoard doubleThreat(TBitBoard moves) {
    TBitBoard ownThreats = winningPositions(m_bActive, false);
    TBitBoard otherThreats = winningPositions(m_bActive ^ m_bAll, true);
    return moves & (ownThreats >> 1) & (ownThreats >> 2) & ~(otherThreats >> 1);
  }

 private:
  /* Having a bitboard that contains all stones and another one representing the
   * current active player has the advantage that we do not have to do any
   * branching to figure out which player's turn it is. After each move we
   * simply apply an XOR-operation to switch players. */
  /* [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ 5, 14, 23, 32, 41, 50, 59],
   * [ 4, 13, 22, 31, 40, 49, 58],
   * [ 3, 12, 21, 30, 39, 48, 57],
   * [ 2, 11, 20, 29, 38, 47, 56],
   * [ 1, 10, 19, 28, 37, 46, 55],
   * [ 0,  9, 18, 27, 36, 45, 54]
   */
  TBitBoard m_bAll,
      m_bActive;  // TODO: rename to m_bAllTokens, m_bActivePTokens
  TMovesCounter m_movesLeft;

  static TBitBoard winningPositions(TBitBoard x, bool verticals);

  auto static inline constexpr getColumnMask(int column) {
    assert(column >= 0 && column < N_COLUMNS);
    // auto rc = N_COLUMNS - column - 1;
    return (UINT64_C(1) << (column * COLUMN_BIT_OFFSET + N_ROWS)) -
           (UINT64_C(1) << (column * COLUMN_BIT_OFFSET));
  }

  auto static inline constexpr getRowMask(int row) {
    assert(row >= 0 && row < N_ROWS);
    TBitBoard mask{0};
    for (int i = 0; i < N_COLUMNS; ++i) {
      mask |= (UINT64_C(1) << (i * COLUMN_BIT_OFFSET + row));
    }
    return mask;
  }

  /* [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ 5, 14, 23, 32, 41, 50, 59],
   * [ 4, 13, 22, 31, 40, 49, 58],
   * [ 3, 12, 21, 30, 39, 48, 57],
   * [ 2, 11, 20, 29, 38, 47, 56],
   * [ 1, 10, 19, 28, 37, 46, 55],
   * [ 0,  9, 18, 27, 36, 45, 54]
   */
  auto static constexpr mirrorBitBoard(TBitBoard x) {
    // TODO: It should be possible to do it in x only (using XORS). But,
    // premature optimization is the root of all evil. Try this later.
    // TODO: Any difference using XOR instead of OR? (probably not)...
    TBitBoard y{UINT64_C(0)};
    // move left-most column to right-most and vice versa:
    y |= ((x & getColumnMask(6)) >> 6 * COLUMN_BIT_OFFSET);
    y |= ((x & getColumnMask(0)) << 6 * COLUMN_BIT_OFFSET);

    // Same with columns 1 & 5...
    y |= ((x & getColumnMask(5)) >> 4 * COLUMN_BIT_OFFSET);
    y |= ((x & getColumnMask(1)) << 4 * COLUMN_BIT_OFFSET);

    // Same with columns 2 & 4
    y |= ((x & getColumnMask(4)) >> 2 * COLUMN_BIT_OFFSET);
    y |= ((x & getColumnMask(2)) << 2 * COLUMN_BIT_OFFSET);

    // column 3 stays where it is...
    return y | (x & getColumnMask(3));
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

  bool hasWon();
};

}  // namespace BitBully

#endif  // XBITBULLY__BOARD_H_
