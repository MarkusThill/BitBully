#ifndef XBITBULLY__BOARD_H_
#define XBITBULLY__BOARD_H_

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <vector>

#include "MoveList.h"

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

static constexpr bool isIllegalBit(const int bitIdx) {
  constexpr int COLUMN_BIT_OFFSET = 9;  // TODO: redundant in class below. Fix??
  constexpr int N_ROWS = 6;             // TODO: redundant in class below. Fix??
  constexpr int COLUMNS = 7;            // TODO: redundant in class below. Fix??
  return bitIdx >= COLUMN_BIT_OFFSET * COLUMNS ||
         (bitIdx % COLUMN_BIT_OFFSET) / N_ROWS;
}

static constexpr uint64_t illegalBitMask() {
  uint64_t bb{UINT64_C(0)};
  for (size_t i = 0; i < CHAR_BIT * sizeof(uint64_t); ++i) {
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
  using TBoardArrayT = std::array<std::array<int32_t, N_COLUMNS>, N_ROWS>;

  [[nodiscard]] Board inline playMoveOnCopy(const TBitBoard mv) const {
    Board b = *this;
    b.playMoveFastBB(mv);
    return b;
  }

  [[nodiscard]] Board inline copy() const {
    Board b = *this;
    return b;
  }

  [[nodiscard]] TBitBoard generateMoves() const;

  [[nodiscard]] static constexpr int popCountBoard(uint64_t x) {
    int count = 0;
    while (x) {
      count += static_cast<int>(x & 1);
      x >>= 1;
    }
    return count;
  }

  [[nodiscard]] inline auto popCountBoard() const {
    return uint64_t_popcnt(m_bAllTokens);
  }

  [[nodiscard]] bool isLegalMove(int column) const;

  [[nodiscard]] static uint64_t hash(uint64_t x) {
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
  }

  [[nodiscard]] uint64_t uid() const {
    // the resulting 64-bit integer is a unique identifier for each board
    // Can be used to store a position in a transposition table
    return m_bActivePTokens + m_bAllTokens;
  }

  [[nodiscard]] uint64_t hash() const {
    return hash(hash(m_bActivePTokens) ^ (hash(m_bAllTokens) << 1));
  }

  [[nodiscard]] static TBitBoard nextMove(TBitBoard allMoves) {
    for (const auto p : BB_MOVES_PRIO_LIST) {
      if (const TBitBoard pvMv = allMoves & p) {
        allMoves = pvMv;
        break;
      }
    }
    return lsb(allMoves);
  }

  [[nodiscard]] bool operator==(const Board& b) const {
    const bool equal = (b.m_bAllTokens == m_bAllTokens &&
                        b.m_bActivePTokens == m_bActivePTokens);

    // Assert that if board is equal that also movesLeft are equal
    assert((equal && (b.m_movesLeft == m_movesLeft)) || !equal);
    return equal;
  }

  [[nodiscard]] bool operator!=(const Board& b) const { return !(b == *this); }

  [[nodiscard]] TBitBoard findOddThreats(TBitBoard moves);

  [[nodiscard]] bool setBoard(const TBoardArray& board);

  [[nodiscard]] bool setBoard(const TBoardArrayT& board);

  [[nodiscard]] bool setBoard(const std::vector<int>& moveSequence);

  bool play(int column);
  [[nodiscard]] bool play(const std::vector<int>& moveSequence);
  [[nodiscard]] bool play(const std::string& moveSequence);

  [[nodiscard]] bool setBoard(const std::string& moveSequence);

  [[nodiscard]] TBoardArray toArray() const;

  [[nodiscard]] static bool isValid(const TBoardArray& board);

  [[nodiscard]] bool canWin() const;

  [[nodiscard]] bool canWin(int column) const;

  [[nodiscard]] bool hasWin() const;

  [[nodiscard]] std::string toString() const;

  [[nodiscard]] inline TMovesCounter movesLeft() const { return m_movesLeft; }

  [[nodiscard]] inline TMovesCounter countTokens() const {
    return N_ROWS * N_COLUMNS - m_movesLeft;
  }

  [[nodiscard]] Board mirror() const;

  [[nodiscard]] MoveList sortMoves(TBitBoard moves) const;

  TBitBoard findThreats(TBitBoard moves);

  static inline TBitBoard lsb(const TBitBoard x) {
    const auto mvMask = x - UINT64_C(1);
    return ~mvMask & x;
  }

  [[nodiscard]] TBitBoard generateNonLosingMoves() const {
    // Mostly inspired by Pascal's Code
    // This function might return an empty bitboard. In this case, the active
    // player will lose, since all possible moves will lead to a defeat.
    TBitBoard moves = generateMoves();
    const TBitBoard threats =
        winningPositions(m_bActivePTokens ^ m_bAllTokens, true);
    if (const TBitBoard directThreats = threats & moves) {
      // no way we can neutralize more than one direct threat...
      moves = directThreats & (directThreats - 1) ? UINT64_C(0) : directThreats;
    }

    // No token under an opponent's threat.
    return moves & ~(threats >> 1);
  }

  [[nodiscard]] TBitBoard doubleThreat(const TBitBoard moves) const {
    const TBitBoard ownThreats = winningPositions(m_bActivePTokens, false);
    const TBitBoard otherThreats =
        winningPositions(m_bActivePTokens ^ m_bAllTokens, true);
    return moves & (ownThreats >> 1) & (ownThreats >> 2) & ~(otherThreats >> 1);
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
  [[nodiscard]] int toHuffman() const {
    // This function is only defined for positions with an even number of tokens
    // and for positions with less or equal than 12 tokens.
    if (m_movesLeft < 30 || m_movesLeft & 1) {
      return 0;
    }
    int huff = INT64_C(0);

    for (int i = 0; i < N_COLUMNS; ++i) {
      auto all = m_bAllTokens;
      auto active = m_bActivePTokens;
      all >>= (i * COLUMN_BIT_OFFSET);
      active >>= (i * COLUMN_BIT_OFFSET);
      for (int j = 0; j < N_ROWS && (all & 1); j++) {
        huff <<= 2;  // we will insert 2 bits for yellow or red
        huff |= (active & 1) ? 2 : 3;  // yellow-> 10b, red -> 11b
        all >>= 1;
        active >>= 1;
      }
      huff <<= 1;  // insert 0 to indicate the end of the column
    }
    // length until here (for 12-ply position): 12*2+7 = 31
    return huff << 1;  // add one 0-bit to fill up to a full byte
  }

  static std::pair<Board, std::vector<int>> randomBoard(
      const int nPly, const bool forbidDirectWin = true) {
    if (nPly < 0 || nPly > N_COLUMNS * N_ROWS) {
      return {};
    }

    auto [b, mvList] = randomBoardInternal(nPly);

    while (mvList.size() != static_cast<decltype(mvList.size())>(nPly) ||
           (forbidDirectWin && b.canWin())) {
      std::tie(b, mvList) = randomBoardInternal(nPly);
    }

    return std::make_pair(b, std::move(mvList));
  }

  [[nodiscard]] std::vector<Board> allPositions(const int upToNPly,
                                                bool exactlyN) const {
    // https://oeis.org/A212693
    std::map<uint64_t, Board> positions;
    positions.insert({uid(), *this});  // add empty board
    addAfterStates(positions, *this, upToNPly);

    std::vector<Board> boardVector;
    boardVector.reserve(positions.size());  // Optimize memory allocation

    for (const auto& [key, board] : positions) {
      if (!exactlyN || board.countTokens() == upToNPly)
        boardVector.push_back(board);  // Copy each board into the vector
    }
    return boardVector;
  }

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

  // These two center fields generally are the most promising ones:
  static constexpr TBitBoard BB_MOVES_PRIO1 = getMask({29, 30});

  // After {29, 30}, we should consider these moves, and so on:
  static constexpr TBitBoard BB_MOVES_PRIO2 = getMask({31, 21, 20, 28, 38, 39});
  static constexpr TBitBoard BB_MOVES_PRIO3 = getMask({40, 32, 22, 19, 27, 37});
  static constexpr TBitBoard BB_MOVES_PRIO4 = getMask({47, 48, 11, 12});
  static constexpr TBitBoard BB_MOVES_PRIO5 =
      getMask({49, 41, 23, 13, 10, 18, 36, 46});
  static constexpr TBitBoard BB_MOVES_PRIO6 = getMask({45, 50, 14, 9});
  static constexpr auto BB_MOVES_PRIO_LIST = {BB_MOVES_PRIO1, BB_MOVES_PRIO2,
                                              BB_MOVES_PRIO3, BB_MOVES_PRIO4,
                                              BB_MOVES_PRIO5, BB_MOVES_PRIO6};

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
  TBitBoard m_bAllTokens, m_bActivePTokens;
  TMovesCounter m_movesLeft;

  static TBitBoard winningPositions(TBitBoard x, bool verticals);

  auto static inline constexpr getColumnMask(const int column) {
    assert(column >= 0 && column < N_COLUMNS);
    return (UINT64_C(1) << (column * COLUMN_BIT_OFFSET + N_ROWS)) -
           (UINT64_C(1) << (column * COLUMN_BIT_OFFSET));
  }

  auto static inline constexpr getRowMask(const int row) {
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
  auto static constexpr mirrorBitBoard(const TBitBoard x) {
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

  static constexpr uint64_t getMaskColRow(const int column, const int row) {
    assert(column >= 0 && column < N_COLUMNS);
    assert(row >= 0 && row < N_ROWS);
    return UINT64_C(1) << (column * COLUMN_BIT_OFFSET + row);
  }

  static constexpr Player opponent(Player p) {
    return static_cast<Player>(3 - p);
  }

  void inline playMoveFastBB(const TBitBoard mv) {
    assert(mv != BB_EMPTY);
    assert((mv & BB_ILLEGAL) == BB_EMPTY);
    assert((m_bAllTokens & mv) == BB_EMPTY);
    m_bActivePTokens ^= m_bAllTokens;  // Already, switch player

    // However, move is performed for current player (assuming, above switch is
    // not yet performed)
    m_bAllTokens ^= mv;  // bitwise xor and bitwise or are equivalent here
    m_movesLeft--;
  }

  void inline playMoveFast(const int column) {
    assert(column >= 0 && column < N_COLUMNS);
    const TBitBoard columnMask = getColumnMask(column);
    assert(uint64_t_popcnt(columnMask) == N_ROWS);
    const auto mvMask = (m_bAllTokens + BB_BOTTOM_ROW) & columnMask;
    playMoveFastBB(mvMask);
  }

  static void addAfterStates(std::map<uint64_t, Board>& boardCollection,
                             const Board& b, const int nPly) {
    if (b.countTokens() >= nPly) {
      return;
    }

    auto moves = b.generateMoves();

    while (moves) {
      const auto mv = b.nextMove(moves);
      assert(uint64_t_popcnt(mv) == 1);
      if (auto newB = b.playMoveOnCopy(mv);
          boardCollection.find(newB.uid()) == boardCollection.end() &&
          !b.hasWin()) {
        // We have not  reached this position yet
        boardCollection.insert({newB.uid(), newB});
        addAfterStates(boardCollection, newB, nPly);
      }

      moves ^= mv;
    }
  }

  static std::pair<Board, std::vector<int>> randomBoardInternal(
      const int nPly) {
    if (nPly < 0 || nPly > N_COLUMNS * N_ROWS) {
      return {};
    }
    Board b;

    // Create a random device to seed the random number generator
    static std::random_device rd;

    // Create a Mersenne Twister random number generator
    static std::mt19937 gen(rd());

    // Create a uniform integer distribution for the desired range
    static std::uniform_int_distribution<> nextUniform(0, N_COLUMNS);

    std::vector<int> mvSequence;
    static constexpr int MAX_TRIES = 20;
    for (int j = 0; j < nPly; ++j) {
      int randColumn, tries = 0;
      do {
        randColumn = nextUniform(gen);
        tries++;
      } while (tries < MAX_TRIES &&
               (!b.isLegalMove(randColumn) || b.canWin(randColumn)));
      if (tries >= MAX_TRIES) {
        return {};
      }
      b.play(randColumn);
      mvSequence.emplace_back(randColumn);
    }

    assert(b.countTokens() == nPly);

    return {std::move(b), std::move(mvSequence)};
  }

  static TBoardArray transpose(const TBoardArrayT& board);
};

}  // namespace BitBully

#endif  // XBITBULLY__BOARD_H_
