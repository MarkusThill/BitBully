// Opening Book and transposition table could be the same table?
// TODO: Simple neural net for move ordering? input board, output: 7-dim vector
// TODO: Use a simple logger. Use glog of google...
// TODO: Log computation times using a software version into a txt file...
// TODO: Play n games against a random (or more advanced) player: It has to win
// every single game! ...

#include <random>  // For C++11/C++17 random library

#include "Board.h"
#include "Solver.hpp"
#include "gtest/gtest.h"

class BoardTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}

  ~BoardTest() override = default;
};

TEST_F(BoardTest, getMask) {
  auto mask = BitBully::getMask({0, 1});
  EXPECT_EQ(mask, UINT64_C(3));
  mask = BitBully::getMask({0, 1, 2});
  EXPECT_EQ(mask, UINT64_C(7));
  mask = BitBully::getMask({});
  EXPECT_EQ(mask, UINT64_C(0));
  mask = BitBully::getMask({31});
  EXPECT_EQ(mask, UINT64_C(2147483648));
  mask = BitBully::getMask({63});
  EXPECT_EQ(mask, UINT64_C(9223372036854775808));
  mask = BitBully::getMask({64});
  EXPECT_EQ(mask, UINT64_C(0));
}

TEST_F(BoardTest, setBoard) {
  using B = BitBully::Board;

  // Empty board
  ASSERT_TRUE(B().setBoard(std::vector<int>{}));

  // First row
  ASSERT_TRUE(B().setBoard(std::vector{0, 1, 2, 3, 4, 5, 6}));

  // Fill one column
  ASSERT_TRUE(B().setBoard(std::vector(6, 3)));

  // Column index too large
  ASSERT_FALSE(B().setBoard(std::vector{0, 1, 7, 2}));

  // negative column index
  ASSERT_FALSE(B().setBoard(std::vector{0, 1, -1, 2}));

  // Too many moves into one column
  ASSERT_FALSE(B().setBoard(std::vector(7, 3)));

  // Move Sequence 1:
  auto b = B();
  ASSERT_TRUE(b.setBoard(std::vector{0, 1, 2, 3, 4, 5, 6}));

  B::TBoardArray arr = {{{1, 0, 0, 0, 0, 0},  //
                         {2, 0, 0, 0, 0, 0},  //
                         {1, 0, 0, 0, 0, 0},  //
                         {2, 0, 0, 0, 0, 0},  //
                         {1, 0, 0, 0, 0, 0},  //
                         {2, 0, 0, 0, 0, 0},  //
                         {1, 0, 0, 0, 0, 0}}};

  auto bExpected = B();
  ASSERT_TRUE(bExpected.setBoard(arr));
  ASSERT_TRUE(bExpected == b);

  // Move Sequence 2:
  b = B();
  ASSERT_TRUE(b.setBoard(std::vector(6, 6)));

  arr = {{{0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 2, 1, 2, 1, 2}}};

  bExpected = B();
  ASSERT_TRUE(bExpected.setBoard(arr));
  ASSERT_TRUE(bExpected == b);

  // Move Sequence 3:
  b = B();
  ASSERT_FALSE(b.setBoard(std::vector(7, 6)));

  arr = {{{0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};

  bExpected = B();
  ASSERT_TRUE(bExpected.setBoard(arr));
  ASSERT_TRUE(bExpected == b);
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
TEST_F(BoardTest, isIllegalBit) {
  const std::vector<std::pair<int, int>> legal_ranges = {
      {0, 5}, {9, 14}, {18, 23}, {27, 32}, {36, 41}, {45, 50}, {54, 59}};
  for (int i = 0; i < 64; i++) {  // Assuming 64-bit board
    bool expected_illegal = true;
    for (const auto& [fst, snd] : legal_ranges) {
      if (i >= fst && i <= snd) {
        expected_illegal = false;
        break;
      }
    }
    EXPECT_EQ(BitBully::isIllegalBit(i), expected_illegal)
        << "Bit " << i << " did not match expected legality.";
  }
}

TEST_F(BoardTest, canWin) {
  using B = BitBully::Board;
  using time_point =
      std::chrono::time_point<std::chrono::high_resolution_clock>;
  using duration = std::chrono::duration<float>;

  // Create a random number generator
  std::random_device rd;   // Seed source
  std::mt19937 gen(rd());  // Mersenne Twister RNG seeded with rd()

  // Define a uniform distribution
  std::uniform_int_distribution<> distrib(0, B::N_COLUMNS - 1);

  float time1 = 0.0F, time2 = 0.0F;
  uint64_t counter = UINT64_C(0);
  for (auto i = 0; i < 10000; i++) {
    B b;
    GameSolver::Connect4::Position P;
    for (auto j = 0; j < B::N_COLUMNS * B::N_ROWS; ++j) {
      time_point tstart = std::chrono::high_resolution_clock::now();
      auto result1 = P.canWinNext();
      time_point tend = std::chrono::high_resolution_clock::now();
      float d = duration(tend - tstart).count();
      time1 += d;

      tstart = std::chrono::high_resolution_clock::now();
      auto result2 = b.canWin();
      tend = std::chrono::high_resolution_clock::now();
      d = duration(tend - tstart).count();
      time2 += d;

      ASSERT_EQ(result1, result2);
      counter++;
      int randColumn = distrib(gen);
      while (!P.canPlay(randColumn)) randColumn = distrib(gen);

      ASSERT_TRUE(b.playMove(randColumn));
      P.playCol(randColumn);

      if (P.isWinningMove(randColumn)) {
        break;
      }
    }
  }

  std::cout << "Time Pons: " << time1 << ". Time Mine: " << time2 << std::endl;
  std::cout << "Pos./s Pons: " << static_cast<double>(counter) / time1
            << ". Pos./s Mine: " << static_cast<double>(counter) / time2
            << std::endl;
}

TEST_F(BoardTest, canWin2) {
  using B = BitBully::Board;

  // Create a random number generator
  std::random_device rd;   // Seed source
  std::mt19937 gen(rd());  // Mersenne Twister RNG seeded with rd()

  // Define a uniform distribution
  std::uniform_int_distribution<> distrib(0, B::N_COLUMNS - 1);

  for (auto i = 0; i < 5000; i++) {
    B b;
    GameSolver::Connect4::Position P;
    for (auto j = 0; j < B::N_COLUMNS * B::N_ROWS; ++j) {
      // TODO: We need a random board generator...
      const auto canWin = b.canWin();

      for (int x = 0; x < B::N_COLUMNS && canWin; ++x) {
        if (b.isLegalMove(x)) {
          ASSERT_EQ(P.isWinningMove(x), b.canWin(x));
        }
      }

      int randColumn = distrib(gen);
      while (!P.canPlay(randColumn)) randColumn = distrib(gen);

      ASSERT_TRUE(b.playMove(randColumn)) << randColumn;
      P.playCol(randColumn);

      if (P.isWinningMove(randColumn)) {
        break;
      }
    }
  }
}

TEST_F(BoardTest, hasWon) {
  using B = BitBully::Board;

  // Create a random number generator
  std::random_device rd;   // Seed source
  std::mt19937 gen(rd());  // Mersenne Twister RNG seeded with rd()

  // Define a uniform distribution
  std::uniform_int_distribution<> distrib(0, B::N_COLUMNS - 1);

  for (auto i = 0; i < 10000; i++) {
    B b;
    for (auto j = 0; j < B::N_COLUMNS * B::N_ROWS; ++j) {
      const auto canWin = b.canWin();
      for (int x = 0; x < B::N_COLUMNS && canWin; ++x) {
        if (b.canWin(x)) {
          ASSERT_TRUE(b.playMove(x));
          ASSERT_TRUE(b.hasWin()) << b.toString();
          break;
        }
      }
      if (canWin) break;

      ASSERT_FALSE(b.hasWin()) << b.toString();

      int randColumn = distrib(gen);
      while (!b.playMove(randColumn)) randColumn = distrib(gen);
    }
  }
}

TEST_F(BoardTest, toString) {
  using B = BitBully::Board;
  B b;

  /* Generated by: genSetBoard */
  B::TBoardArray arr = {{{2, 1, 2, 2, 2, 1},  //
                         {2, 1, 1, 2, 1, 1},  //
                         {1, 2, 2, 1, 1, 2},  //
                         {1, 1, 2, 1, 2, 2},  //
                         {1, 2, 2, 1, 1, 0},  //
                         {2, 1, 1, 2, 2, 1},  //
                         {2, 1, 1, 2, 1, 2}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 1);

  std::cout << b.toString();

  arr = {{{1, 1, 1, 2, 0, 0},  //
          {1, 2, 1, 2, 0, 0},  //
          {1, 2, 2, 1, 0, 0},  //
          {2, 1, 2, 1, 2, 1},  //
          {1, 1, 1, 2, 1, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {2, 2, 2, 1, 2, 1}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 11);
  std::cout << b.toString();
}

TEST_F(BoardTest, toArray) {
  using B = BitBully::Board;

  /* Generated by: genSetBoard */
  B b;
  B::TBoardArray arr{{{2, 0, 0, 0, 0, 0},  //
                      {1, 0, 0, 0, 0, 0},  //
                      {0, 0, 0, 0, 0, 0},  //
                      {1, 0, 0, 0, 0, 0},  //
                      {0, 0, 0, 0, 0, 0},  //
                      {2, 0, 0, 0, 0, 0},  //
                      {2, 1, 1, 2, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  /* Generated by: genSetBoard */
  arr = {{{1, 1, 0, 0, 0, 0},  //
          {2, 2, 1, 0, 0, 0},  //
          {2, 1, 2, 2, 1, 1},  //
          {1, 2, 1, 2, 2, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 1, 2, 2, 1, 0},  //
          {2, 2, 2, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 0, 0, 0, 0, 0},  //
          {2, 1, 1, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 2, 1, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 1, 2, 2, 2, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 2, 1, 2, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 1, 2, 1, 2, 1}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 1, 1, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0},  //
          {1, 2, 2, 1, 1, 0},  //
          {2, 1, 2, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 1, 2, 1, 0, 0},  //
          {2, 2, 1, 2, 0, 0},  //
          {1, 1, 1, 2, 2, 0},  //
          {2, 1, 2, 1, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 1, 0, 0, 0},  //
          {1, 2, 2, 2, 1, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 1, 1, 2, 2, 1},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 2, 2, 1, 2, 2},  //
          {2, 1, 1, 2, 1, 2},  //
          {2, 2, 1, 2, 0, 0},  //
          {1, 1, 2, 1, 1, 2},  //
          {1, 1, 2, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 0, 0},  //
          {1, 1, 2, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 1, 2, 0, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 2, 1, 1, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {2, 2, 1, 0, 0, 0},  //
          {1, 2, 1, 2, 0, 0},  //
          {2, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 0, 0, 0, 0, 0},  //
          {2, 1, 1, 0, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {1, 1, 1, 2, 2, 2},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 1, 2, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 1, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 2, 1},  //
          {2, 2, 2, 0, 0, 0},  //
          {1, 1, 2, 1, 2, 1},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {2, 2, 1, 2, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 0, 0, 0, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{0, 0, 0, 0, 0, 0},  //
          {1, 1, 1, 2, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {2, 1, 1, 0, 0, 0},  //
          {2, 2, 2, 1, 0, 0},  //
          {1, 2, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 0, 0, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 2, 1, 1, 2, 1}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 1, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 2, 1, 2, 0},  //
          {1, 1, 1, 2, 2, 1},  //
          {2, 1, 2, 1, 1, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 1, 0, 0, 0, 0},  //
          {1, 2, 2, 1, 1, 1},  //
          {2, 0, 0, 0, 0, 0},  //
          {2, 1, 2, 2, 1, 2},  //
          {1, 1, 2, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 2, 1, 2, 2, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 0, 0, 0, 0, 0},  //
          {2, 2, 1, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 1, 1, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 1, 2, 2, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {2, 1, 2, 2, 0, 0},  //
          {1, 2, 1, 2, 2, 0},  //
          {1, 2, 1, 2, 0, 0},  //
          {2, 1, 1, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 1, 2, 2, 2, 1},  //
          {2, 1, 1, 2, 1, 1},  //
          {1, 2, 2, 1, 1, 2},  //
          {1, 1, 2, 1, 2, 2},  //
          {1, 2, 2, 1, 1, 2},  //
          {2, 1, 1, 2, 2, 1},  //
          {2, 1, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 1, 1, 1, 2, 1},  //
          {1, 1, 1, 2, 1, 2},  //
          {2, 2, 1, 1, 1, 2},  //
          {1, 1, 2, 1, 2, 1},  //
          {2, 2, 2, 1, 2, 2},  //
          {2, 1, 1, 2, 2, 2},  //
          {1, 2, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 2, 1, 1, 2, 1},  //
          {2, 1, 1, 2, 2, 1},  //
          {1, 2, 1, 2, 1, 2},  //
          {1, 2, 2, 2, 1, 2},  //
          {2, 1, 1, 1, 2, 1},  //
          {1, 2, 2, 2, 1, 1},  //
          {2, 2, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 2, 2, 1, 2, 2},  //
          {1, 1, 1, 2, 1, 2},  //
          {2, 2, 2, 1, 1, 2},  //
          {2, 1, 2, 1, 2, 1},  //
          {1, 1, 2, 1, 2, 2},  //
          {1, 2, 1, 2, 1, 1},  //
          {2, 1, 1, 2, 1, 1}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 1, 2, 2, 2, 1},  //
          {2, 1, 1, 2, 1, 1},  //
          {1, 2, 2, 1, 1, 2},  //
          {1, 1, 2, 1, 2, 2},  //
          {1, 2, 2, 1, 1, 0},  //
          {2, 1, 1, 2, 2, 1},  //
          {2, 1, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{1, 2, 1, 1, 2, 2},  //
          {2, 1, 2, 2, 1, 0},  //
          {1, 2, 1, 1, 2, 1},  //
          {2, 1, 1, 2, 1, 2},  //
          {1, 2, 2, 1, 1, 1},  //
          {2, 1, 2, 1, 1, 2},  //
          {2, 2, 1, 1, 2, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);

  arr = {{{2, 1, 1, 1, 2, 1},  //
          {1, 1, 1, 2, 1, 2},  //
          {2, 2, 1, 1, 1, 2},  //
          {1, 1, 2, 1, 2, 1},  //
          {2, 2, 2, 1, 2, 2},  //
          {2, 1, 1, 2, 2, 0},  //
          {1, 2, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
}

TEST_F(BoardTest, toArray2) {
  using B = BitBully::Board;
  B b;
  B::TBoardArray arr;

  /* Generated by: genSetBoard */
  arr = {{{2, 1, 2, 2, 2, 1},  //
          {2, 1, 1, 2, 1, 1},  //
          {1, 2, 2, 1, 1, 2},  //
          {1, 1, 2, 1, 2, 2},  //
          {1, 2, 2, 1, 1, 0},  //
          {2, 1, 1, 2, 2, 1},  //
          {2, 1, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 1);
  arr = {{{1, 2, 1, 1, 2, 2},  //
          {2, 1, 2, 2, 1, 0},  //
          {1, 2, 1, 1, 2, 1},  //
          {2, 1, 1, 2, 1, 2},  //
          {1, 2, 2, 1, 1, 1},  //
          {2, 1, 2, 1, 1, 2},  //
          {2, 2, 1, 1, 2, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 1);
  arr = {{{2, 1, 1, 1, 2, 1},  //
          {1, 1, 1, 2, 1, 2},  //
          {2, 2, 1, 1, 1, 2},  //
          {1, 1, 2, 1, 2, 1},  //
          {2, 2, 2, 1, 2, 2},  //
          {2, 1, 1, 2, 2, 0},  //
          {1, 2, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 1);

  arr = {{{2, 1, 2, 2, 2, 1},  //
          {2, 1, 1, 2, 1, 1},  //
          {1, 2, 2, 1, 1, 2},  //
          {1, 1, 2, 1, 2, 2},  //
          {1, 2, 2, 1, 1, 2},  //
          {2, 1, 1, 2, 2, 1},  //
          {2, 1, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 0);
  arr = {{{2, 1, 1, 1, 2, 1},  //
          {1, 1, 1, 2, 1, 2},  //
          {2, 2, 1, 1, 1, 2},  //
          {1, 1, 2, 1, 2, 1},  //
          {2, 2, 2, 1, 2, 2},  //
          {2, 1, 1, 2, 2, 2},  //
          {1, 2, 1, 2, 1, 2}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 0);

  arr = {{{2, 2, 1, 2, 1, 0},  //
          {1, 2, 2, 1, 2, 1},  //
          {1, 1, 2, 2, 2, 1},  //
          {1, 2, 0, 0, 0, 0},  //
          {2, 1, 1, 0, 0, 0},  //
          {2, 1, 2, 2, 1, 0},  //
          {1, 1, 2, 1, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 11);
  arr = {{{2, 1, 1, 2, 0, 0},  //
          {1, 1, 2, 2, 0, 0},  //
          {2, 1, 2, 2, 1, 1},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 2, 1, 0, 0, 0},  //
          {1, 1, 2, 2, 1, 2},  //
          {2, 2, 1, 1, 2, 1}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 11);
  arr = {{{2, 2, 1, 1, 0, 0},  //
          {1, 2, 1, 2, 2, 1},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 1, 2, 2, 1, 2},  //
          {2, 2, 1, 1, 2, 2},  //
          {1, 1, 1, 2, 1, 1}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 11);
  arr = {{{1, 1, 1, 2, 0, 0},  //
          {1, 2, 1, 2, 0, 0},  //
          {1, 2, 2, 1, 0, 0},  //
          {2, 1, 2, 1, 2, 1},  //
          {1, 1, 1, 2, 1, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {2, 2, 2, 1, 2, 1}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 11);
  arr = {{{2, 1, 2, 2, 1, 1},  //
          {2, 2, 1, 0, 0, 0},  //
          {2, 1, 2, 0, 0, 0},  //
          {1, 2, 1, 1, 0, 0},  //
          {1, 2, 1, 2, 1, 1},  //
          {1, 2, 1, 2, 1, 0},  //
          {2, 1, 2, 2, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 11);

  arr = {{{2, 2, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {1, 1, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 30);
  arr = {{{2, 2, 0, 0, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {1, 1, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 30);
  arr = {{{2, 2, 1, 2, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 2, 1, 0, 0, 0},  //
          {2, 1, 2, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 30);
  arr = {{{1, 2, 1, 0, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 2, 2, 0, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 30);

  /* Generated by: genSetBoard */
  arr = {{{0, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 41);
  arr = {{{0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 41);
  arr = {{{0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 41);

  arr = {{{0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 42);

  arr = {{{1, 1, 0, 0, 0, 0},  //
          {2, 2, 1, 0, 0, 0},  //
          {2, 1, 2, 2, 1, 1},  //
          {1, 2, 1, 2, 2, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 1, 2, 2, 1, 0},  //
          {2, 2, 2, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 16);
  arr = {{{1, 0, 0, 0, 0, 0},  //
          {2, 1, 1, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 2, 1, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 29);
  arr = {{{1, 1, 2, 2, 2, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 26);
  arr = {{{2, 2, 1, 2, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 1, 2, 1, 2, 1}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 25);
  arr = {{{2, 1, 1, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0},  //
          {1, 2, 2, 1, 1, 0},  //
          {2, 1, 2, 0, 0, 0},  //
          {2, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 26);
}
