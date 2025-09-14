#ifndef BITBULLY__TRANSPOSITIONTABLE_H_
#define BITBULLY__TRANSPOSITIONTABLE_H_

#include <memory>

#include "Board.h"

namespace BitBully {

class TranspositionTable {
 public:
  // hash tables of size 2^n allow fast modulo operations since
  // x mod 2^n = x & (2^n - 1)
  // TODO: compute the effect of the hash table size on the long-term perf. of
  // the BitBully solver
  static constexpr int LOG_2_SIZE = 20;

  struct Entry {
    enum NodeType { NONE = 0, EXACT = 1, LOWER = 2, UPPER = 3 };
    uint64_t b;  // TODO: There should be a global type TBitboard
    NodeType flag{NONE};
    int value;
  };

  TranspositionTable(const int log_2_size = LOG_2_SIZE) {
    tableSize = UINT64_C(1) << log_2_size;
    table = std::make_unique<Entry[]>(tableSize);
  }

  inline Entry* get(const Board& b) {
    // Prefetching?:
    // size_t index = b.hash() & (tableSize - 1);
    // __builtin_prefetch(&table[index]);  // GCC/Clang prefetching
    // return &table[index];

    return &table[b.hash() & (tableSize - 1)];
  }

 private:
  std::unique_ptr<Entry[]> table;
  size_t tableSize;
};

}  // namespace BitBully

#endif  // BITBULLY__TRANSPOSITIONTABLE_H_
