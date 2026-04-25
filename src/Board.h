/**
 * @file Board.h
 * @brief Bitboard-based representation of a Connect-4 position.
 *
 * Defines the @ref BitBully::Board class together with a handful of
 * compile-time helpers (bit masks, popcount, count-trailing-zeros) used by the
 * solver. The encoding follows the "split bitboard" scheme made popular by
 * John Tromp's Fhourstones: a single 64-bit word holds all occupied cells of a
 * 7&times;6 grid, plus an extra "sentinel" row that is never set during play
 * but is used to detect overflowing column appends in constant time.
 *
 * @see https://github.com/qu1j0t3/fhourstones
 */
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
/// Hardware popcount for 64-bit integers (GCC / Clang builtin).
#define uint64_t_popcnt __builtin_popcountll
#else
#if _MSC_VER
#include <intrin.h>
/// Hardware popcount for 64-bit integers (MSVC intrinsic).
#define uint64_t_popcnt __popcnt64
#else
/// Portable fallback popcount when no compiler intrinsic is available.
#define uint64_t_popcnt popCountBoard
#endif
#endif

/**
 * @brief Count trailing zero bits of a 64-bit integer.
 *
 * Returns the bit index of the least significant set bit. The function uses
 * the most efficient implementation available for the current toolchain
 * (`_BitScanForward64` on MSVC, `__builtin_ctzll` on GCC/Clang) and falls
 * back to a portable loop otherwise.
 *
 * @param x Non-zero 64-bit input. Calling with @c x == 0 is undefined for the
 *          intrinsic variants.
 * @return Zero-based index of the least significant set bit.
 */
inline int ctz_u64(uint64_t x) {
#if defined(_MSC_VER)
  unsigned long index;
  _BitScanForward64(&index, x);
  return static_cast<int>(index);
#elif defined(__GNUC__) || defined(__clang__)
  return __builtin_ctzll(x);
#else
  int idx = 0;
  while ((x & 1u) == 0u) {
    x >>= 1u;
    ++idx;
  }
  return idx;
#endif
}

/**
 * @brief Decompose a bitboard into the indices of its set bits.
 *
 * Walks the input from the least to the most significant bit, clearing one
 * bit at a time using `x &= x - 1`. The result is sorted in ascending bit
 * order.
 *
 * @param x Bitboard whose set bits should be enumerated.
 * @return Vector of bit indices (0&hellip;63), one per set bit in @p x.
 */
inline std::vector<int> bits_set(uint64_t x) {
  std::vector<int> result;
  result.reserve(uint64_t_popcnt(x));
  while (x) {
    int bit = ctz_u64(x);
    result.push_back(bit);
    x &= x - UINT64_C(1);
  }
  return result;
}

/// @namespace BitBully
/// @brief Top-level namespace for the BitBully Connect-4 engine.
namespace BitBully {

#ifndef CHAR_BIT
/// Number of bits per byte (defined locally if not provided by the platform).
constexpr int CHAR_BIT = 8;
#endif

/**
 * @brief Build a bitboard with the given bit indices set.
 *
 * Convenience helper used to author compile-time constants (row masks, column
 * masks, priority lists). Indices outside the valid 0&hellip;63 range cause
 * the function to return @c 0.
 *
 * @param bits Indices of the bits to set.
 * @return A 64-bit mask containing exactly the bits in @p bits, or @c 0 if
 *         any index is out of range.
 */
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

/**
 * @brief Test whether a bit index falls outside the playable 7&times;6 grid.
 *
 * Each of the seven columns occupies @c COLUMN_BIT_OFFSET (9) bits inside
 * the 64-bit word, of which only the bottom @c N_ROWS (6) are reachable by
 * play. Bits in the top three positions of every column &mdash; plus all bits
 * past the last column &mdash; are reserved as guard / sentinel bits.
 *
 * @param bitIdx Bit index to test (0&hellip;63).
 * @return @c true if @p bitIdx is a guard / sentinel bit, @c false if the
 *         bit is part of the playable grid.
 */
static constexpr bool isIllegalBit(const int bitIdx) {
  constexpr int COLUMN_BIT_OFFSET = 9;  // TODO: redundant in class below. Fix??
  constexpr int N_ROWS = 6;             // TODO: redundant in class below. Fix??
  constexpr int COLUMNS = 7;            // TODO: redundant in class below. Fix??
  return bitIdx >= COLUMN_BIT_OFFSET * COLUMNS ||
         (bitIdx % COLUMN_BIT_OFFSET) / N_ROWS;
}

/**
 * @brief Compile-time mask containing every "illegal" / guard bit.
 *
 * Used to mask out spurious wins generated when shifting the bitboard across
 * column boundaries. The complement of this mask covers exactly the 42
 * playable cells.
 *
 * @return 64-bit mask with all guard bits set to 1.
 */
static constexpr uint64_t illegalBitMask() {
  uint64_t bb{UINT64_C(0)};
  for (size_t i = 0; i < CHAR_BIT * sizeof(uint64_t); ++i) {
    bb ^= (isIllegalBit(i) ? UINT64_C(1) << i : UINT64_C(0));
  }
  return bb;
}

/**
 * @class Board
 * @brief Connect-4 position represented as a pair of 64-bit bitboards.
 *
 * The board uses two 64-bit integers to encode an entire 7&times;6 position:
 *
 *  - @c m_bAllTokens has a bit set for every occupied cell.
 *  - @c m_bActivePTokens has a bit set for every cell occupied by the player
 *    *whose turn it is*. The opponent mask is therefore obtained as
 *    `m_bActivePTokens ^ m_bAllTokens`.
 *
 * Encoding the position this way removes any branching when switching the
 * side to move &mdash; @c m_bActivePTokens is simply XOR'ed with
 * @c m_bAllTokens after each ply. The internal bit layout is:
 *
 * ```
 * [ *,  *,  *,  *,  *,  *,  *]   <- guard row (never set)
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ 5, 14, 23, 32, 41, 50, 59]   <- top row
 * [ 4, 13, 22, 31, 40, 49, 58]
 * [ 3, 12, 21, 30, 39, 48, 57]
 * [ 2, 11, 20, 29, 38, 47, 56]
 * [ 1, 10, 19, 28, 37, 46, 55]
 * [ 0,  9, 18, 27, 36, 45, 54]   <- bottom row
 * ```
 *
 * Bit indices for a column @c c and row @c r are @c c * 9 + r.
 */
class Board {
  friend class BoardTest;

 public:
  /// Construct an empty board with player Yellow to move.
  Board();
  /// Number of columns in a Connect-4 grid.
  static constexpr int N_COLUMNS = 7;
  /// Number of rows in a Connect-4 grid.
  static constexpr int N_ROWS = 6;
  /// Bit-offset between two columns inside the 64-bit board representation.
  ///
  /// Each column gets one extra "guard" bit on top of the playable rows so
  /// that operations such as `m_bAllTokens + BB_BOTTOM_ROW` cannot leak from
  /// one column into the next.
  static constexpr int COLUMN_BIT_OFFSET = 9;

  /// @brief Player identifiers used by the board's array view.
  enum Player {
    P_EMPTY = 0,   ///< Empty cell.
    P_YELLOW = 1,  ///< Yellow stone (player who moves first).
    P_RED = 2      ///< Red stone (player who moves second).
  };
  /// Number of distinct cell values: empty, yellow, red.
  static constexpr size_t N_VALID_BOARD_VALUES = 3;
  /// 64-bit type used for any bitboard value.
  using TBitBoard = uint64_t;
  /// Type used to count remaining moves on a position.
  using TMovesCounter = int;
  /// Column-major 2D representation of the board (`board[col][row]`).
  using TBoardArray = std::array<std::array<int32_t, N_ROWS>, N_COLUMNS>;
  /// Row-major 2D representation of the board (`board[row][col]`).
  using TBoardArrayT = std::array<std::array<int32_t, N_COLUMNS>, N_ROWS>;

  /**
   * @brief Apply a move (already encoded as a single-bit mask) on a copy of
   *        this board.
   * @param mv Bitboard with exactly one bit set, representing the cell to
   *           occupy. Must be a legal move; behaviour is undefined otherwise.
   * @return The new board after the move was played.
   */
  [[nodiscard]] Board inline playBitMaskOnCopy(const TBitBoard mv) const {
    Board b = *this;
    b.playMoveFastBB(mv);
    return b;
  }

  /**
   * @brief Apply a move given by column index on a copy of this board.
   *
   * @param mv Column index (0&hellip;@c N_COLUMNS-1).
   * @return A new @c Board with the move applied. If @p mv is illegal a
   *         freshly-constructed empty board is returned instead.
   */
  [[nodiscard]] Board inline playMoveOnCopy(const int mv) const {
    // Returns an empty board in case the move is illegal.
    Board b = *this;
    return b.play(mv) ? b : Board();
  }

  /// @brief Return a deep copy of this board.
  [[nodiscard]] Board inline copy() const {
    Board b = *this;
    return b;
  }

  /**
   * @brief Bitboard whose set bits mark the next reachable cell of every
   *        non-full column.
   *
   * For each column the lowest empty cell receives a 1-bit, all other bits
   * are zero. The result has between 0 and @c N_COLUMNS bits set.
   */
  [[nodiscard]] TBitBoard legalMovesMask() const;

  /**
   * @brief Enumerate legal moves as column indices.
   *
   * @param nonLosing  If @c true, restrict the result to moves that do not
   *                   immediately allow the opponent to win. Direct winning
   *                   moves of the active player are always included.
   * @param orderMoves If @c true, sort the moves by the heuristic implemented
   *                   in @c sortMoves() (best first).
   * @return Vector of column indices (0&hellip;@c N_COLUMNS-1).
   */
  [[nodiscard]] std::vector<int> legalMoves(bool nonLosing,
                                            bool orderMoves) const;

  /**
   * @brief Software popcount fallback used when no intrinsic is available.
   *
   * @param x 64-bit input.
   * @return Number of set bits in @p x.
   */
  [[nodiscard]] static constexpr int popCountBoard(uint64_t x) {
    int count = 0;
    while (x) {
      count += static_cast<int>(x & 1);
      x >>= 1;
    }
    return count;
  }

  /**
   * @brief Number of stones currently on the board.
   *
   * Equivalent to @c countTokens() but exposes the raw popcount of the
   * internal bitboard.
   */
  [[nodiscard]] inline auto popCountBoard() const {
    return uint64_t_popcnt(m_bAllTokens);
  }

  /**
   * @brief Check whether dropping a stone in @p column is legal.
   *
   * @param column Column index.
   * @return @c true if @p column is in range and not yet full.
   */
  [[nodiscard]] bool isLegalMove(int column) const;

  /**
   * @brief Splittable-mix-style hash for a 64-bit integer.
   *
   * Implementation of David Stafford's "Mix13" finaliser. Used internally to
   * derive transposition-table indices from board UIDs.
   *
   * @param x 64-bit input.
   * @return Avalanche-mixed 64-bit hash of @p x.
   */
  [[nodiscard]] static uint64_t hash(uint64_t x) {
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
  }

  /**
   * @brief Unique 64-bit identifier of the position.
   *
   * Combines the "all tokens" bitboard with the "active player" bitboard in
   * a way that ensures every legal Connect-4 position maps to a distinct
   * value. Suitable as a key for transposition tables.
   *
   * @return Unique identifier of the current position.
   */
  [[nodiscard]] uint64_t uid() const { return m_bActivePTokens + m_bAllTokens; }

  /**
   * @brief 64-bit hash of the position derived from @ref uid().
   *
   * Equivalent positions (same @c m_bAllTokens and @c m_bActivePTokens) map
   * to the same hash value; mirror-symmetric positions do **not**.
   */
  [[nodiscard]] uint64_t hash() const {
    return hash(hash(m_bActivePTokens) ^ (hash(m_bAllTokens) << 1));
  }

  /**
   * @brief Pick the best-ranked move from a candidate set using a fixed
   *        priority order.
   *
   * The static priority lists @c BB_MOVES_PRIO1 &hellip; @c BB_MOVES_PRIO6
   * encode a center-first ordering. The first priority class that has a
   * non-empty intersection with @p allMoves wins; among the remaining
   * candidates the one with the smallest bit index (i.e.\ lower row, leftmost
   * column) is returned.
   *
   * @param allMoves Bitboard of candidate moves (one bit per cell).
   * @return Bitboard with exactly one bit set selecting the chosen move, or
   *         @c 0 if @p allMoves is empty.
   */
  [[nodiscard]] static TBitBoard nextMove(TBitBoard allMoves) {
    for (const auto p : BB_MOVES_PRIO_LIST) {
      if (const TBitBoard pvMv = allMoves & p) {
        allMoves = pvMv;
        break;
      }
    }
    return lsb(allMoves);
  }

  /// Compare two positions for equality (same stones, same side to move).
  [[nodiscard]] bool operator==(const Board& b) const {
    const bool equal = (b.m_bAllTokens == m_bAllTokens &&
                        b.m_bActivePTokens == m_bActivePTokens);

    // Assert that if board is equal that also movesLeft are equal
    assert((equal && (b.m_movesLeft == m_movesLeft)) || !equal);
    return equal;
  }

  /// Inequality counterpart of @c operator==.
  [[nodiscard]] bool operator!=(const Board& b) const { return !(b == *this); }

  /**
   * @brief Identify candidate moves that introduce a new "odd" threat.
   *
   * A threat sitting in an odd-numbered row (rows 3 and 5 from below) is
   * particularly valuable for the first player due to Connect-4's
   * even/odd-row strategy theory.
   *
   * @param moves Set of candidate moves (bitboard).
   * @return Sub-set of @p moves that increase the number of odd threats.
   */
  [[nodiscard]] TBitBoard findOddThreats(TBitBoard moves);

  /**
   * @brief Replace the current position with the one described by @p board.
   *
   * @param board Column-major array using @c P_EMPTY / @c P_YELLOW / @c P_RED
   *              values. Stones must respect gravity (no floating cells).
   * @return @c true on success, @c false if @p board is malformed; the
   *         object is unchanged on failure.
   */
  [[nodiscard]] bool setBoard(const TBoardArray& board);

  /// Row-major variant of @c setBoard(const TBoardArray&).
  [[nodiscard]] bool setBoard(const TBoardArrayT& board);

  /**
   * @brief Replay a move sequence onto a fresh board and copy the result.
   *
   * @param moveSequence Sequence of column indices to play in order.
   * @return @c true on success, @c false if the sequence contains an illegal
   *         move; the object is unchanged on failure.
   */
  [[nodiscard]] bool setBoard(const std::vector<int>& moveSequence);

  /**
   * @brief Drop a stone of the player to move into @p column.
   * @param column Column index.
   * @return @c true if the move was legal and applied, @c false otherwise.
   */
  bool play(int column);
  /**
   * @brief Apply a sequence of column-index moves.
   * @param moveSequence Sequence of column indices.
   * @return @c true if every move was legal, @c false otherwise; on failure
   *         the board is left unchanged.
   */
  [[nodiscard]] bool play(const std::vector<int>& moveSequence);
  /**
   * @brief Apply a sequence of moves encoded as digit characters.
   *
   * Each character must be in `'0'..'0'+N_COLUMNS-1`.
   *
   * @param moveSequence String of column-index digits.
   * @return @c true on success, @c false on the first illegal move; on
   *         failure the board is left unchanged.
   */
  [[nodiscard]] bool play(const std::string& moveSequence);

  /// String-based variant of @ref setBoard(const std::vector<int>&).
  [[nodiscard]] bool setBoard(const std::string& moveSequence);

  /**
   * @brief Convert the bitboard representation into a column-major array.
   * @return 2D array of @c P_EMPTY / @c P_YELLOW / @c P_RED values.
   */
  [[nodiscard]] TBoardArray toArray() const;

  /**
   * @brief Validate a board layout.
   *
   * Checks that all cells contain a known value, that gravity is respected,
   * and that the stone count satisfies `0 <= yellow - red <= 1`.
   *
   * @param board Column-major array to validate.
   * @return @c true if @p board could appear in a real game.
   */
  [[nodiscard]] static bool isValid(const TBoardArray& board);

  /// @brief Does the player to move have an immediately winning move?
  [[nodiscard]] bool canWin() const;

  /**
   * @brief Variant of @ref canWin() restricted to a particular column.
   * @param column Column index to inspect.
   * @return @c true if dropping the next stone in @p column wins immediately.
   */
  [[nodiscard]] bool canWin(int column) const;

  /**
   * @brief Did the player who *just moved* form a four-in-a-row?
   * @return @c true if the position is a win for the player who is currently
   *         **not** to move.
   */
  [[nodiscard]] bool hasWin() const;

  /**
   * @brief Pretty-printed ASCII representation of the board.
   *
   * Yellow stones are rendered as `X`, Red stones as `O`, and empty cells as
   * `_`. Rows are printed from top to bottom.
   */
  [[nodiscard]] std::string toString() const;

  /// Number of plies remaining until the board is full.
  [[nodiscard]] inline TMovesCounter movesLeft() const { return m_movesLeft; }

  /// Number of stones currently placed on the board.
  [[nodiscard]] inline TMovesCounter countTokens() const {
    return N_ROWS * N_COLUMNS - m_movesLeft;
  }

  /**
   * @brief Mirror the board around its central column.
   *
   * Useful for transposition lookups: a position and its mirror image have
   * identical theoretical value, so storing only one of them roughly halves
   * the effective table size.
   */
  [[nodiscard]] Board mirror() const;

  /**
   * @brief Sort candidate moves into a @ref MoveList using a threat-based
   *        heuristic.
   *
   * Each move is scored by the number of new threats (winning lines)
   * it introduces, with a penalty for moves placed directly under one of the
   * active player's existing threats.
   *
   * @param moves Bitboard of candidate moves.
   * @return Priority queue ordered from best to worst.
   */
  [[nodiscard]] MoveList sortMoves(TBitBoard moves) const;

  /**
   * @brief Identify candidate moves that yield a tactical advantage.
   *
   * Returns either:
   *  - the first move that creates two simultaneous immediate threats (a
   *    forced win in two plies), or
   *  - the bit-OR of all moves that strictly increase the active player's
   *    number of threats.
   *
   * @param moves Candidate move bitboard.
   * @return Selected sub-set of @p moves.
   */
  TBitBoard findThreats(TBitBoard moves);

  /*
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ *,  *,  *,  *,  *,  *,  *]
   * [ 5, 14, 23, 32, 41, 50, 59],
   * [ 4, 13, 22, 31, 40, 49, 58],
   * [ 3, 12, 21, 30, 39, 48, 57],
   * [ 2, 11, 20, 29, 38, 47, 56],
   * [ 1, 10, 19, 28, 37, 46, 55],
   * [ 0,  9, 18, 27, 36, 45, 54]
   */
  /**
   * @brief Number of stones currently stacked in @p column.
   * @param column Column index (must be in 0&hellip;@c N_COLUMNS-1).
   * @return Stack height between 0 and @c N_ROWS.
   */
  [[nodiscard]] int getColumnHeight(const int column) const;

  /**
   * @brief Isolate the least significant set bit of a bitboard.
   * @param x Input bitboard.
   * @return @p x with all bits cleared except for its lowest set bit. If
   *         @p x is zero, the result is zero as well.
   */
  static inline TBitBoard lsb(const TBitBoard x) {
    const auto mvMask = x - UINT64_C(1);
    return ~mvMask & x;
  }

  /**
   * @brief Compute moves that do not lose immediately.
   *
   * Inspired by Pascal Pons' Connect-4 solver
   * (http://blog.gamesolver.org/). The function returns a bitboard of
   * legal moves whose execution does not let the opponent finish the game on
   * the next ply.
   *
   * @note If the active player has only losing replies, the returned
   *       bitboard is empty.
   * @note Direct winning moves are **not** included when the opponent has a
   *       double threat; callers that need them should add the result of
   *       @c winningPositions() explicitly.
   *
   * @return Bitboard of safe moves.
   */
  [[nodiscard]] TBitBoard generateNonLosingMoves() const {
    // Mostly inspired by Pascal's Code
    // This function might return an empty bitboard. In this case, the active
    // player will lose, since all possible moves will lead to a defeat.
    // NOTE: This function will not return immediate winning moves in those
    // cases where the opposing player has a double threat (or threat)
    TBitBoard moves = legalMovesMask();
    const TBitBoard threats =
        winningPositions(m_bActivePTokens ^ m_bAllTokens, true);
    if (const TBitBoard directThreats = threats & moves) {
      // no way we can neutralize more than one direct threat...
      moves = directThreats & (directThreats - 1) ? UINT64_C(0) : directThreats;
    }

    // No token under an opponent's threat.
    return moves & ~(threats >> 1);
  }

  /**
   * @brief Identify moves that create a "double threat" (forced win).
   *
   * A double threat is a stone that produces two immediate vertical threats
   * stacked on top of each other &mdash; the opponent can only block one of
   * them.
   *
   * @param moves Candidate move bitboard.
   * @return Sub-set of @p moves whose execution yields a double threat.
   */
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
  /**
   * @brief Encode the position as a compact Huffman-like integer.
   *
   * Used as a key for the opening-book tables. The encoding is only defined
   * for positions with an even number of tokens and at most 12 placed stones
   * (i.e.\ @c m_movesLeft is even and at least 30).
   *
   * Each column is encoded by walking from the bottom up: every stone
   * contributes two bits (`10` for the active player, `11` for the
   * opponent), and the stack is terminated by a single `0` bit.
   *
   * @return Huffman-encoded position, or @c 0 if the position does not
   *         qualify for encoding.
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

  /**
   * @brief Generate a random reachable position with a fixed number of plies.
   *
   * Plays random legal moves until @p nPly stones are on the board. If
   * @p forbidDirectWin is true the function reroutes around positions where
   * the side to move can win immediately (useful when generating benchmark
   * positions).
   *
   * @param nPly Number of plies to play (0&hellip;@c N_ROWS*N_COLUMNS).
   * @param forbidDirectWin Reject positions where the active player has an
   *                        immediate win.
   * @return Pair of the generated board and the move sequence used to reach
   *         it. Returns an empty pair if @p nPly is out of range.
   */
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

  /**
   * @brief Enumerate every reachable position from this board within a depth.
   *
   * Performs a recursive expansion via @c addAfterStates() and deduplicates
   * positions by their @ref uid().
   *
   * @param upToNPly Maximum number of plies to play from the current board.
   * @param exactlyN If @c true, return only positions with exactly
   *                 @p upToNPly stones placed; if @c false, include all
   *                 intermediate positions as well.
   * @return Vector of distinct boards.
   *
   * @see https://oeis.org/A212693
   */
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

  /**
   * @brief Plain-data snapshot of the board's internal state.
   *
   * Exposed for advanced use cases (Python bindings, serialisation) where
   * the caller needs direct access to the underlying bitboards.
   */
  struct RawState {
    TBitBoard all_tokens;      ///< Bitboard of all occupied cells.
    TBitBoard active_tokens;   ///< Bitboard of cells held by the side to move.
    TMovesCounter moves_left;  ///< Plies left until the board is full.
  };

  /// @brief Snapshot the internal state.
  [[nodiscard]] inline RawState rawState() const noexcept {
    return RawState{m_bAllTokens, m_bActivePTokens, m_movesLeft};
  }

  /**
   * @brief Restore an internal state previously captured with @ref rawState().
   *
   * @warning No validity checks are performed. Callers must ensure that the
   *          provided state corresponds to a real Connect-4 position;
   *          otherwise subsequent operations on the board are undefined.
   */
  inline void setRawState(const RawState& s) noexcept {
    m_bAllTokens = s.all_tokens;
    m_bActivePTokens = s.active_tokens;
    m_movesLeft = s.moves_left;
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
  TBitBoard m_bAllTokens;      ///< Bitboard of every occupied cell.
  TBitBoard m_bActivePTokens;  ///< Bitboard of cells held by the active player.
  TMovesCounter m_movesLeft;   ///< Plies remaining until the board is full.

  /**
   * @brief Compute the set of empty cells that would complete a four-in-a-row
   *        for the player whose stones are encoded in @p x.
   *
   * @param x         Bitboard with the player's stones set.
   * @param verticals If @c true, also generate vertical wins; pass @c false
   *                  to skip them when only horizontal/diagonal threats are
   *                  needed.
   * @return Bitboard of "completing" cells, restricted to the playable area.
   */
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

    auto moves = b.legalMovesMask();

    while (moves) {
      const auto mv = b.nextMove(moves);
      assert(uint64_t_popcnt(mv) == 1);
      if (auto newB = b.playBitMaskOnCopy(mv);
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

  std::vector<int> orderedLegalMovesFromMask(TBitBoard mvBits) const;

  std::vector<int> legalMovesFromMask(TBitBoard mvBits) const;
};

}  // namespace BitBully

#endif  // XBITBULLY__BOARD_H_
