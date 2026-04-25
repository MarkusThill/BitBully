/**
 * @file BitBully.h
 * @brief Connect-4 search engine that operates on @ref BitBully::Board.
 *
 * Provides @ref BitBully::BitBully, a perfect-play solver built around an
 * alpha-beta negamax. Notable features:
 *  - move ordering via @ref BitBully::Board::sortMoves(),
 *  - a transposition table with EXACT / LOWER / UPPER bounds and an
 *    Enhanced-Transposition-Cutoff (ETC) variant,
 *  - mirror-symmetry lookups,
 *  - optional opening-book consultation (8-ply or 12-ply),
 *  - MTD(f) and binary-search (null-window) drivers on top of the negamax.
 */
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
/**
 * @brief Perfect-play Connect-4 solver.
 *
 * Maintains a transposition table and (optionally) an opening book between
 * searches so that subsequent calls reuse previously computed information.
 * Scores returned by the engine follow the convention introduced by Pascal
 * Pons:
 *
 *  - a positive score @c s means the player to move wins on ply
 *    `2 * s - 1` from the current position,
 *  - a negative score @c s means the player to move loses on ply `2 * |s|`,
 *  - a score of @c 0 indicates a draw.
 *
 * The helper @c scoreToMovesLeft converts these compact scores back into
 * the actual number of plies remaining.
 */
class BitBully {
 private:
  unsigned long long int nodeCounter;  ///< Number of nodes visited.
  /// Compile-time switch enabling the transposition table.
  static bool constexpr USE_TRANSPOSITION_TABLE = true;
  /// Default size of the transposition table, expressed as `log2(entries)`.
  static auto constexpr DEFAULT_LOG_TRANSPOSITION_SIZE = 22;

  TranspositionTable transpositionTable;       ///< Backing transposition table.
  std::unique_ptr<OpeningBook> m_openingBook;  ///< Optional opening book.

 public:
  // MoveList sortMoves(Board::TBitBoard moves); // implemented in Board.cpp

  /**
   * @brief Construct a solver, optionally loading an opening book.
   * @param bookPath Path to a binary opening-book file. If empty, no book is
   *                 loaded and @ref isBookLoaded() returns @c false.
   */
  explicit BitBully(const std::filesystem::path& bookPath = "")
      : nodeCounter{0},
        transpositionTable{
            USE_TRANSPOSITION_TABLE ? DEFAULT_LOG_TRANSPOSITION_SIZE : 0} {
    loadBook(bookPath);  // will not do anything if path is empty
  };

  /// @brief Was an opening book successfully loaded?
  inline bool isBookLoaded() const { return m_openingBook != nullptr; }

  /// @brief Discard the currently loaded opening book (if any).
  inline void resetBook() { m_openingBook.reset(); }

  /**
   * @brief Load an opening book from disk.
   *
   * The database type (8-ply vs.\ 12-ply, with or without distance
   * information) is inferred from the file size.
   *
   * @param bookPath Path to the book file. Empty paths are a no-op.
   * @return @c true if a new book is now loaded; @c false if a book was
   *         already loaded or @p bookPath is empty.
   */
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

  /// Convert a solver score to the number of moves until the game ends.
  ///
  /// @param score The solver score (positive = current player wins,
  ///              negative = current player loses, 0 = draw).
  /// @param b     The board state for which the score was computed.
  /// @return Number of moves until the game concludes under perfect play.
  static int scoreToMovesLeft(const int score, const Board& b) noexcept {
    if (score == 0) {
      return b.movesLeft();
    }
    const int p = (b.movesLeft() + 1) % 2;  // 1 -> yellow, 0 -> red
    const int sgnScore = score < 0 ? 1 : 0;
    const int absScore = score < 0 ? -score : score;
    const int mvFinalLeft = 2 * (absScore - 1) + (sgnScore ^ p);
    return b.movesLeft() - mvFinalLeft;
  }

  /**
   * @brief Cheap evaluation: play out the game using safe heuristic moves.
   *
   * Both players use @ref Board::generateNonLosingMoves() to avoid immediate
   * losses; among multiple non-losing moves the centre-priority heuristic
   * @ref Board::nextMove() picks the next ply. The function terminates as
   * soon as a side reaches a winning position or runs out of non-losing
   * replies.
   *
   * @param b Position to evaluate. Passed by value because the rollout
   *          mutates a local copy.
   * @return Solver score from the perspective of the side to move in @p b.
   */
  static int rollout(Board b) noexcept {
    int ply = 0;

    while (true) {
      if (b.canWin()) {
        const int score = (b.movesLeft() + 1) / 2;
        return (ply % 2 == 0) ? score : -score;
      }

      if (!b.movesLeft()) {
        return 0;
      }

      auto moves = b.generateNonLosingMoves();
      if (!moves) {
        const int score = -(b.movesLeft() / 2);
        return (ply % 2 == 0) ? score : -score;
      }

      const auto mv = Board::nextMove(moves);

      // TODO: probably faster to do the move directly on the current
      // board instead of copying it first and then doing the move on the copy.
      // However, this would require some changes to the Board class (e.g., a
      // playMoveFast() method which does not check for legality of the move
      // since we already know that it is legal).
      b = b.playBitMaskOnCopy(mv);
      ply++;
    }
  }

  /**
   * @brief Solve a position using the MTD(f) driver.
   *
   * MTD(f) repeatedly invokes @ref negamax() with zero-width
   * (`[beta-1, beta]`) windows, tightening the upper and lower bounds until
   * they meet. The @p firstGuess parameter seeds the initial test value: a
   * good guess (e.g.\ from a previous shallow search) drastically reduces
   * the number of re-searches.
   *
   * @param b          Position to evaluate.
   * @param firstGuess Initial guess for the position's score.
   * @param maxDepth   Maximum search depth in plies, or @c -1 for unlimited
   *                   search (full perfect-play resolution).
   * @return Exact score of @p b (or a depth-limited approximation when
   *         @p maxDepth &ge; 0).
   *
   * @par Reference
   * Plaat, Schaeffer, Pijls and de Bruin, "Best-first Fixed-depth Minimax
   * Algorithms", Artificial Intelligence 87 (1996), 255&ndash;293.
   */
  int mtdf(const Board& b, const int firstGuess,
           const int maxDepth = -1) noexcept {
    // MTD(f) algorithm by Aske Plaat: Plaat, Aske; Jonathan Schaeffer; Wim
    // Pijls; Arie de Bruin (November 1996). "Best-first Fixed-depth Minimax
    // Algorithms". Artificial Intelligence. 87 (1–2): 255–293.
    // doi:10.1016/0004-3702(95)00126-3
    auto g = firstGuess;
    int upperBound = INT32_MAX;
    int lowerBound = INT32_MIN;

    while (lowerBound < upperBound) {
      const auto beta = std::max(g, lowerBound + 1);
      g = negamax(b, beta - 1, beta, 0, maxDepth);
      if (g < beta) {
        upperBound = g;
      } else {
        lowerBound = g;
      }
    }
    return g;
  }

  /**
   * @brief Alternative driver based on a binary search of zero-width windows.
   *
   * Empirically slower than @ref mtdf() in this codebase but useful for
   * cross-checking results.
   *
   * @param b        Position to evaluate.
   * @param maxDepth Maximum search depth in plies, or @c -1 for unlimited
   *                 search.
   * @return Exact score of @p b.
   */
  int nullWindow(const Board& b, const int maxDepth = -1) noexcept {
    int min = -b.movesLeft() / 2;
    int max = (b.movesLeft() + 1) / 2;

    while (min < max) {
      int mid = min + (max - min) / 2;
      if (mid <= 0 && min / 2 < mid)
        mid = min / 2;
      else if (mid >= 0 && max / 2 > mid)
        mid = max / 2;
      int r = negamax(b, mid, mid + 1, 0, maxDepth);
      if (r <= mid) {
        max = r;
      } else {
        min = r;
      }
    }
    return min;
  }

  /// @brief Discard the contents of the transposition table.
  void resetTranspositionTable() {
    transpositionTable = TranspositionTable{
        USE_TRANSPOSITION_TABLE ? DEFAULT_LOG_TRANSPOSITION_SIZE : 0};
  }

  /// @brief Number of nodes visited since @ref resetNodeCounter().
  [[nodiscard]] auto getNodeCounter() const { return nodeCounter; }

  /// @brief Reset the visited-node counter to zero.
  void resetNodeCounter() { nodeCounter = 0ULL; }

  /**
   * @brief Negamax search with alpha-beta pruning, transposition table and
   *        opening book consultation.
   *
   * The function combines several enhancements:
   *  - move ordering through @ref Board::sortMoves(),
   *  - transposition cutoffs (EXACT / LOWER / UPPER) keyed on the position's
   *    @ref Board::uid(),
   *  - Enhanced Transposition Cutoffs (ETC) at low depths,
   *  - mirror-symmetry lookups,
   *  - opening-book consultation when the configured ply depth is reached,
   *  - depth-limited rollouts via @ref rollout() once @p maxDepth is hit.
   *
   * @param b        Position to evaluate (passed by value &mdash; recursion
   *                 mutates a local copy).
   * @param alpha    Current lower bound of the search window.
   * @param beta     Current upper bound of the search window.
   * @param depth    Plies played from the search root.
   * @param maxDepth Maximum total search depth, or @c -1 for unlimited.
   * @return Score of @p b in the active player's perspective.
   */
  int negamax(Board b, int alpha, int beta, const int depth,
              const int maxDepth = -1) noexcept {
    // In several aspects inspired by Pascal's code
    assert(alpha < beta);
    nodeCounter++;

    if (maxDepth >= 0 && depth >= maxDepth) {
      return rollout(b);
    }

    const int8_t remainingBudget =
        (maxDepth < 0) ? INT8_MAX : static_cast<int8_t>(maxDepth - depth);

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
      assert(!b.legalMovesMask());
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
        if (ttEntry && ttEntry->b == b.uid() &&
            ttEntry->searchDepth >= remainingBudget) {
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
        auto etcMoves = b.legalMovesMask();
        while (etcMoves) {
          auto mv = b.nextMove(etcMoves);
          assert(uint64_t_popcnt(mv) == 1);
          auto bETC = b.playBitMaskOnCopy(mv);
          auto etcEntry = transpositionTable.get(bETC);

          if (etcEntry->b == bETC.uid() &&
              etcEntry->searchDepth >= remainingBudget &&
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
        if (ttEntryMirror && ttEntryMirror->b == bMirror.uid() &&
            ttEntryMirror->searchDepth >= remainingBudget) {
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
        auto moveValue = -negamax(b.playBitMaskOnCopy(mv), -beta, -alpha,
                                  depth + 1, maxDepth);
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
        auto moveValue = -negamax(b.playBitMaskOnCopy(mv), -beta, -alpha,
                                  depth + 1, maxDepth);
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
      ttEntry->searchDepth = remainingBudget;

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

  /**
   * @brief Evaluate the move that drops a stone in @p column.
   *
   * Plays the move on a copy of @p b and runs @ref mtdf() on the resulting
   * position, then negates the score so that it is reported from the
   * perspective of the player to move in @p b.
   *
   * @param b          Current position.
   * @param column     Column index of the move to evaluate.
   * @param firstGuess Seed value for MTD(f).
   * @param maxDepth   Search depth limit, or @c -1 for unlimited search.
   * @return Score of the resulting position, or @c -1000 if @p column is
   *         not a legal move.
   */
  auto scoreMove(const Board& b, const int column, const int firstGuess,
                 const int maxDepth = -1) {
    int score = -1000;
    if (auto afterB = b; afterB.play(column)) {
      if (afterB.hasWin()) {
        return (afterB.movesLeft()) / 2 + 1;
      }
      // TODO: Get first guess from hash table if possible
      score = -mtdf(afterB, firstGuess, maxDepth);
    }
    return score;
  }

  /**
   * @brief Evaluate every column move from @p b.
   *
   * Calls @ref scoreMove() for each of the @c N_COLUMNS columns. Illegal
   * moves are reported with the sentinel score @c -1000.
   *
   * @param b        Current position.
   * @param maxDepth Search depth limit, or @c -1 for unlimited search.
   * @return Per-column scores indexed by column number.
   */
  auto scoreMoves(const Board& b, const int maxDepth = -1) {
    std::vector scores(Board::N_COLUMNS, -1000);
    for (auto col = 0UL; col < scores.size(); col++) {
      /*
      if (auto afterB = b; afterB.play(col)) {
        if (afterB.hasWin()) {
          scores[col] = (afterB.movesLeft()) / 2 + 1;
          continue;
        }
        // TODO: Get first guess from hash table if possible
        scores[col] = -mtdf(afterB, !col ? 0 : scores.at(col - 1));
      }
      */
      // TODO: Get first guess from hash table if possible
      scores[col] = scoreMove(b, static_cast<int>(col),
                              !col ? 0 : scores.at(col - 1), maxDepth);
    }

    return scores;
  }
};  // class BitBully
}  // namespace BitBully

#endif  // BITBULLY__BITBULLY_H_
