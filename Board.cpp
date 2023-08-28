#include "Board.h"
#include <array>
#include <cassert>

namespace BitBully {

Board::Board() : m_board{UINT64_C(0), UINT64_C(0)}, m_columnHeight{0} {}

bool Board::setBoard(const TBoardArray &board) {
  if (!isValid(board)) {
    return false;
  }

  TBitBoard bbYellow{UINT64_C(0)}, bbRed{UINT64_C(0)};
  for (auto c = 0; c < N_COLUMNS; ++c) {
    for (auto r = 0; r < N_ROWS; ++r) {
      auto val = board[c][r];
      auto mask = getMask(c, r);
      if (val == P_YELLOW) {
        bbYellow ^= mask;
      } else if (val == P_RED) {
        bbRed ^= mask;
      } else {
        assert(val == P_EMPTY);
      }
    }
  }
  m_board.bbYellow = bbYellow;
  m_board.bbRed = bbRed;
  m_columnHeight = getColumnHeights(m_board);

  return true;
}

void Board::setBoardFast(TBitBoard bbYellow, TBitBoard bbRed) {
  m_board = {bbYellow, bbRed};
  m_columnHeight = getColumnHeights(m_board);
}

void Board::setBoardFast(Board::BitBoard bb) {
  m_board = bb;
  m_columnHeight = getColumnHeights(bb);
}

bool Board::setBoard(TBitBoard bbYellow, TBitBoard bbRed) {
  Board bb;
  bb.m_board.bbYellow = bbYellow;
  bb.m_board.bbRed = bbRed;
  bb.m_columnHeight = getColumnHeights(bbYellow, bbRed);

  if (!isValid(bb.toArray())) {
    return false;
  }

  *this = bb;
  return false;
}

Board::THeightsArray Board::getColumnHeights(const Board::TBitBoard &bbYellow,
                                             const Board::TBitBoard &bbRed) {
  THeightsArray heights;
  const auto bbYellowRed = bbYellow | bbRed;
  for (auto i = 0; i < N_COLUMNS; i++) {
    THeight j;
    for (j = 0; j < N_ROWS; j++) {
      auto mask = getMask(i, j);
      if ((bbYellowRed & mask) != mask)
        break;
    }
    heights[i] = j;
  }
  return heights;
}

Board::TBoardArray Board::toArray(const Board::TBitBoard &bbYellow,
                                  const Board::TBitBoard &bbRed) {
  TBoardArray board;
  for (int r = N_ROWS - 1; r >= 0; r--) {
    for (int c = 0; c < N_COLUMNS; c++) {
      auto mask = getMask(c, r);
      if (bbRed & mask) {
        board[c][r] = P_RED;
      } else if (bbYellow & mask) {
        board[c][r] = P_YELLOW;
      } else {
        board[c][r] = P_EMPTY;
      }
    }
  }
  return board;
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
          return false; // Unexpected yellow/red stone
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

void Board::playMoveFast(int column) {
  // TODO:...
}

bool Board::playMove(int column) {
  auto &h = m_columnHeight;
  if (column < 0 || column >= N_COLUMNS)
    return false;
  if (h[column] >= N_ROWS)
    return false;
  THeight odd = (h[0] ^ h[1] ^ h[2] ^ h[3] ^ h[4] ^ h[5] ^ h[6]) & 1;
  auto mask = getMask(column, h[column]);
  if (odd) {
    m_board.bbRed ^= mask;
  } else {
    m_board.bbYellow ^= mask;
  }
  h[column]++;
  return true;
}

} // namespace BitBully