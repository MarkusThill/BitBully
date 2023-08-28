#include "Board.h"

#define OPT_X86 false
namespace BitBully {

bool Board::hasWin() {
  auto &h = m_columnHeight;
  THeight odd = (h[0] ^ h[1] ^ h[2] ^ h[3] ^ h[4] ^ h[5] ^ h[6]) & 1;
  if (odd) {
    return hasWinRed();
  }
  return hasWinYellow();
}

bool Board::hasWinRed() {
  const auto x = ~m_board.bbRed;
  // TODO: Which one is faster???
#if OPT_X86
  int y = static_cast<int>(x);
#else
  const auto &y = x;
#endif
  switch (m_columnHeight[3]) { // TODO: maybe switch is slower than if-else
  case 0:
    if (!(x & 0x20820000000LL && x & 0x820020000LL && y & 0x20020800 &&
          y & 0x20820 && y & 0x10204 && x & 0x4210000000LL))
      return true;
    break;
  case 1:
    if (!(y & 0x20008100 && y & 0x10410 && x & 0x10410000000LL &&
          x & 0x2108000000LL && x & 0x108020000LL && y & 0x10010400 &&
          y & 0x8102 && x & 0x410010000LL))
      return true;
    break;
  case 2:
    if (!(x & 0x810004000LL && y & 0x4010800 && x & 0x8208000000LL &&
          x & 0x208008000LL && y & 0x8008200 && y & 0x8208 && y & 0x84010000 &&
          y & 0x10004080 && y & 0x4081 && x & 0x1084000000LL))
      return true;
    break;
  case 3:
    if (!(x & 0x20408000000LL && y & 0x8420 && y & 0xE00000 &&
          x & 0x4104000000LL && y & 0x2008400 && x & 0x408002000LL &&
          y & 0x4004100 && x & 0x104004000LL && y & 0x42008000 &&
          y & 0x8002040 && y & 0x4104))
      return true;
    break;
  case 4:
    if (!(x & 0x10204000000LL && y & 0x700000 && y & 0x4210 &&
          x & 0x204001000LL && x & 0x2082000000LL && y & 0x1004200 &&
          y & 0x82002000 && y & 0x2002080 && y & 0x2082))
      return true;
    break;
  case 5:
    if (!(y & 0x380000 && x & 0x8102000000LL && y & 0x2108 &&
          x & 0x1041000000LL && y & 0x41001000 && y & 0x1001040 && y & 0x1041))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[4]) {
  case 0:
    if (!(x & 0x108400000LL && y & 0x800820 && y & 0x20800800 &&
          x & 0x820800000LL))
      return true;
    break;
  case 1:
    if (!(y & 0x800204 && y & 0x4200800 && y & 0x400410 && y & 0x10400400 &&
          x & 0x410400000LL && y & 0x84200000))
      return true;
    break;
  case 2:
    if (!(y & 0x100420 && y & 0x20400100 && y & 0x8200200 &&
          x & 0x208200000LL && y & 0x200208 && y & 0x2100400 && y & 0x400102 &&
          y & 0x42100000))
      return true;
    break;
  case 3:
    if (!(x & 0x810200000LL && y & 0x80210 && y & 0x38000 && y & 0x4100100 &&
          x & 0x104100000LL && y & 0x10200080 && y & 0x1080200 &&
          y & 0x100104 && y & 0x200081))
      return true;
    break;
  case 4:
    if (!(x & 0x408100000LL && y & 0x40108 && y & 0x1C000 && y & 0x2080080 &&
          y & 0x82080000 && y & 0x8100040 && y & 0x80082))
      return true;
    break;
  case 5:
    if (!(x & 0x204080000LL && y & 0xE000 && y & 0x1040040 && y & 0x41040000 &&
          y & 0x40041))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[2]) {
  case 0:
    if (!(y & 0x408100 && x & 0x20800800000LL && y & 0x820800 &&
          x & 0x800820000LL))
      return true;
    break;
  case 1:
    if (!(x & 0x4200800000LL && x & 0x10400400000LL && x & 0x800204000LL &&
          y & 0x410400 && x & 0x400410000LL && y & 0x204080))
      return true;
    break;
  case 2:
    if (!(x & 0x100420000LL && x & 0x8200200000LL && x & 0x20400100000LL &&
          x & 0x200208000LL && y & 0x208200 && x & 0x400102000LL &&
          x & 0x2100400000LL && y & 0x102040))
      return true;
    break;
  case 3:
    if (!(y & 0x210800 && x & 0x10200080000LL && x & 0x4100100000LL &&
          x & 0x100104000LL && x & 0x200081000LL && y & 0x80210000 &&
          y & 0x38000000 && y & 0x104100 && x & 0x1080200000LL))
      return true;
    break;
  case 4:
    if (!(y & 0x108400 && x & 0x8100040000LL && y & 0x80082000 &&
          y & 0x40108000 && y & 0x82080 && y & 0x1C000000 &&
          x & 0x2080080000LL))
      return true;
    break;
  case 5:
    if (!(y & 0x84200 && y & 0xE000000 && y & 0x40041000 && y & 0x41040 &&
          x & 0x1040040000LL))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[5]) {
  case 0:
    if (!(y & 0x4210000 && y & 0x20820000 && y & 0x820020))
      return true;
    break;
  case 1:
    if (!(y & 0x10410000 && y & 0x410010 && y & 0x108020 && y & 0x2108000))
      return true;
    break;
  case 2:
    if (!(y & 0x8208000 && y & 0x208008 && y & 0x84010 && y & 0x810004 &&
          y & 0x1084000))
      return true;
    break;
  case 3:
    if (!(y & 0x20408000 && y & 0x4104000 && y & 0xE00 && y & 0x104004 &&
          y & 0x42008 && y & 0x408002))
      return true;
    break;
  case 4:
    if (!(y & 0x10204000 && y & 0x2082000 && y & 0x700 && y & 0x82002 &&
          y & 0x204001))
      return true;
    break;
  case 5:
    if (!(y & 0x8102000 && y & 0x1041000 && y & 0x380 && y & 0x41001))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[1]) {
  case 0:
    if (!(y & 0x10204000 && y & 0x20820000 && x & 0x20020800000LL))
      return true;
    break;
  case 1:
    if (!(x & 0x10010400000LL && x & 0x20008100000LL && y & 0x10410000 &&
          y & 0x8102000))
      return true;
    break;
  case 2:
    if (!(x & 0x8008200000LL && x & 0x4010800000LL && y & 0x8208000 &&
          x & 0x10004080000LL && y & 0x4081000))
      return true;
    break;
  case 3:
    if (!(y & 0x8420000 && x & 0xE00000000LL && y & 0x4104000 &&
          x & 0x4004100000LL && x & 0x8002040000LL && x & 0x2008400000LL))
      return true;
    break;
  case 4:
    if (!(y & 0x4210000 && y & 0x2082000 && x & 0x700000000LL &&
          x & 0x2002080000LL && x & 0x1004200000LL))
      return true;
    break;
  case 5:
    if (!(y & 0x2108000 && y & 0x1041000 && x & 0x380000000LL &&
          x & 0x1001040000LL))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[6]) {
  case 0:
    if (!(y & 0x108400 && y & 0x820800))
      return true;
    break;
  case 1:
    if (!(y & 0x410400 && y & 0x84200))
      return true;
    break;
  case 2:
    if (!(y & 0x208200 && y & 0x42100))
      return true;
    break;
  case 3:
    if (!(y & 0x810200 && y & 0x38 && y & 0x104100))
      return true;
    break;
  case 4:
    if (!(y & 0x408100 && y & 0x1C && y & 0x82080))
      return true;
    break;
  case 5:
    if (!(y & 0x204080 && y & 0xE && y & 0x41040))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[0]) {
  case 0:
    if (!(x & 0x408100000LL && x & 0x820800000LL))
      return true;
    break;
  case 1:
    if (!(x & 0x410400000LL && x & 0x204080000LL))
      return true;
    break;
  case 2:
    if (!(x & 0x208200000LL && x & 0x102040000LL))
      return true;
    break;
  case 3:
    if (!(x & 0x38000000000LL && x & 0x210800000LL && x & 0x104100000LL))
      return true;
    break;
  case 4:
    if (!(x & 0x108400000LL && x & 0x1C000000000LL && y & 0x82080000))
      return true;
    break;
  case 5:
    if (!(y & 0x84200000 && x & 0xE000000000LL && y & 0x41040000))
      return true;
    break;
  default:
    break;
  }
  return false;
}

bool Board::hasWinYellow() {
  const auto x = ~m_board.bbYellow;

// TODO: Which one is faster???
#if OPT_X86
  int y = static_cast<int>(x);
#else
  const auto &y = x;
#endif

  switch (m_columnHeight[3]) {
  case 0:
    if (!(x & 0x20820000000LL && x & 0x820020000LL && y & 0x20020800 &&
          y & 0x20820 && y & 0x10204 && x & 0x4210000000LL))
      return true;
    break;
  case 1:
    if (!(y & 0x20008100 && y & 0x10410 && x & 0x10410000000LL &&
          x & 0x2108000000LL && x & 0x108020000LL && y & 0x10010400 &&
          y & 0x8102 && x & 0x410010000LL))
      return true;
    break;
  case 2:
    if (!(x & 0x810004000LL && y & 0x4010800 && x & 0x8208000000LL &&
          x & 0x208008000LL && y & 0x8008200 && y & 0x8208 && y & 0x84010000 &&
          y & 0x10004080 && y & 0x4081 && x & 0x1084000000LL))
      return true;
    break;
  case 3:
    if (!(x & 0x20408000000LL && y & 0x8420 && y & 0xE00000 &&
          x & 0x4104000000LL && y & 0x2008400 && x & 0x408002000LL &&
          y & 0x4004100 && x & 0x104004000LL && y & 0x42008000 &&
          y & 0x8002040 && y & 0x4104))
      return true;
    break;
  case 4:
    if (!(x & 0x10204000000LL && y & 0x700000 && y & 0x4210 &&
          x & 0x204001000LL && x & 0x2082000000LL && y & 0x1004200 &&
          y & 0x82002000 && y & 0x2002080 && y & 0x2082))
      return true;
    break;
  case 5:
    if (!(y & 0x380000 && x & 0x8102000000LL && y & 0x2108 &&
          x & 0x1041000000LL && y & 0x41001000 && y & 0x1001040 && y & 0x1041))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[4]) {
  case 0:
    if (!(x & 0x108400000LL && y & 0x800820 && y & 0x20800800 &&
          x & 0x820800000LL))
      return true;
    break;
  case 1:
    if (!(y & 0x800204 && y & 0x4200800 && y & 0x400410 && y & 0x10400400 &&
          x & 0x410400000LL && y & 0x84200000))
      return true;
    break;
  case 2:
    if (!(y & 0x100420 && y & 0x20400100 && y & 0x8200200 &&
          x & 0x208200000LL && y & 0x200208 && y & 0x2100400 && y & 0x400102 &&
          y & 0x42100000))
      return true;
    break;
  case 3:
    if (!(x & 0x810200000LL && y & 0x80210 && y & 0x38000 && y & 0x4100100 &&
          x & 0x104100000LL && y & 0x10200080 && y & 0x1080200 &&
          y & 0x100104 && y & 0x200081))
      return true;
    break;
  case 4:
    if (!(x & 0x408100000LL && y & 0x40108 && y & 0x1C000 && y & 0x2080080 &&
          y & 0x82080000 && y & 0x8100040 && y & 0x80082))
      return true;
    break;
  case 5:
    if (!(x & 0x204080000LL && y & 0xE000 && y & 0x1040040 && y & 0x41040000 &&
          y & 0x40041))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[2]) {
  case 0:
    if (!(y & 0x408100 && x & 0x20800800000LL && y & 0x820800 &&
          x & 0x800820000LL))
      return true;
    break;
  case 1:
    if (!(x & 0x4200800000LL && x & 0x10400400000LL && x & 0x800204000LL &&
          y & 0x410400 && x & 0x400410000LL && y & 0x204080))
      return true;
    break;
  case 2:
    if (!(x & 0x100420000LL && x & 0x8200200000LL && x & 0x20400100000LL &&
          x & 0x200208000LL && y & 0x208200 && x & 0x400102000LL &&
          x & 0x2100400000LL && y & 0x102040))
      return true;
    break;
  case 3:
    if (!(y & 0x210800 && x & 0x10200080000LL && x & 0x4100100000LL &&
          x & 0x100104000LL && x & 0x200081000LL && y & 0x80210000 &&
          y & 0x38000000 && y & 0x104100 && x & 0x1080200000LL))
      return true;
    break;
  case 4:
    if (!(y & 0x108400 && x & 0x8100040000LL && y & 0x80082000 &&
          y & 0x40108000 && y & 0x82080 && y & 0x1C000000 &&
          x & 0x2080080000LL))
      return true;
    break;
  case 5:
    if (!(y & 0x84200 && y & 0xE000000 && y & 0x40041000 && y & 0x41040 &&
          x & 0x1040040000LL))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[5]) {
  case 0:
    if (!(y & 0x4210000 && y & 0x20820000 && y & 0x820020))
      return true;
    break;
  case 1:
    if (!(y & 0x10410000 && y & 0x410010 && y & 0x108020 && y & 0x2108000))
      return true;
    break;
  case 2:
    if (!(y & 0x8208000 && y & 0x208008 && y & 0x84010 && y & 0x810004 &&
          y & 0x1084000))
      return true;
    break;
  case 3:
    if (!(y & 0x20408000 && y & 0x4104000 && y & 0xE00 && y & 0x104004 &&
          y & 0x42008 && y & 0x408002))
      return true;
    break;
  case 4:
    if (!(y & 0x10204000 && y & 0x2082000 && y & 0x700 && y & 0x82002 &&
          y & 0x204001))
      return true;
    break;
  case 5:
    if (!(y & 0x8102000 && y & 0x1041000 && y & 0x380 && y & 0x41001))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[1]) {
  case 0:
    if (!(y & 0x10204000 && y & 0x20820000 && x & 0x20020800000LL))
      return true;
    break;
  case 1:
    if (!(x & 0x10010400000LL && x & 0x20008100000LL && y & 0x10410000 &&
          y & 0x8102000))
      return true;
    break;
  case 2:
    if (!(x & 0x8008200000LL && x & 0x4010800000LL && y & 0x8208000 &&
          x & 0x10004080000LL && y & 0x4081000))
      return true;
    break;
  case 3:
    if (!(y & 0x8420000 && x & 0xE00000000LL && y & 0x4104000 &&
          x & 0x4004100000LL && x & 0x8002040000LL && x & 0x2008400000LL))
      return true;
    break;
  case 4:
    if (!(y & 0x4210000 && y & 0x2082000 && x & 0x700000000LL &&
          x & 0x2002080000LL && x & 0x1004200000LL))
      return true;
    break;
  case 5:
    if (!(y & 0x2108000 && y & 0x1041000 && x & 0x380000000LL &&
          x & 0x1001040000LL))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[6]) {
  case 0:
    if (!(y & 0x108400 && y & 0x820800))
      return true;
    break;
  case 1:
    if (!(y & 0x410400 && y & 0x84200))
      return true;
    break;
  case 2:
    if (!(y & 0x208200 && y & 0x42100))
      return true;
    break;
  case 3:
    if (!(y & 0x810200 && y & 0x38 && y & 0x104100))
      return true;
    break;
  case 4:
    if (!(y & 0x408100 && y & 0x1C && y & 0x82080))
      return true;
    break;
  case 5:
    if (!(y & 0x204080 && y & 0xE && y & 0x41040))
      return true;
    break;
  default:
    break;
  }
  switch (m_columnHeight[0]) {
  case 0:
    if (!(x & 0x408100000LL && x & 0x820800000LL))
      return true;
    break;
  case 1:
    if (!(x & 0x410400000LL && x & 0x204080000LL))
      return true;
    break;
  case 2:
    if (!(x & 0x208200000LL && x & 0x102040000LL))
      return true;
    break;
  case 3:
    if (!(x & 0x38000000000LL && x & 0x210800000LL && x & 0x104100000LL))
      return true;
    break;
  case 4:
    if (!(x & 0x108400000LL && x & 0x1C000000000LL && y & 0x82080000))
      return true;
    break;
  case 5:
    if (!(y & 0x84200000 && x & 0xE000000000LL && y & 0x41040000))
      return true;
    break;
  default:
    break;
  }
  return false;
}
} // namespace BitBully