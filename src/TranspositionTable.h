/**
 * @file TranspositionTable.h
 * @brief Direct-mapped, replacement-free transposition table used by the
 * solver.
 */
#ifndef BITBULLY__TRANSPOSITIONTABLE_H_
#define BITBULLY__TRANSPOSITIONTABLE_H_

#include <memory>

#include "Board.h"

namespace BitBully {

/**
 * @class TranspositionTable
 * @brief Power-of-two-sized direct-mapped cache for negamax search results.
 *
 * The table is indexed by `Board::hash() & (size - 1)`, which compiles down
 * to a single masking operation. A single slot is reserved per index;
 * collisions are simply overwritten. Each slot stores the Board's
 * @ref Board::uid() so that callers can recognise hash collisions.
 */
class TranspositionTable {
 public:
  /// Default `log2` of the number of entries.
  // hash tables of size 2^n allow fast modulo operations since
  // x mod 2^n = x & (2^n - 1)
  // TODO: compute the effect of the hash table size on the long-term perf. of
  // the BitBully solver
  static constexpr int LOG_2_SIZE = 20;

  /**
   * @brief One transposition-table slot.
   */
  struct Entry {
    /**
     * @brief Type of the bound stored in @ref value.
     */
    enum NodeType {
      NONE = 0,   ///< Empty slot.
      EXACT = 1,  ///< Exact score (PV node).
      LOWER = 2,  ///< Lower bound (fail-high cut node).
      UPPER = 3   ///< Upper bound (fail-low all node).
    };
    uint64_t b;           ///< Position UID (see @ref Board::uid()).
    NodeType flag{NONE};  ///< Type of bound stored in @ref value.
    int value;            ///< Cached search score.
    /**
     * @brief Remaining search budget when the entry was stored.
     *
     * `INT8_MAX` denotes an unlimited (full) search.
     */
    int8_t searchDepth{0};
  };

  /**
   * @brief Allocate a transposition table with @c 2^log_2_size entries.
   * @param log_2_size Log-2 of the number of slots. Use @c 0 to obtain a
   *                   single-slot table.
   */
  TranspositionTable(const int log_2_size = LOG_2_SIZE) {
    tableSize = UINT64_C(1) << log_2_size;
    table = std::make_unique<Entry[]>(tableSize);
  }

  /**
   * @brief Get the slot associated with @p b.
   *
   * The slot is shared with every position that hashes to the same index;
   * callers must inspect @ref Entry::b to verify a hit.
   *
   * @param b Position to look up.
   * @return Pointer to the slot. Always non-null and valid for the lifetime
   *         of the @c TranspositionTable.
   */
  inline Entry* get(const Board& b) {
    // Prefetching?:
    // size_t index = b.hash() & (tableSize - 1);
    // __builtin_prefetch(&table[index]);  // GCC/Clang prefetching
    // return &table[index];

    return &table[b.hash() & (tableSize - 1)];
  }

 private:
  std::unique_ptr<Entry[]> table;  ///< Backing storage of @c tableSize slots.
  size_t tableSize;                ///< Number of slots (always a power of two).
};

}  // namespace BitBully

#endif  // BITBULLY__TRANSPOSITIONTABLE_H_
