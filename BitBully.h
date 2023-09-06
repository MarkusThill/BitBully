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

  BitBully() : transpositionTable{USE_TRANSPOSITION_TABLE ? 18 : 0} {};

  // TODO: firstGuess is a parameter which could be tuned!
  int mtdf(Board b, int firstGuess) {
    // MTD(f) algorithm by Aske Plaat: Plaat, Aske; Jonathan Schaeffer; Wim
    // Pijls; Arie de Bruin (November 1996). "Best-first Fixed-depth Minimax
    // Algorithms". Artificial Intelligence. 87 (1–2): 255–293.
    // doi:10.1016/0004-3702(95)00126-3
    auto g = firstGuess;
    int upperBound = 100000;
    int lowerBound = -100000;

    while (lowerBound < upperBound) {
      auto beta = std::max(g, lowerBound + 1);
      g = negamax(b, beta - 1, beta, 0);
      if (g < beta) {
        upperBound = g;
      } else {
        lowerBound = g;
      }
    }
    return g;
  }

  int negamax(Board b, int alpha, int beta, int depth) {
    assert(alpha < beta);
    if (b.canWin()) {
      return (b.movesLeft() + 1) / 2; // TODO: b.movesLeft() suffices I think
    }

    if (alpha >= (b.movesLeft() + 1) / 2) {
      // We cannot get better than this any more (with every additional move,
      // our potential score gets lower since we have a later win).
      return alpha;
    }

    if (!b.movesLeft()) {
      assert(!b.generateMoves());
      assert(b.popCountBoard() == Board::N_COLUMNS * Board::N_ROWS);
      return 0;
    }

    int oldAlpha = alpha;

    // Transposition cutoff: TODO: Pretty ugly...
    TranspositionTable::Entry *ttEntry = nullptr;
    if constexpr (USE_TRANSPOSITION_TABLE) {
      // TODO: Try later again. Checking mirrored positions seems to help a bit
      // for positions with less than 16 pieces...
      if (b.movesLeft() >= 4 && b.movesLeft() % 2 == 0) {
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
            return ttEntry->value; //
          }
        }
      } else if (depth < 22 && b.movesLeft() % 2) {
        // Enhanced Transposition Cutoff
        // TODO: Check, if this is correct
        auto etcMoves = b.generateMoves();
        while (etcMoves) {
          auto mv = b.nextMove(etcMoves);
          assert(uint64_t_popcnt(mv) == 1);
          auto bETC = b.playMoveOnCopy(mv);
          auto etcEntry = transpositionTable.get(bETC);
          /*if (etcEntry->b == bETC &&
              etcEntry->flag != TranspositionTable::Entry::LOWER &&
              etcEntry->value >= beta)
            return etcEntry->value;*/
          if (etcEntry->b == bETC &&
              etcEntry->flag != TranspositionTable::Entry::LOWER &&
              -etcEntry->value >= beta)
            return -etcEntry->value;

          etcMoves ^= mv;
        }
      }
      /*if (depth < 14) {
        auto bM = b.mirror();
        auto ttEntry2 = transpositionTable.get(bM);
        if (ttEntry2 && ttEntry2->b == bM) {
          if (ttEntry2->flag == TranspositionTable::Entry::EXACT) {
            return ttEntry2->value;
          } else if (ttEntry2->flag == TranspositionTable::Entry::LOWER) {
            alpha = std::max(alpha, ttEntry2->value);
          } else if (ttEntry2->flag == TranspositionTable::Entry::UPPER) {
            beta = std::min(beta, ttEntry2->value);
          }
          if (alpha >= beta) {
            return ttEntry2->value;
          }
        }
      } */
    }

    /*
    if (alpha >= (b.movesLeft() + 1) / 2) {
      // We cannot get better than this any more (with every additional move,
      // our potential score gets lower since we have a later win).
      return alpha;
    }
     */

    auto moves = b.generateNonLosingMoves();
    if (!moves) {
      return -b.movesLeft() / 2;
      // value = std::max(value, -b.movesLeft() / 2);
      // alpha = std::max(alpha, value);
    }

    if (depth < 20 && b.doubleThreat(moves)) {
      // value = std::max(value, (b.movesLeft() - 1) / 2);
      // alpha = std::max(alpha, value);
      return (b.movesLeft() - 1) / 2;
    }

    assert(uint64_t_popcnt(moves) <= Board::N_COLUMNS);
    assert(uint64_t_popcnt(moves) > 0);

    // TODO: depth parameter needs tuning...
    auto threats = depth < 22 ? b.findThreats(moves) : UINT64_C(0);
    assert((threats & moves) == threats);

    // auto movesFirst = b.removeMovesUnderOwnThreats(moves);
    int value = -(1 << 10);
    while (moves && alpha < beta) {
      // auto mvList = (movesFirst ? movesFirst : moves);
      auto mv = (threats ? b.nextMove(threats) : b.nextMove(moves));
      assert(uint64_t_popcnt(mv) == 1);
      auto moveValue = -negamax(b.playMoveOnCopy(mv), -beta, -alpha, depth + 1);
      value = std::max(value, moveValue);
      alpha = std::max(alpha, value);
      threats &= ~mv;
      // movesFirst &= ~mv;
      moves ^= mv;
    }
    /*while (silly && alpha < beta) {
      auto mv = b.nextMove(silly);
      assert(uint64_t_popcnt(mv) == 1);
      auto moveValue = -negamax(b.playMoveOnCopy(mv), -beta, -alpha, depth + 1);
      value = std::max(value, moveValue);
      alpha = std::max(alpha, value);
      silly ^= mv;
    }*/

    if constexpr (USE_TRANSPOSITION_TABLE) {

      if (!ttEntry)
        return value;
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
