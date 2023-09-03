#ifndef BITBULLY__BITBULLY_H_
#define BITBULLY__BITBULLY_H_

#include "Board.h"
#include <numeric>
#include <vector>

namespace BitBully {
class BitBully {
public:
  int negamax(Board b, int alpha, int beta) {
    assert(alpha < beta);
    if (b.hasWin()) {
      return (b.movesLeft() + 1) / 2;
    }

    if (!b.movesLeft()) {
      assert(!b.generateMoves());
      assert(b.popCountBoard() == Board::N_COLUMNS * Board::N_ROWS);
      return 0;
    }
    auto moves = b.generateMoves();
    assert(uint64_t_popcnt(moves) <= Board::N_COLUMNS);
    assert(uint64_t_popcnt(moves) > 0);

    int value = -(1 << 10);
    while (moves && alpha < beta) {
      auto mvMask = moves - UINT64_C(1);

      auto mv = ~mvMask & moves;
      assert(uint64_t_popcnt(mv) == 1);

      value =
          std::max(value, -negamax(b.playMoveFastonCopy(mv), -beta, -alpha));
      alpha = std::max(alpha, value);

      moves &= mvMask;
    }

    return value;
  }
};
} // namespace BitBully

#endif // BITBULLY__BITBULLY_H_
