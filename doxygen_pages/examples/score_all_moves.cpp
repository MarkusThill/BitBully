/**
 * @example score_all_moves.cpp
 *
 * Evaluate every legal column from the current position and print one
 * line per column: the column index and the perfect-play score that
 * results from playing it. Equivalent to what an analysis UI would do
 * to surface candidate moves to a human.
 */
#include <iostream>

#include "BitBully.h"

int main() {
  BitBully::Board b;
  b.play("4435");  // arbitrary mid-game position

  BitBully::BitBully solver;
  const auto scores = solver.scoreMoves(b);

  for (int col = 0; col < BitBully::Board::N_COLUMNS; ++col) {
    if (b.isLegalMove(col)) {
      std::cout << "col " << col << " -> " << scores[col] << '\n';
    }
  }
  return 0;
}
