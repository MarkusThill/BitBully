# BitBully — Connect-4 Engine {#mainpage}

[TOC]

**BitBully** is a high-performance perfect-play Connect-4 solver written in
modern C++. It reaches every win/loss/draw verdict for any position by
combining a bitboard representation, a hand-tuned alpha-beta negamax, a
transposition table and pre-computed opening books.

This documentation covers the **C++ core** of the project.

- For Python usage and tutorials see the
  [main BitBully docs](../../index.html).
- For the source repository, visit the
  [BitBully GitHub project](https://github.com/MarkusThill/BitBully).

\image html bitbully-logo-full.png "BitBully" width=420px

---

## Library at a glance

| Component                                | Header                                          | Purpose                                      |
| ---------------------------------------- | ----------------------------------------------- | -------------------------------------------- |
| @ref BitBully::Board                     | [Board.h](Board_8h.html)                        | Bitboard position with legal-move generation |
| @ref BitBully::BitBully                  | [BitBully.h](BitBully_8h.html)                  | Negamax + MTD(f) solver driver               |
| @ref BitBully::TranspositionTable        | [TranspositionTable.h](TranspositionTable_8h.html) | Direct-mapped score cache                  |
| @ref BitBully::OpeningBook               | [OpeningBook.h](OpeningBook_8h.html)            | 8-/12-ply opening database loader            |
| @ref BitBully::MoveList                  | [MoveList.h](MoveList_8h.html)                  | Tiny priority queue used for move ordering   |

---

## Quick example

```cpp
#include "BitBully.h"

int main() {
  BitBully::Board b;          // empty starting position
  if (!b.play("4435")) {       // play a few moves (column indices)
    return 1;
  }
  std::cout << b.toString() << '\n';

  BitBully::BitBully solver;   // create the search engine
  const int score = solver.mtdf(b, /*firstGuess=*/0);
  std::cout << "score = " << score << '\n';
  return 0;
}
```

The score follows the convention introduced by Pascal Pons: positive means
the side to move wins, negative means it loses, and `0` is a draw. Use
@ref BitBully::BitBully::scoreToMovesLeft to translate the compact score
into the actual number of plies remaining.

---

## Where to go next

- @subpage architecture — high-level overview of the engine
- @subpage build_and_test — how to compile and test the C++ core
- @subpage references — papers, blog posts, and prior art that shaped the
  implementation
