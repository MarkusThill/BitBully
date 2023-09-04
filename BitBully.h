#ifndef BITBULLY__BITBULLY_H_
#define BITBULLY__BITBULLY_H_

#include "Board.h"
#include "TranspositionTable.h"
#include <iostream>
#include <numeric>
#include <vector>

namespace BitBully {
class BitBully {
private:
  static bool constexpr USE_TRANSPOSITION_TABLE = true;

public:
  TranspositionTable transpositionTable;

  BitBully() : transpositionTable{USE_TRANSPOSITION_TABLE ? 19 : 0} {};

  int negamax(Board b, int alpha, int beta) {
    assert(alpha < beta);
    if (b.canWin()) {
      return (b.movesLeft() + 1) / 2; // TODO: b.movesLeft() suffices I think
    }

    if (!b.movesLeft()) {
      assert(!b.generateMoves());
      assert(b.popCountBoard() == Board::N_COLUMNS * Board::N_ROWS);
      return 0;
    }

    int oldAlpha = alpha;

    // Transposition cutoff:
    TranspositionTable::Entry *ttEntry;
    if constexpr (USE_TRANSPOSITION_TABLE) {
      ttEntry = transpositionTable.get(b);
      if (ttEntry && ttEntry->b == b) {
        if (ttEntry->flag == TranspositionTable::Entry::EXACT) {
          return ttEntry->value;
        } else if (ttEntry->flag == TranspositionTable::Entry::LOWER) {
          alpha = std::max(alpha, ttEntry->value);
        } else if (ttEntry->flag == TranspositionTable::Entry::UPPER) {
          beta = std::min(beta, ttEntry->value);
        }
        if (alpha >= beta) {
          return ttEntry->value;
        }
      }
    }

    auto moves = b.generateMoves();
    assert(uint64_t_popcnt(moves) <= Board::N_COLUMNS);
    assert(uint64_t_popcnt(moves) > 0);

    int value = -(1 << 10);
    while (moves && alpha < beta) {
      auto mvMask = moves - UINT64_C(1);

      auto mv = ~mvMask & moves;
      assert(uint64_t_popcnt(mv) == 1);

      value = std::max(value, -negamax(b.playMoveOnCopy(mv), -beta, -alpha));
      alpha = std::max(alpha, value);

      moves &= mvMask;
    }

    if constexpr (USE_TRANSPOSITION_TABLE) {
      assert(ttEntry != nullptr);
      // Store node result in Transposition value
      ttEntry->b = b;
      ttEntry->value = value;
      if (value <= oldAlpha) {
        ttEntry->flag = TranspositionTable::Entry::UPPER;
      } else if (value >= beta) {
        ttEntry->flag = TranspositionTable::Entry::LOWER;
      } else {
        ttEntry->flag = TranspositionTable::Entry::EXACT;
      }
    }
    return value;
  }
};
} // namespace BitBully

#endif // BITBULLY__BITBULLY_H_
