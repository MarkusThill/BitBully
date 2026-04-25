# Architecture {#architecture}

[TOC]

This page explains how the C++ pieces fit together. If you are looking for
specific function signatures, jump straight to the class index instead.

## High-level layering

\dot
digraph BitBullyArch {
  rankdir=TB;
  bgcolor="transparent";
  node [shape=box, style="rounded,filled", fontname="Helvetica",
        fillcolor="#fbf1d9", color="#a5781f"];
  edge [color="#a5781f"];

  py    [label="Python package\nbitbully.solver / bitbully.board",
         fillcolor="#fdf9ee"];
  core  [label="bitbully_core (pybind11)\nBoardCore / BitBullyCore / OpeningBookCore"];
  bb    [label="BitBully::BitBully\nnegamax + MTD(f) driver",
         URL="\ref BitBully::BitBully"];
  board [label="BitBully::Board\nbitboards + move generation",
         URL="\ref BitBully::Board"];
  tt    [label="TranspositionTable\ndirect-mapped score cache",
         URL="\ref BitBully::TranspositionTable"];
  book  [label="OpeningBook\n8-/12-ply pre-computed answers",
         URL="\ref BitBully::OpeningBook"];

  py    -> core;
  core  -> bb;
  bb    -> board;
  bb    -> tt;
  bb    -> book;
  board -> book [style=dashed, label="Huffman key"];
}
\enddot

## Bitboard layout

@ref BitBully::Board encodes a 7&times;6 grid in two `uint64_t` words:

- `m_bAllTokens`: every occupied cell.
- `m_bActivePTokens`: every cell currently held by the side to move.

Each column owns nine bits in the layout below. The top three bits of each
column act as guard bits so that "drop a stone" can be expressed as a single
`bitboard + BB_BOTTOM_ROW` increment without overflowing into the next
column. The complement of `BitBully::illegalBitMask()` covers exactly the
42 playable cells.

```
[ *,  *,  *,  *,  *,  *,  *]    <- guard row (never set)
[ *,  *,  *,  *,  *,  *,  *]
[ *,  *,  *,  *,  *,  *,  *]
[ 5, 14, 23, 32, 41, 50, 59]    <- top row
[ 4, 13, 22, 31, 40, 49, 58]
[ 3, 12, 21, 30, 39, 48, 57]
[ 2, 11, 20, 29, 38, 47, 56]
[ 1, 10, 19, 28, 37, 46, 55]
[ 0,  9, 18, 27, 36, 45, 54]    <- bottom row
```

## Search

The solver in @ref BitBully::BitBully::negamax combines the following ideas:

1. **Move ordering** via @ref BitBully::Board::sortMoves —
   threats-first with a slight central preference.
2. **Bound-aware transposition table** with EXACT, LOWER, and UPPER flags
   (see @ref BitBully::TranspositionTable::Entry).
3. **Enhanced Transposition Cutoff (ETC)** at low depths, tested via the
   one-ply child positions.
4. **Mirror symmetry**: every position and its left/right mirror share the
   same theoretical value, so probes for the mirrored UID can shortcut work.
5. **Opening book** lookups when the configured ply count is reached, using
   the Huffman encoding produced by @ref BitBully::Board::toHuffman.
6. **Depth-limited rollouts** (@ref BitBully::BitBully::rollout) for fast
   evaluations near the depth horizon.

The two top-level drivers are:

- @ref BitBully::BitBully::mtdf — the recommended driver; extremely fast
  with a decent first-guess.
- @ref BitBully::BitBully::nullWindow — alternative binary search; slower
  but useful for cross-checks.

## Score convention

The compact integer scores follow Pascal Pons' convention:

| Score    | Meaning                                                   |
| -------- | --------------------------------------------------------- |
| `s > 0`  | Side-to-move wins; final position reached on ply `2s − 1` |
| `s < 0`  | Side-to-move loses; final position reached on ply `2|s|`  |
| `s = 0`  | Draw                                                      |

@ref BitBully::BitBully::scoreToMovesLeft turns these into "plies until the
game ends", which is the form most users actually want to display.
