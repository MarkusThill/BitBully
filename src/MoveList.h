/**
 * @file MoveList.h
 * @brief Tiny priority queue used by the engine for move ordering.
 */
#ifndef MOVELIST_H
#define MOVELIST_H
#include <cstdint>

// TODO: Definitions into cpp file
namespace BitBully {

/**
 * @class MoveList
 * @brief Fixed-capacity priority queue tailored for Connect-4 move ordering.
 *
 * The queue holds at most @c N_COLUMNS entries (one per column) using a
 * stack-allocated array. @ref insert() performs an insertion sort by
 * ascending score so that @ref pop() can return the highest-scoring entry in
 * @c O(1). When two entries share the same score the FIFO order of insertion
 * is preserved.
 *
 * @ingroup util
 */
class MoveList {
 public:
  /// 64-bit bitboard type (mirrors the alias defined in Board.h).
  // TODO: This is also defined in Board.h
  using TBitBoard = uint64_t;

  /// Maximum number of moves the queue can hold.
  // TODO: This also:
  static constexpr int N_COLUMNS = 7;

  /**
   * @brief Insert a (move, score) pair while keeping the queue sorted.
   * @param move  Bitboard with exactly one bit representing the move.
   * @param score Priority score; higher values are popped first.
   */
  void insert(const TBitBoard move, const int score) {
    int pos = size++;
    for (; pos && m_arrayPrioQueue[pos - 1].score >= score; --pos)
      m_arrayPrioQueue[pos] = m_arrayPrioQueue[pos - 1];
    m_arrayPrioQueue[pos].move = move;
    m_arrayPrioQueue[pos].score = score;
  }

  /**
   * @brief Remove and return the highest-priority move.
   * @return The bitboard mask of the move, or @c 0 if the queue is empty.
   */
  inline TBitBoard pop() {
    return size ? m_arrayPrioQueue[--size].move : UINT64_C(0);
  }

  /// @brief Number of moves currently stored.
  unsigned int getSize() const { return size; }

  /// @brief Empty the queue without freeing any memory.
  void reset() { size = 0; }

  /// @brief Construct an empty move list.
  MoveList() : size{0}, m_arrayPrioQueue{} {}

 private:
  /// @brief Number of stored moves.
  unsigned int size;

  /**
   * @brief Internal storage for the priority queue.
   *
   * Higher score corresponds to higher priority. For equal scores the FIFO
   * principle holds.
   */
  struct {
    TBitBoard move;  ///< Single-bit move mask.
    int score;       ///< Priority score.
  } m_arrayPrioQueue[N_COLUMNS];
};
}  // namespace BitBully

#endif  // MOVELIST_H
