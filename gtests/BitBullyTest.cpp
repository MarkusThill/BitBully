// Opening Book and transposition table could be the same table?
// TODO: use project-own google tests!!! Use git sub module!!!
// TODO: Simple neural net for move ordering? input board, output: 7-dim vector
// TODO: Use a simple logger. Use glog of google...
// TODO: Log computation times using a software version into a txt file...
// TODO: Play n games against a random (or more advanced) player: It has to win
// every single game! ...
// TODO: Github CI/CD Pipeline
// TODO: Namespace for Pons/FierzC4??

#include <chrono>

#include "BitBully.h"
#include "Board.h"
#include "Solver.hpp"
#include "gtest/gtest.h"

#ifdef _WIN32  // Check if we're on a Windows platform
using Clock = std::chrono::steady_clock;  // Use steady_clock on Windows
#else
using Clock = std::chrono::high_resolution_clock;  // Use high_resolution_clock
                                                   // on other platforms
#endif

class BitBullyTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // GTEST_SKIP() << "Skipping this file for now";
  }

  void TearDown() override {}

  ~BitBullyTest() override {
    // resources cleanup, no exceptions allowed
  }
};

TEST_F(BitBullyTest, comparePonsBitbully) {
  using B = BitBully::Board;
  GameSolver::Connect4::Solver solver;
  BitBully::BitBully bb;

  for (auto i = 0; i < 50; i++) {
    B b;
    GameSolver::Connect4::Position P;
    int j;
    for (j = 0; j < 15; ++j) {  // TODO: We need a random board generator...
      int randColumn = rand() % 7;
      while (!P.canPlay(randColumn)) randColumn = rand() % 7;

      if (P.isWinningMove(randColumn)) {
        break;
      }
      ASSERT_TRUE(b.play(randColumn));
      P.playCol(randColumn);
    }

    if (j != 15) continue;

    int scorePons = solver.solve(P, false);
    int scoreMine = bb.mtdf(b, 0);

    auto scoresMine = bb.scoreMoves(b);
    auto scoresPons = solver.analyze(P, false);
    ASSERT_EQ(scoresMine, scoresPons) << b.toString();

    ASSERT_EQ(scorePons, scoreMine)
        << "Error: " << b.toString() << "Pons: " << scorePons
        << " Mine: " << scoreMine << std::endl;
  }
}

TEST_F(BitBullyTest, test2) {
  using B = BitBully::Board;
  using time_point = std::chrono::time_point<Clock>;
  using duration = std::chrono::duration<float>;
  float time1 = 0.0F, time2 = 0.0F;

  GameSolver::Connect4::Solver solver;
  BitBully::BitBully bb;

  for (auto i = 0; i < 5 * 0.5; i++) {
    B b;
    GameSolver::Connect4::Position P;
    // std::cout << std::endl << "MoveSequence:";
    int j;
    for (j = 0; j < 12; ++j) {  // TODO: We need a random board generator...
      int randColumn = rand() % 7;
      while (!P.canPlay(randColumn)) randColumn = rand() % 7;

      if (P.isWinningMove(randColumn)) {
        break;
      }
      ASSERT_TRUE(b.play(randColumn));
      P.playCol(randColumn);
    }

    if (j != 12) continue;

    // std::cout << b.toString();

    auto tstart = Clock::now();
    int scorePons = solver.solve(P, false);
    auto tend = Clock::now();
    auto d = float(duration(tend - tstart).count());
    time1 += d;

    tstart = Clock::now();
    int scoreMine = bb.negamax(b, -100000, 100000, 0);
    // int scoreMine = bb.mtdf(b, 0);
    tend = Clock::now();
    d = float(duration(tend - tstart).count());
    time2 += d;

    // std::cout << "Pons: " << scorePons << " Mine: " << scoreMine <<
    // std::endl;
    ASSERT_EQ(scorePons, scoreMine)
        << "Error: " << b.toString() << "Pons: " << scorePons
        << " Mine: " << scoreMine << std::endl;
  }
  std::cout << "Time Pons: " << time1 << ". Time Mine: " << time2
            << "; Diff: " << time1 - time2 << std::endl;
}

TEST_F(BitBullyTest, comparePonsBitbullyTime) {
  using B = BitBully::Board;
  using duration = std::chrono::duration<float>;
  float time1 = 0.0F, time2 = 0.0F;

  GameSolver::Connect4::Solver solver;
  BitBully::BitBully bb;

  srand(42);

  for (auto i = 0; i < 5 * 0.5; i++) {
    B b;
    GameSolver::Connect4::Position P;
    // std::cout << std::endl << "MoveSequence:";
    int j;
    for (j = 0; j < 12; ++j) {  // TODO: We need a random board generator...
      int randColumn = rand() % 7;
      while (!P.canPlay(randColumn)) randColumn = rand() % 7;

      if (P.isWinningMove(randColumn)) {
        break;
      }
      ASSERT_TRUE(b.play(randColumn));
      P.playCol(randColumn);
      // std::cout << (randColumn + 1);
    }

    if (j != 12) continue;
    if (P.canWinNext()) continue;

    // bb.resetTranspositionTable();
    auto tstart = Clock::now();
    int scoreMine = bb.mtdf(b, 0);
    // int scoreMine = bb.nullWindow(b);
    auto tend = Clock::now();
    auto d2 = float(duration(tend - tstart).count());
    time2 += d2;

    tstart = Clock::now();
    int scorePons = solver.solve(P, false);
    tend = Clock::now();
    auto d1 = float(duration(tend - tstart).count());
    time1 += d1;

    std::cout << "Time Pons: " << time1 << ". Time Mine: " << time2
              << "; Diff: " << time2 - time1
              << " sec. Percent: " << (time2 - time1) / time2 * 100.0 << " %"
              << std::endl;

    std::cout << "Node Count Pons: " << solver.getNodeCount() << ", "
              << "Mine: " << bb.getNodeCounter() << " Percent: "
              << double(bb.getNodeCounter() - solver.getNodeCount()) /
                     bb.getNodeCounter() * 100.0
              << " %" << std::endl;

    // std::cout << "Pons: " << scorePons << " Mine: " << scoreMine <<
    // std::endl;
    ASSERT_EQ(scorePons, scoreMine)
        << "Error: " << b.toString() << "Pons: " << scorePons
        << " Mine: " << scoreMine << std::endl;
  }
}

TEST_F(BitBullyTest, mtdfWithBook) {
  auto bookPath =
      std::filesystem::path("../gtests/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));

  using B = BitBully::Board;

  BitBully::BitBully bb(bookPath);
  ASSERT_TRUE(bb.isBookLoaded());

  const auto expectedValues = {
      1,   // empty board
      2,   // yellow plays (leftmost) column "a"
      1,   // yellow plays column "b"
      0,   // yellow plays column "c"
      -1,  // yellow plays (center) column "d"
      0,   // yellow plays column "e"
      1,   // yellow plays column "f"
      2    // yellow plays (rightmost) column "g"
  };

  // Start with -1, since we want to keep the first board empty
  auto itExp = expectedValues.begin();
  for (int i = -1; i < B::N_COLUMNS; ++i) {
    bb.resetTranspositionTable();  // For fair comparison of times
    B b;
    b.play(i);
    ASSERT_EQ(b.countTokens(), (i >= 0 ? 1 : 0));
    using duration = std::chrono::duration<float>;
    const auto tstart = Clock::now();
    const auto bitbullyValue = bb.mtdf(b, 0);
    const auto tend = Clock::now();
    const auto d = float(duration(tend - tstart).count());
    std::cout << b.toString() << "GTV: " << bitbullyValue << ". time: " << d
              << "\n";
    EXPECT_EQ(bitbullyValue, *itExp++);
  }
}
