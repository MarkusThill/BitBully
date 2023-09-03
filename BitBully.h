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
    
    std::vector<int> app{0, 1, 2, 3, 4, 5, 6};
    assert(uint64_t_popcnt(moves) == b.isLegalMove(0) + b.isLegalMove(1) +
                                         b.isLegalMove(2) + b.isLegalMove(3) +
                                         b.isLegalMove(4) + b.isLegalMove(5) +
                                         b.isLegalMove(6));

    // int counter = 0, exp = uint64_t_popcnt(moves); // DEBUG...
    int value = -(1 << 17);
    while (moves && alpha < beta) {
      auto mvMask = moves - UINT64_C(1);

      // auto newB = b;
      auto mv = ~mvMask & moves;
      assert(uint64_t_popcnt(mv) == 1);

      value =
          std::max(value, -negamax(b.playMoveFastonCopy(mv), -beta, -alpha));
      alpha = std::max(alpha, value);

      moves &= mvMask;
      // counter++;
    }

    // assert(exp == counter);

    /*
    int value = -(1 << 17);
    while (moves && alpha < beta) {
      auto mvMask = moves - UINT64_C(1);
      value = std::max(
          value, -negamax(b.playMoveFastBB(moves & ~mvMask), -beta,
   -alpha)); alpha = std::max(alpha, value); moves &= mvMask;
    }
*/

    /*
    value = -(1 << 17);
    for (int i = 0; i < 7 && alpha < beta; ++i) {
      if (b.isLegalMove(i)) {
        auto newB = b;
        assert(newB.playMove(i));
        value = std::max(value, -negamax(newB, -beta, -alpha));
        alpha = std::max(alpha, value);
      }
    }
*/
    return value;
  }
};
} // namespace BitBully

#endif // BITBULLY__BITBULLY_H_
