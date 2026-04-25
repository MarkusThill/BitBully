/**
 * @example with_opening_book.cpp
 *
 * Demonstrates loading an 8-ply opening book at construction time and
 * letting the solver short-circuit early-game positions that are already
 * resolved in the book. If the book file is missing the example still
 * works; it just performs the full search.
 */
#include <filesystem>
#include <iostream>

#include "BitBully.h"

int main() {
  // Path is relative to wherever the binary is launched from.
  const std::filesystem::path bookPath = "openingbook_8ply.bin";

  BitBully::BitBully solver{bookPath};
  if (!solver.isBookLoaded()) {
    std::cerr << "(book not found, falling back to full search)\n";
  }

  BitBully::Board b;  // empty starting position
  const int score = solver.mtdf(b, /*firstGuess=*/0);
  std::cout << "score from start = " << score << '\n';
  return 0;
}
