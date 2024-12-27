// Opening Book and transposition table could be the same table?
// TODO: use project-own google tests!!! Use git sub module!!!
// TODO: Simple neural net for move ordering? input board, output: 7-dim vector
// TODO: Use a simple logger. Use glog of google...
// TODO: Log computation times using a software version into a txt file...
// TODO: Play n games against a random (or more advanced) player: It has to win
// every single game! ...
// TODO: Github CI/CD Pipeline
// TODO: Namespace for Pons/FierzC4??

#include "BitBully.h"
#include "Board.h"
#include "Solver.hpp"
#include "gtest/gtest.h"

class BitBullyTest : public ::testing::Test {

protected:
  void SetUp() override
  {
    GTEST_SKIP() << "Skipping this file for now";
  }

  void TearDown() override {}

  ~BitBullyTest() override {
    // resources cleanup, no exceptions allowed
  }
};

TEST_F(BitBullyTest, test1) {
  using B = BitBully::Board;
  using time_point =
      std::chrono::time_point<std::chrono::high_resolution_clock>;
  using duration = std::chrono::duration<float>;
  float time1 = 0.0F, time2 = 0.0F;

  GameSolver::Connect4::Solver solver;
  BitBully::BitBully bb;

  for (auto i = 0; i < 5 * 1; i++) {
    B b;
    GameSolver::Connect4::Position P;
    // std::cout << std::endl << "MoveSequence:";
    int j;
    for (j = 0; j < 16; ++j) { // TODO: We need a random board generator...
      int randColumn = rand() % 7;
      while (!P.canPlay(randColumn))
        randColumn = rand() % 7;

      if (P.isWinningMove(randColumn)) {
        break;
      }
      ASSERT_TRUE(b.playMove(randColumn));
      P.playCol(randColumn);
      // std::cout << (randColumn + 1);
    }

    if (j != 16)
      continue;

    // std::cout << b.toString();

    auto tstart = std::chrono::high_resolution_clock::now();
    int scorePons = solver.solve(P, false);
    auto tend = std::chrono::high_resolution_clock::now();
    auto d = float(duration(tend - tstart).count());
    time1 += d;

    tstart = std::chrono::high_resolution_clock::now();
    // int scoreMine = bb.negamax(b, -100000, 100000, 0);
    int scoreMine = bb.mtdf(b, 0);
    tend = std::chrono::high_resolution_clock::now();
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

TEST_F(BitBullyTest, test2) {
  using B = BitBully::Board;
  using time_point =
      std::chrono::time_point<std::chrono::high_resolution_clock>;
  using duration = std::chrono::duration<float>;
  float time1 = 0.0F, time2 = 0.0F;

  GameSolver::Connect4::Solver solver;
  BitBully::BitBully bb;

  for (auto i = 0; i < 5 * 1; i++) {
    B b;
    GameSolver::Connect4::Position P;
    // std::cout << std::endl << "MoveSequence:";
    int j;
    for (j = 0; j < 12; ++j) { // TODO: We need a random board generator...
      int randColumn = rand() % 7;
      while (!P.canPlay(randColumn))
        randColumn = rand() % 7;

      if (P.isWinningMove(randColumn)) {
        break;
      }
      ASSERT_TRUE(b.playMove(randColumn));
      P.playCol(randColumn);
      // std::cout << (randColumn + 1);
    }

    if (j != 12)
      continue;

    // std::cout << b.toString();

    auto tstart = std::chrono::high_resolution_clock::now();
    int scorePons = solver.solve(P, false);
    auto tend = std::chrono::high_resolution_clock::now();
    auto d = float(duration(tend - tstart).count());
    time1 += d;

    tstart = std::chrono::high_resolution_clock::now();
    // int scoreMine = bb.negamax(b, -100000, 100000, 0);
    int scoreMine = bb.mtdf(b, 0);
    tend = std::chrono::high_resolution_clock::now();
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

TEST_F(BitBullyTest, test3) {
  using B = BitBully::Board;
  using duration = std::chrono::duration<float>;
  float time1 = 0.0F, time2 = 0.0F;

  GameSolver::Connect4::Solver solver;
  BitBully::BitBully bb;

  for (auto i = 0; i < 5 * 1; i++) {
    B b;
    GameSolver::Connect4::Position P;
    // std::cout << std::endl << "MoveSequence:";
    int j;
    for (j = 0; j < 12; ++j) { // TODO: We need a random board generator...
      int randColumn = rand() % 7;
      while (!P.canPlay(randColumn))
        randColumn = rand() % 7;

      if (P.isWinningMove(randColumn)) {
        break;
      }
      ASSERT_TRUE(b.playMove(randColumn));
      P.playCol(randColumn);
      // std::cout << (randColumn + 1);
    }

    if (j != 12)
      continue;

    // std::cout << b.toString();

    auto tstart = std::chrono::high_resolution_clock::now();
    int scorePons = solver.solve(P, false);
    auto tend = std::chrono::high_resolution_clock::now();
    auto d1 = float(duration(tend - tstart).count());
    time1 += d1;
    // std::cout << "Pons: " << d1 * 1000 << "ms ; ";
    if (i % 100 == 0) {
      bb.resetTranspositionTable();
      std::cout << "Reset transposition table" << std::endl;
    }

    tstart = std::chrono::high_resolution_clock::now();
    // int scoreMine = bb.negamax(b, -100000, 100000, 0);
    int scoreMine = bb.mtdf(b, 0);
    // int scoreMine = bb.iterativeDeepening(b);
    // int scoreMine = bb.nullWindow(b);
    tend = std::chrono::high_resolution_clock::now();
    auto d2 = float(duration(tend - tstart).count());
    time2 += d2;
    // std::cout << " Mine: " << d2 * 1000 << "ms ; Diff: " << (d2 - d1) * 1000
    //           << " ms. Percent: " << (d2 - d1) / d2 * 100.0 << " %"
    //           << std::endl;
    std::cout << "Time Pons: " << time1 << ". Time Mine: " << time2
              << "; Diff: " << time2 - time1
              << " sec. Percent: " << (time2 - time1) / time2 * 100.0 << " %"
              << std::endl;

    // std::cout << "Pons: " << scorePons << " Mine: " << scoreMine <<
    // std::endl;
    ASSERT_EQ(scorePons, scoreMine)
        << "Error: " << b.toString() << "Pons: " << scorePons
        << " Mine: " << scoreMine << std::endl;
  }
}