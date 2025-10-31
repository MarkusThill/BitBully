#ifndef BITBULLY__BITBULLY_H_
#define BITBULLY__BITBULLY_H_

#include <filesystem>
#include <iostream>
#include <vector>

#include "Board.h"
#include "MoveList.h"
#include "OpeningBook.h"
#include "TranspositionTable.h"

namespace BitBully {
class BitBully {
 private:
  unsigned long long int nodeCounter;
  static bool constexpr USE_TRANSPOSITION_TABLE = true;
  static auto constexpr DEFAULT_LOG_TRANSPOSITION_SIZE = 22;

  TranspositionTable transpositionTable;
  std::unique_ptr<OpeningBook> m_openingBook;

 public:
  MoveList sortMoves(Board::TBitBoard moves);

  explicit BitBully(const std::filesystem::path& bookPath = "")
      : nodeCounter{0},
        transpositionTable{
            USE_TRANSPOSITION_TABLE ? DEFAULT_LOG_TRANSPOSITION_SIZE : 0} {
    loadBook(bookPath);  // will not do anything if path is empty
  };

  inline bool isBookLoaded() const { return m_openingBook != nullptr; }

  inline void resetBook() { m_openingBook.reset(); }

  inline bool loadBook(const std::filesystem::path& bookPath = "") {
    if (isBookLoaded()) {
      return false;
    }
    if (!bookPath.empty()) {
      m_openingBook = std::make_unique<OpeningBook>(bookPath);
      assert(isBookLoaded());
    }
    return isBookLoaded();
  }

  // TODO: firstGuess is a parameter which could be tuned!
  int mtdf(const Board& b, const int firstGuess) noexcept {
    // MTD(f) algorithm by Aske Plaat: Plaat, Aske; Jonathan Schaeffer; Wim
    // Pijls; Arie de Bruin (November 1996). "Best-first Fixed-depth Minimax
    // Algorithms". Artificial Intelligence. 87 (1–2): 255–293.
    // doi:10.1016/0004-3702(95)00126-3
    auto g = firstGuess;
    int upperBound = INT32_MAX;
    int lowerBound = INT32_MIN;

    while (lowerBound < upperBound) {
      const auto beta = std::max(g, lowerBound + 1);
      g = negamax(b, beta - 1, beta, 0);
      if (g < beta) {
        upperBound = g;
      } else {
        lowerBound = g;
      }
    }
    return g;
  }

  // generally, appears to be slower than mtdf
  int nullWindow(const Board& b) noexcept {
    int min = -b.movesLeft() / 2;
    int max = (b.movesLeft() + 1) / 2;

    while (min < max) {
      int mid = min + (max - min) / 2;
      if (mid <= 0 && min / 2 < mid)
        mid = min / 2;
      else if (mid >= 0 && max / 2 > mid)
        mid = max / 2;
      int r = negamax(b, mid, mid + 1, 0);
      if (r <= mid) {
        max = r;
      } else {
        min = r;
      }
    }
    return min;
  }

  void resetTranspositionTable() {
    transpositionTable = TranspositionTable{
        USE_TRANSPOSITION_TABLE ? DEFAULT_LOG_TRANSPOSITION_SIZE : 0};
  }

  [[nodiscard]] auto getNodeCounter() const { return nodeCounter; }

  void resetNodeCounter() { nodeCounter = 0ULL; }

  int negamax(Board b, int alpha, int beta, const int depth) noexcept {
    // In many aspects inspired by Pascal's code
    assert(alpha < beta);
    nodeCounter++;

    if (isBookLoaded() && b.countTokens() == m_openingBook->getNPly()) {
      return m_openingBook->getBoardValue(b);
    }

    // It appears as if this check is not necessary. Below we check, if we
    // have any non-losing moves left. If not, we return with a negative
    // score.
    // TODO: move this outside negamax:
    if (!depth && b.canWin()) {
      return (b.movesLeft() + 1) / 2;
    }

    if (alpha >= (b.movesLeft() + 1) / 2) {
      // We cannot get better than this (alpha) anymore (with every additional
      // move, our potential score gets lower since we have a later win).
      return alpha;
    }

    // lower bound of score as opponent cannot win next move:
    if (const int min = -b.movesLeft() / 2; alpha < min) {
      alpha = min;
      if (alpha >= beta) return alpha;
    }
    if (const int max = (b.movesLeft() - 1) / 2; beta > max) {
      beta = max;
      if (alpha >= beta) return beta;
    }

    if (!b.movesLeft()) {
      assert(!b.generateMoves());
      assert(b.popCountBoard() == Board::N_COLUMNS * Board::N_ROWS);
      return 0;
    }

    int oldAlpha = alpha;

    auto moves = b.generateNonLosingMoves();
    if (!moves) {
      return -b.movesLeft() / 2;
    }

    assert(uint64_t_popcnt(moves) <= Board::N_COLUMNS);
    assert(uint64_t_popcnt(moves) > 0);

    if (depth < 20 && b.doubleThreat(moves)) {
      return (b.movesLeft() - 1) / 2;
    }

    // Transposition cutoff: TODO: Pretty ugly...
    TranspositionTable::Entry* ttEntry = nullptr;
    if constexpr (USE_TRANSPOSITION_TABLE) {
      if (b.movesLeft() > 6 && b.movesLeft() % 2 == 0) {
        ttEntry = transpositionTable.get(b);
        if (ttEntry && ttEntry->b == b.uid()) {
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
      // Enhanced Transposition Cutoff
      else if (depth < 22 && b.movesLeft() % 2) {
        auto etcMoves = b.generateMoves();
        while (etcMoves) {
          auto mv = b.nextMove(etcMoves);
          assert(uint64_t_popcnt(mv) == 1);
          auto bETC = b.playMoveOnCopy(mv);
          auto etcEntry = transpositionTable.get(bETC);

          if (etcEntry->b == bETC.uid() &&
              etcEntry->flag != TranspositionTable::Entry::LOWER &&
              -etcEntry->value >= beta) {
            return -etcEntry->value;
          }

          etcMoves ^= mv;
        }
      }

      // Check symmetric positions
      // Symmetries get rare at some point in the game, so do not check them
      // on almost-full boards
      if (b.movesLeft() > 20) {
        const auto bMirror = b.mirror();
        auto ttEntryMirror = transpositionTable.get(bMirror);
        if (ttEntryMirror && ttEntryMirror->b == bMirror.uid()) {
          if (ttEntryMirror->flag == TranspositionTable::Entry::EXACT) {
            return ttEntryMirror->value;
          } else if (ttEntryMirror->flag == TranspositionTable::Entry::LOWER) {
            alpha = std::max(alpha, ttEntryMirror->value);
          } else if (ttEntryMirror->flag == TranspositionTable::Entry::UPPER) {
            beta = std::min(beta, ttEntryMirror->value);
          }
          if (alpha >= beta) {
            return ttEntryMirror->value;
          }
        }
      }
    }

    /*
    if (alpha >= (b.movesLeft() + 1) / 2) {
      // We cannot get better than this any more (with every additional move,
      // our potential score gets lower since we have a later win).
      return alpha;
    }
     */

    int value = -(1 << 10);
    if (depth < 20) {
      auto mvList = b.sortMoves(moves);

      // while (const auto mv = mvList.getNext() && alpha < beta) {
      auto mv = mvList.pop();
      for (; mv && alpha < beta; mv = mvList.pop()) {
        // const auto mv = (threats ? b.nextMove(threats) : b.nextMove(moves));
        assert(uint64_t_popcnt(mv) == 1);
        auto moveValue =
            -negamax(b.playMoveOnCopy(mv), -beta, -alpha, depth + 1);
        value = std::max(value, moveValue);
        alpha = std::max(alpha, value);
      }
    } else {
      auto threats = depth < 22 ? b.findThreats(moves) : UINT64_C(0);
      assert((threats & moves) == threats);

      // int value = -(1 << 10);
      while (moves && alpha < beta) {
        // auto mvList = (movesFirst ? movesFirst : moves);
        const auto mv = (threats ? b.nextMove(threats) : b.nextMove(moves));
        assert(uint64_t_popcnt(mv) == 1);
        auto moveValue =
            -negamax(b.playMoveOnCopy(mv), -beta, -alpha, depth + 1);
        value = std::max(value, moveValue);
        alpha = std::max(alpha, value);
        threats &= ~mv;
        moves ^= mv;
      }
    }

    if constexpr (USE_TRANSPOSITION_TABLE) {
      if (!ttEntry) return value;
      assert(ttEntry != nullptr);
      // Do not allow high-depth nodes to override low-depth nodes (low-depth
      // nodes achieve higher cut-offs): Does not help!
      // if ( ttEntry->flag == TranspositionTable::Entry::EXACT &&
      // ttEntry->b.movesLeft() < 42 &&
      // ttEntry->b.movesLeft() > b.movesLeft() + 16)
      // return value;

      //    Store node result in Transposition value
      ttEntry->b = b.uid();
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

  auto scoreMoves(const Board& b) {
    std::vector scores(Board::N_COLUMNS, -1000);
    for (auto col = 0UL; col < scores.size(); col++) {
      if (auto afterB = b; afterB.play(col)) {
        if (afterB.hasWin()) {
          scores[col] = (afterB.movesLeft()) / 2 + 1;
          continue;
        }
        // TODO: Get first guess from hash table if possible
        scores[col] = -mtdf(afterB, !col ? 0 : scores.at(col - 1));
      }
    }

    return scores;
  }
};
}  // namespace BitBully

#endif  // BITBULLY__BITBULLY_H_
