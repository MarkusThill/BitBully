#include "Board.h"

#include <algorithm>
#include <array>
#include <cassert>

namespace BitBully {

Board::Board()
    : m_bAllTokens{BB_EMPTY},
      m_bActivePTokens{BB_EMPTY},
      m_movesLeft{N_COLUMNS * N_ROWS} {
  // asserts will be turned off in Release mode
  assert(uint64_t_popcnt(BB_ALL_LEGAL_TOKENS) == N_COLUMNS * N_ROWS);
  assert(uint64_t_popcnt(BB_ILLEGAL) ==
         CHAR_BIT * sizeof(TBitBoard) - N_COLUMNS * N_ROWS);
  assert(getRowMask(0) == getMask({0, 9, 18, 27, 36, 45, 54}));
  assert(getRowMask(5) == getMask({5, 14, 23, 32, 41, 50, 59}));
}

bool Board::setBoard(const std::vector<int>& moveSequence) {
  Board b;  // First operate on fresh board

  if (!b.play(moveSequence)) {
    return false;
  }

  *this = b;
  return true;
}

bool Board::setBoard(const std::string& moveSequence) {
  Board b;  // start with an empty board

  // attempt to play the move sequence
  if (!b.play(moveSequence)) return false;

  // only modify the current board on success
  *this = b;
  return true;
}

Board::TBoardArray Board::transpose(const TBoardArrayT& board) {
  TBoardArray result{};
  for (std::size_t c = 0; c < N_COLUMNS; ++c) {
    for (std::size_t r = 0; r < N_ROWS; ++r) {
      result[c][N_ROWS - r - 1] = board[r][c];
    }
  }
  return result;
}

bool Board::setBoard(const TBoardArrayT& board) {
  const auto boardT = transpose(board);
  return setBoard(boardT);
}

bool Board::setBoard(const TBoardArray& board) {
  if (!isValid(board)) {
    return false;
  }
  TBitBoard allTokens{BB_EMPTY}, yellowTokens{BB_EMPTY};
  for (auto c = 0; c < N_COLUMNS; ++c) {
    for (auto r = 0; r < N_ROWS; ++r) {
      const auto val = board[c][r];
      const auto mask = getMaskColRow(c, r);
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
  m_bActivePTokens = yellowTokens ^ (m_movesLeft & 1 ? allTokens : BB_EMPTY);
  m_bAllTokens = allTokens;

  return true;
}

bool Board::isValid(const TBoardArray& board) {
  // Counter for P_YELLOW, P_RED and P_EMPTY
  std::array<int, N_VALID_BOARD_VALUES> valCounts = {0UL};
  for (auto c = static_cast<TBoardArray::size_type>(0); c < N_COLUMNS; ++c) {
    bool columnComplete = false;
    for (auto r = static_cast<TBoardArray::size_type>(0); r < N_ROWS; ++r) {
      const auto val = board[c][r];
      // There are only 3 options for a cell:
      if (val != P_RED && val != P_YELLOW && val != P_EMPTY) {
        return false;
      }
      if (val == P_EMPTY) {
        columnComplete = true;  // rest of column should be empty
      } else {
        if (columnComplete) return false;  // Unexpected bYellow/red stone
      }
      valCounts[val]++;
    }
  }

  // Yellow cannot have fewer pieces than Red:
  if (const auto diff = valCounts[P_YELLOW] - valCounts[P_RED];
      diff < 0 || diff > 1) {
    return false;
  }

  // TODO: Check for multiple Wins...
  return true;
}

/* [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ 5, 14, 23, 32, 41, 50, 59],
 * [ 4, 13, 22, 31, 40, 49, 58],
 * [ 3, 12, 21, 30, 39, 48, 57],
 * [ 2, 11, 20, 29, 38, 47, 56],
 * [ 1, 10, 19, 28, 37, 46, 55],
 * [ 0,  9, 18, 27, 36, 45, 54]
 */
bool Board::play(const int column) {
  // Check, if column full...
  if (!isLegalMove(column)) return false;

  playMoveFast(column);

  return true;
}

bool Board::play(const std::vector<int>& moveSequence) {
  Board b = *this;  // First operate on copy

  for (const auto mv : moveSequence) {
    if (!b.play(mv)) return false;
  }
  *this = b;
  return true;
}

bool Board::play(const std::string& moveSequence) {
  for (const char c : moveSequence) {
    if (c < '0' || c >= '0' + N_COLUMNS) return false;
  }

  std::vector<int> moveSequenceInt;
  moveSequenceInt.reserve(
      moveSequence.size());  // optional, avoids reallocations
  std::transform(moveSequence.begin(), moveSequence.end(),
                 std::back_inserter(moveSequenceInt),
                 [](const char c) { return c - '0'; });
  return play(moveSequenceInt);
}

bool Board::isLegalMove(const int column) const {
  if (column < 0 || column >= N_COLUMNS) return false;

  TBitBoard columnMask = getColumnMask(column);
  columnMask -= (UINT64_C(1) << (column * COLUMN_BIT_OFFSET));
  return !(m_bAllTokens & columnMask & BB_TOP_ROW);
}

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
 * 0 1 1 1 | 1 0 0 0  < A = bcd
 * 1 0 0 0 | 0 0 0 0
 * 1 0 0 1 | 0 0 0 0
 * 1 0 1 0 | 0 0 0 0
 * 1 0 1 1 | 0 1 0 0  < B = acd
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
 * TODO: inspired by John Tromp:
 * https://github.com/gamesolver/fhourstones/blob/master/Game.c
 */
/*
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ 5, 14, 23, 32, 41, 50, 59],
 * [ 4, 13, 22, 31, 40, 49, 58],
 * [ 3, 12, 21, 30, 39, 48, 57],
 * [ 2, 11, 20, 29, 38, 47, 56],
 * [ 1, 10, 19, 28, 37, 46, 55],
 * [ 0,  9, 18, 27, 36, 45, 54]
 */
Board::TBitBoard Board::winningPositions(const TBitBoard x,
                                         const bool verticals) {
  // Vertical wins are fairly simple:
  TBitBoard wins = verticals ? (x << 1) & (x << 2) & (x << 3) : UINT64_C(0);

  for (auto b = COLUMN_BIT_OFFSET - 1; b <= COLUMN_BIT_OFFSET + 1; ++b) {
    // tokens shifted by 1 & 2 positions left, up-left and down-left (this
    // avoids some redundant computations below):
    auto tmp = (x << b) & (x << 2 * b);
    wins |= tmp & (x << 3 * b);  // A = bcd
    wins |= tmp & (x >> b);      // B = bcd

    // tokens shifted by 1 & 2 positions right, up-right and down-right:
    tmp = (x >> b) & (x >> 2 * b);
    wins |= tmp & (x << b);      // C = abd
    wins |= tmp & (x >> 3 * b);  // D = abc
  }

  return wins & BB_ALL_LEGAL_TOKENS;
}

/*
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ 5, 14, 23, 32, 41, 50, 59],
 * [ 4, 13, 22, 31, 40, 49, 58],
 * [ 3, 12, 21, 30, 39, 48, 57],
 * [ 2, 11, 20, 29, 38, 47, 56],
 * [ 1, 10, 19, 28, 37, 46, 55],
 * [ 0,  9, 18, 27, 36, 45, 54]
 */
bool Board::hasWin() const {
  // Vertical win:
  const auto y = m_bActivePTokens ^ m_bAllTokens;
  auto x = y & (y << 2);
  if (x & (x << 1)) return true;

  // Horizontal win:
  x = y & (y << 2 * COLUMN_BIT_OFFSET);
  if (x & (x << COLUMN_BIT_OFFSET)) return true;

  // Diagonal-1 win:
  x = y & (y << 2 * (COLUMN_BIT_OFFSET - 1));
  if (x & (x << (COLUMN_BIT_OFFSET - 1))) return true;

  // Diagonal-2 win:
  x = y & (y << 2 * (COLUMN_BIT_OFFSET + 1));
  if (x & (x << (COLUMN_BIT_OFFSET + 1))) return true;
  return false;
}

/*
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ *,  *,  *,  *,  *,  *,  *]
 * [ 5, 14, 23, 32, 41, 50, 59],
 * [ 4, 13, 22, 31, 40, 49, 58],
 * [ 3, 12, 21, 30, 39, 48, 57],
 * [ 2, 11, 20, 29, 38, 47, 56],
 * [ 1, 10, 19, 28, 37, 46, 55],
 * [ 0,  9, 18, 27, 36, 45, 54]
 */
MoveList Board::sortMoves(TBitBoard moves) const {
  MoveList mvList;

  // own threats
  const TBitBoard ownThreats = winningPositions(m_bActivePTokens, false);

  while (moves) {
    const auto mv = nextMove(moves);  // TODO: not yet efficient enough
    assert(uint64_t_popcnt(mv) == 1);

    // How many threats (indirect & direct) will I have after this move?
    const auto threats =
        winningPositions(m_bActivePTokens ^ mv, true) & ~(m_bAllTokens ^ mv);
    auto numThreats = static_cast<int>(uint64_t_popcnt(threats));

    // Usually, try to avoid moving under own threat since opponent will
    // neutralize it...
    if (ownThreats & (mv << 1)) numThreats--;

    mvList.insert(mv, numThreats);

    moves ^= mv;
  }
  return mvList;
}

Board::TBitBoard Board::findThreats(TBitBoard moves) {
  auto threats = winningPositions(m_bActivePTokens, true) & ~m_bAllTokens;

  // TODO: The pop couting function seems to matter a lot
  auto curNumThreats = uint64_t_popcnt(threats);
  auto threatMoves = UINT64_C(0);
  while (moves) {
    auto mv = lsb(moves);

    // Threats are winning positions which are generated by performing move mv.
    // However, we do not care about winning positions which are not achievable
    // any more (since the positions are already occupied). Hence, we have to
    // only select those positions as threats, which are still empty.
    // It seems that all kind of threats (immediate and indirect threats) are
    // beneficial for the move ordering!
    threats =
        winningPositions(m_bActivePTokens ^ mv, true) & ~(m_bAllTokens ^ mv);

    if (uint64_t_popcnt(threats & (moves ^ mv)) > 1)
      return mv;  // at least 2 immediate threats

    if (const auto numThreats = uint64_t_popcnt(threats);
        numThreats > curNumThreats) {
      threatMoves ^= mv;
    }
    moves ^= mv;
  }

  return threatMoves;
}

Board::TBitBoard Board::findOddThreats(TBitBoard moves) {
  constexpr auto ODD_ROWS = getRowMask(2) | getRowMask(4);
  auto threats = winningPositions(m_bActivePTokens, true) & ~m_bAllTokens;

  // TODO: The pop couting function seems to matter a lot
  const auto curNumThreats = uint64_t_popcnt(threats & ODD_ROWS);
  auto threatMoves = UINT64_C(0);
  while (moves) {
    const auto mv = lsb(moves);

    threats =
        winningPositions(m_bActivePTokens ^ mv, true) & ~(m_bAllTokens ^ mv);
    if (const auto numThreats = uint64_t_popcnt(threats & ODD_ROWS);
        numThreats > curNumThreats) {
      threatMoves ^= mv;
    }
    moves ^= mv;
  }

  return threatMoves;
}

/*
std::pair<unsigned long, unsigned long> Board::findOddEvenThreats() {
  constexpr auto ODD_ROWS = getRowMask(2) | getRowMask(4);
  constexpr auto EVEN_ROWS = getRowMask(1) | getRowMask(3) | getRowMask(5);
  auto threats = winningPositions(m_bActive) & ~m_bAll;
  auto oddThreats = threats & ODD_ROWS;
  auto evenThreats = threats & EVEN_ROWS;

  int curNumOddThreats = uint64_t_popcnt(oddThreats);
  int curNumEvenThreats = uint64_t_popcnt(evenThreats);
  auto threatOddMoves = UINT64_C(0);
  auto threatEvenMoves = UINT64_C(0);
  auto moves = generateMoves();
  while (moves) {
    auto mvMask = moves - UINT64_C(1);
    auto mv = ~mvMask & moves;

    // Threats are winning positions which are generated by performing move mv.
    // However, we do not care about winning positions which are not achievable
    // any more (since the positions are already occupied). Hence, we have to
    // only select those positions as threats, which are still empty.
    threats = winningPositions(m_bActive ^ mv) & ~(m_bAll ^ mv);
    oddThreats = threats & ODD_ROWS;
    int numOddThreats = uint64_t_popcnt(oddThreats);
    if (numOddThreats > curNumOddThreats) {
      threatOddMoves ^= mv;
    }

    evenThreats = threats & ODD_ROWS;
    int numEvenThreats = uint64_t_popcnt(oddThreats);
    if (numEvenThreats > curNumEvenThreats) {
      threatEvenMoves ^= mv;
    }

    moves ^= mv;
  }

  return std::pair{threatOddMoves, threatEvenMoves};
}
 */

bool Board::canWin() const {
  return winningPositions(m_bActivePTokens, true) &
         (m_bAllTokens + BB_BOTTOM_ROW);
}

bool Board::canWin(int column) const {
  return isLegalMove(column) &&
         (winningPositions(m_bActivePTokens, true) &
          (m_bAllTokens + BB_BOTTOM_ROW) & getColumnMask(column));
}

Board::TBitBoard Board::generateMoves() const {
  return (m_bAllTokens + BB_BOTTOM_ROW) & BB_ALL_LEGAL_TOKENS;
}

Board::TBoardArray Board::toArray() const {
  TBoardArray board{{0}};
  const auto activePlayer = (m_movesLeft & 1 ? P_RED : P_YELLOW);
  const auto inactivePlayer = opponent(activePlayer);
  for (auto c = 0; c < N_COLUMNS; ++c) {
    for (auto r = 0; r < N_ROWS; ++r) {
      if (const auto mask = getMaskColRow(c, r); m_bActivePTokens & mask) {
        board[c][r] = activePlayer;
      } else if (m_bAllTokens & mask) {
        board[c][r] = inactivePlayer;
      } else {
        board[c][r] = P_EMPTY;
      }
    }
  }
  return board;
}

std::string Board::toString() const {
  std::stringstream ss;
  ss << "\n  ";
  const auto arr = toArray();
  for (int r = N_ROWS - 1; r >= 0; r--) {
    for (int c = 0; c < N_COLUMNS; c++) {
      if (arr[c][r] == P_RED) {
        ss << "O  ";
      } else if (arr[c][r] == P_YELLOW) {
        ss << "X  ";
      } else {
        assert(arr[c][r] == P_EMPTY);
        ss << "_  ";
      }
    }
    ss << "\n  ";
  }
  return ss.str();
}

Board Board::mirror() const {
  Board mB;
  mB.m_movesLeft = m_movesLeft;
  mB.m_bActivePTokens = mirrorBitBoard(m_bActivePTokens);
  mB.m_bAllTokens = mirrorBitBoard(m_bAllTokens);
  assert(uint64_t_popcnt(mB.m_bActivePTokens) ==
         uint64_t_popcnt(m_bActivePTokens));
  assert(uint64_t_popcnt(mB.m_bAllTokens) == uint64_t_popcnt(m_bAllTokens));
  return mB;
}

}  // namespace BitBully
