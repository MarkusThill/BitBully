#ifndef MOVELIST_H
#define MOVELIST_H
#include <cstdint>

// TODO: Definitions into cpp file
namespace BitBully {

// A simple priority queue.
class MoveList {
 public:
  // TODO: This is also defined in Board.h
  using TBitBoard = uint64_t;

  // TODO: This also:
  static constexpr int N_COLUMNS = 7;

  void insert(const TBitBoard move, const int score) {
    int pos = size++;
    for (; pos && m_arrayPrioQueue[pos - 1].score >= score; --pos)
      m_arrayPrioQueue[pos] = m_arrayPrioQueue[pos - 1];
    m_arrayPrioQueue[pos].move = move;
    m_arrayPrioQueue[pos].score = score;
  }

  inline TBitBoard pop() {
    return size ? m_arrayPrioQueue[--size].move : UINT64_C(0);
  }

  void reset() { size = 0; }

  MoveList() : size{0}, m_arrayPrioQueue{} {}

 private:
  // number of stored moves
  unsigned int size;

  // An array-based priority queue containing a list of moves, where a higher
  // score corresponds to a higher prio. In case two or more scores are equal,
  // the FIFO principle holds
  struct {
    TBitBoard move;
    int score;
  } m_arrayPrioQueue[N_COLUMNS];
};
}  // namespace BitBully

#endif  // MOVELIST_H
