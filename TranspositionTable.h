#ifndef BITBULLY__TRANSPOSITIONTABLE_H_
#define BITBULLY__TRANSPOSITIONTABLE_H_

#include <memory>

#include "Board.h"

namespace BitBully {

class TranspositionTable {
 public:
  static constexpr int LOG_2_SIZE = 20;

  struct Entry {
    enum NodeType { NONE = 0, EXACT = 1, LOWER = 2, UPPER = 3 };
    Board b{};
    NodeType flag{NONE};
    int value;
  };

  TranspositionTable(int log_2_size = LOG_2_SIZE) {
    tableSize = UINT64_C(1) << log_2_size;
    table = std::make_unique<Entry[]>(tableSize);
  }

  inline Entry *get(Board b) { return &table[b.hash() & (tableSize - 1)]; }

 private:
  std::unique_ptr<Entry[]> table;
  size_t tableSize;
};

}  // namespace BitBully

#endif  // BITBULLY__TRANSPOSITIONTABLE_H_
