/**
 * @example quick_solve.cpp
 *
 * Minimal driver: build a position from a column-index move sequence,
 * solve it with MTD(f), and print the resulting score and the number of
 * plies until the game ends.
 *
 * Compile against the library and run with no arguments &mdash; the moves
 * `4435` are hard-coded for illustration.
 */
#include <iostream>

#include "BitBully.h"

int main() {
  BitBully::Board b;
  if (!b.play("4435")) {
    std::cerr << "Illegal move sequence\n";
    return 1;
  }

  BitBully::BitBully solver;
  const int score = solver.mtdf(b, /*firstGuess=*/0);
  const int plies = BitBully::BitBully::scoreToMovesLeft(score, b);

  std::cout << b.toString();
  std::cout << "score = " << score << "  (plies until end = " << plies << ")\n";
  return 0;
}
