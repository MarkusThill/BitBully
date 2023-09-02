#include "Board.h"
#include <array>
#include <cassert>
#include <iostream>

namespace BitBully {

Board::Board()
    : m_bActive{BB_EMPTY}, //
      m_bAll{BB_EMPTY},    //
      m_movesLeft{N_COLUMNS * N_ROWS} {
  // TODO: where to put some general asserts???
  // Some asserts that will be turned off in Release mode
  assert(uint64_t_popcnt(BB_ALL_LEGAL_TOKENS) == N_COLUMNS * N_ROWS);
  assert(uint64_t_popcnt(BB_ILLEGAL) ==
         CHAR_BIT * sizeof(TBitBoard) - N_COLUMNS * N_ROWS);
}

bool Board::setBoard(const TBoardArray &board) {
  if (!isValid(board)) {
    return false;
  }
  TBitBoard allTokens{BB_EMPTY}, yellowTokens{BB_EMPTY};
  for (auto c = 0; c < N_COLUMNS; ++c) {
    for (auto r = 0; r < N_ROWS; ++r) {
      auto val = board[c][r];
      auto mask = getMaskColRow(c, r);
      if (val == P_YELLOW) {
        yellowTokens ^= mask;
      } else if (val != P_RED) {
        assert(val == P_EMPTY);
        continue;
      }
      assert((allTokens & mask) == BB_EMPTY);
      allTokens ^= mask;
    }
  }

  m_movesLeft =
      N_COLUMNS * N_ROWS - static_cast<int>(uint64_t_popcnt(allTokens));

  // if m_movesLeft is odd that means that its players RED turn
  m_bActive = yellowTokens ^ (m_movesLeft & 1 ? allTokens : BB_EMPTY);
  m_bAll = allTokens;

  return true;
}

bool Board::isValid(const Board::TBoardArray &board) {
  // Counter for P_YELLOW, P_RED and P_EMPTY
  std::array<int, N_VALID_BOARD_VALUES> valCounts = {0UL};
  for (auto c = TBoardArray::size_type(0); c < N_COLUMNS; ++c) {
    bool columnComplete = false;
    for (auto r = TBoardArray::size_type(0); r < N_ROWS; ++r) {
      auto val = board[c][r];
      // There are only 3 options for a cell:
      if (val != P_RED && val != P_YELLOW && val != P_EMPTY) {
        return false;
      } else if (val == P_EMPTY) {
        columnComplete = true; // rest of column should be empty
      } else {
        if (columnComplete)
          return false; // Unexpected bYellow/red stone
      }
      valCounts[val]++;
    }
  }

  // Yellow cannot have less pieces than Red:
  auto diff = valCounts[P_YELLOW] - valCounts[P_RED];
  if (diff < 0 || diff > 1) {
    return false;
  }

  // TODO: Check for multiple Wins...
  return true;
}

/* [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [59, 50, 41, 32, 23, 14,  5],
 * [58, 49, 40, 31, 22, 13,  4],
 * [57, 48, 39, 30, 21, 12,  3],
 * [56, 47, 38, 29, 20, 11,  2],
 * [55, 46, 37, 28, 19, 10,  1],
 * [54, 45, 36, 27, 18,  9,  0]
 */
bool Board::playMove(int column) {
  // Check, if column full...
  if (!isLegalMove(column))
    return false;

  playMoveFast(column);

  return true;
}

bool Board::isLegalMove(int column) {
  if (column < 0 || column >= N_COLUMNS)
    return false;

  TBitBoard columnMask = getColumnMask(column);
  columnMask -= (UINT64_C(1) << (column * COLUMN_BIT_OFFSET));
  return !(m_bAll & columnMask & BB_TOP_ROW);
}

/*Board::TBitBoard Board::winningPositions(TBitBoard x) {
  // Vertical wins are fairly simple:
  TBitBoard wins = (x << 1) & (x << 2) & (x << 3);

  // ====================================================================
  // Horizontal Wins (move tokens by +-9 bits)
  // tokens shifted by 1 & 2 columns to the left (this avoids some redundant
  // computations below):
  auto tmp = (x << COLUMN_BIT_OFFSET) & (x << 2 * COLUMN_BIT_OFFSET);
  wins |= tmp & (x << 3 * COLUMN_BIT_OFFSET); // A = bcd
  wins |= tmp & (x >> COLUMN_BIT_OFFSET);     // B = bcd

  // tokens shifted by 1 & 2 columns to the right:
  tmp = (x >> COLUMN_BIT_OFFSET) & (x >> 2 * COLUMN_BIT_OFFSET);
  wins |= tmp & (x << COLUMN_BIT_OFFSET);     // C = abd
  wins |= tmp & (x >> 3 * COLUMN_BIT_OFFSET); // D = abc
  // ====================================================================

  // ====================================================================
  // Diagonal Wins 1: bottom-left to top-right (move tokens by +-8 bits)
  // tokens shifted 1 & 2 positions towards the bottom-left:
  constexpr auto DIAG_OFFSET1 = COLUMN_BIT_OFFSET - 1;
  tmp = (x << DIAG_OFFSET1) & (x << 2 * DIAG_OFFSET1);
  wins |= tmp & (x << 3 * DIAG_OFFSET1); // A = bcd
  wins |= tmp & (x >> DIAG_OFFSET1);     // B = bcd

  // tokens shifted by 1 & 2 positions towards the top-right:
  tmp = (x >> DIAG_OFFSET1) & (x >> 2 * DIAG_OFFSET1);
  wins |= tmp & (x << DIAG_OFFSET1);     // C = abd
  wins |= tmp & (x >> 3 * DIAG_OFFSET1); // D = abc

  // TODO: Not checked..
  // ====================================================================
  // Diagonal Wins 2: bottom-left to top-right (move tokens by +-8 bits)
  // tokens shifted 1 & 2 positions towards the top-left:
  constexpr auto DIAG_OFFSET2 = COLUMN_BIT_OFFSET + 1;
  tmp = (x << DIAG_OFFSET2) & (x << 2 * DIAG_OFFSET2);
  wins |= tmp & (x << 3 * DIAG_OFFSET2); // A = bcd
  wins |= tmp & (x >> DIAG_OFFSET2);     // B = bcd

  // tokens shifted by 1 & 2 positions towards the bottom-right:
  tmp = (x >> DIAG_OFFSET2) & (x >> 2 * DIAG_OFFSET2);
  wins |= tmp & (x << DIAG_OFFSET2);     // C = abd
  wins |= tmp & (x >> 3 * DIAG_OFFSET2); // D = abc

  return wins & BB_ALL_LEGAL_TOKENS;
}
 */

/*
 * Simplified case: Position x is a nibble (4 bit): x = abcd
 *
 * a b c d | A B C D
 * --------+--------
 * 0 0 0 0 | 0 0 0 0
 * 0 0 0 1 | 0 0 0 0
 * 0 0 1 0 | 0 0 0 0
 * 0 0 1 1 | 0 0 0 0
 * 0 1 0 0 | 0 0 0 0
 * 0 1 0 1 | 0 0 0 0
 * 0 1 1 0 | 0 0 0 0
 * 0 1 1 1 | 0 0 0 0  < A = bcd
 * 1 0 0 0 | 0 0 0 0
 * 1 0 0 1 | 0 0 0 0
 * 1 0 1 0 | 0 0 0 0
 * 1 0 1 1 | 0 0 0 0  < B = acd
 * 1 1 0 0 | 0 0 0 0
 * 1 1 0 1 | 0 0 1 0  < C = abd
 * 1 1 1 0 | 0 0 0 1  < D = abc
 * 1 1 1 1 | * * * *  < Should not happen (since it already contains a win)
 *
 * In order to check if C = abd is true (putting a token in "c" would win), we
 * have to shift the remaining bits "a", "b", "d" onto the position "c" and then
 * perform a bitwise "AND". In the simplified tabular case above, we could do
 * something like " C = (x >> 2) & (x >> 1) & ( x << 1)
 * Per direction (vertical is an exception) we have 4 possible winning positions
 * A, B, C and, D. We have to check all 4 of them. However, in order to win, the
 * target position has to be empty and reachable. This has to be checked
 * subsequently.
 * TODO: inspired by John Tromp and Pascal Pons
 */
/* [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [59, 50, 41, 32, 23, 14,  5],
 * [58, 49, 40, 31, 22, 13,  4],
 * [57, 48, 39, 30, 21, 12,  3],
 * [56, 47, 38, 29, 20, 11,  2],
 * [55, 46, 37, 28, 19, 10,  1],
 * [54, 45, 36, 27, 18,  9,  0]
 */
Board::TBitBoard Board::winningPositions(TBitBoard x) {
  // Vertical wins are fairly simple:
  TBitBoard wins = (x << 1) & (x << 2) & (x << 3);

  for (auto bOff = COLUMN_BIT_OFFSET - 1; bOff <= COLUMN_BIT_OFFSET + 1;
       ++bOff) {
    // tokens shifted by 1 & 2 positions left, up-left and down-left (this
    // avoids some redundant computations below):
    auto tmp = (x << bOff) & (x << 2 * bOff);
    wins |= tmp & (x << 3 * bOff); // A = bcd
    wins |= tmp & (x >> bOff);     // B = bcd

    // tokens shifted by 1 & 2 positions right, up-right and down-right:
    tmp = (x >> bOff) & (x >> 2 * bOff);
    wins |= tmp & (x << bOff);     // C = abd
    wins |= tmp & (x >> 3 * bOff); // D = abc
  }

  return wins & BB_ALL_LEGAL_TOKENS;
}

bool Board::hasWin() {
  return winningPositions(m_bActive) & (m_bAll + BB_BOTTOM_ROW);
}

Board::TBoardArray Board::toArray() {
  TBoardArray board{0};
  auto activePlayer = (m_movesLeft & 1 ? P_RED : P_YELLOW);
  auto inactivePlayer = opponent(activePlayer);
  for (auto c = 0; c < N_COLUMNS; ++c) {
    for (auto r = 0; r < N_ROWS; ++r) {
      auto mask = getMaskColRow(c, r);
      if (m_bActive & mask) {
        board[c][r] = activePlayer;
      } else if (m_bAll & mask) {
        board[c][r] = inactivePlayer;
      } else {
        board[c][r] = P_EMPTY;
      }
    }
  }
  return board;
}

} // namespace BitBully