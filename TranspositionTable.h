#ifndef BITBULLY__TRANSPOSITIONTABLE_H_
#define BITBULLY__TRANSPOSITIONTABLE_H_

#include "Board.h"
#include <memory>

namespace BitBully {

class TranspositionTable {
public:
  static constexpr int LOG_2_SIZE = 20;

  struct Entry {
    enum NodeType { EXACT, LOWER, UPPER };
    Board b;
    NodeType flag;
    int value;
  };

  TranspositionTable(int log_2_size = LOG_2_SIZE) {
    tableSize = 1 << log_2_size;
    table = std::make_unique<Entry[]>(tableSize);
  }

  Entry *get(Board b) { // TODO: pass by reference ???
    return &table[b.hash() & (tableSize - 1)];
  }

private:
  std::unique_ptr<Entry[]> table;
  size_t tableSize;
};

} // namespace BitBully

#endif // BITBULLY__TRANSPOSITIONTABLE_H_
