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

// TODO: This Test is mostly incomplete...
TEST_F(BitBullyTest, scoreMove) {
  auto bookPath =
      std::filesystem::path("../gtests/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));

  using B = BitBully::Board;

  BitBully::BitBully bb(bookPath);
  ASSERT_TRUE(bb.isBookLoaded());

  B b;
  // Case 1: Empty board
  const auto expectedValues_1 = {-2, -1, 0, 1, 0, -1, -2};
  auto itExp = expectedValues_1.begin();
  for (int i = 0; i < B::N_COLUMNS; ++i) {
    auto score = bb.scoreMove(b, i, 0);
    EXPECT_EQ(score, *itExp++);
  }

  // Case 2: After "333331111"
  ASSERT_TRUE(b.play("333331111"));
  const auto expectedValues_2 = {-3, -3, -2, -1, -3, -1, -1};
  itExp = expectedValues_2.begin();
  for (int i = 0; i < B::N_COLUMNS; ++i) {
    auto score = bb.scoreMove(b, i, 0);
    EXPECT_EQ(score, *itExp++);
  }
}

// ============================================================
// Rollout & maxDepth tests
// ============================================================

TEST_F(BitBullyTest, rolloutEmptyBoard) {
  using B = BitBully::Board;
  B b;
  int score = BitBully::BitBully::rollout(b);
  EXPECT_GE(score, -21);
  EXPECT_LE(score, 21);
}

TEST_F(BitBullyTest, rolloutImmediateWin) {
  // Build a position where the current player can win immediately
  using B = BitBully::Board;
  B b;
  // P1: col 3 x3 (vertical), P2: col 2 x2 + col 4
  ASSERT_TRUE(b.play("323234"));
  ASSERT_TRUE(b.canWin());  // P1 can win with col 3
  int score = BitBully::BitBully::rollout(b);
  EXPECT_GT(score, 0);
}

TEST_F(BitBullyTest, rolloutknownSequence) {
  // Build a position where the current player can win immediately
  using B = BitBully::Board;
  B b;
  // P1: col 3 x3 (vertical), P2: col 2 x2 + col 4
  ASSERT_TRUE(b.play("33333045555312221111505106600444404"));
  int score = BitBully::BitBully::rollout(b);
  EXPECT_EQ(score, -1);
}

TEST_F(BitBullyTest, rolloutScoreInValidRange) {
  using B = BitBully::Board;
  srand(42);
  for (int trial = 0; trial < 50; trial++) {
    auto [board, moves] = B::randomBoard(10 + trial % 30, true);
    int score = BitBully::BitBully::rollout(board);
    int bound = (board.movesLeft() + 1) / 2;
    EXPECT_GE(score, -bound) << "trial=" << trial << " " << board.toString();
    EXPECT_LE(score, bound) << "trial=" << trial << " " << board.toString();
  }
}

TEST_F(BitBullyTest, rolloutDeterministic) {
  using B = BitBully::Board;
  B b;
  ASSERT_TRUE(b.play("334411"));
  int score1 = BitBully::BitBully::rollout(b);
  int score2 = BitBully::BitBully::rollout(b);
  EXPECT_EQ(score1, score2);
}

TEST_F(BitBullyTest, maxDepthMinusOneMatchesFullSearch) {
  using B = BitBully::Board;
  BitBully::BitBully bb;
  B b;
  ASSERT_TRUE(b.play("334411"));

  bb.resetTranspositionTable();
  int scoreDefault = bb.negamax(b, -100000, 100000, 0);
  bb.resetTranspositionTable();
  int scoreExplicit = bb.negamax(b, -100000, 100000, 0, -1);
  EXPECT_EQ(scoreDefault, scoreExplicit);
}

TEST_F(BitBullyTest, maxDepthZeroIsRollout) {
  // maxDepth=0 means negamax immediately calls rollout at the root
  using B = BitBully::Board;
  BitBully::BitBully bb;
  B b;
  ASSERT_TRUE(b.play("334411"));

  bb.resetTranspositionTable();
  int scoreNegamax = bb.negamax(b, -100000, 100000, 0, 0);
  int scoreRollout = BitBully::BitBully::rollout(b);
  EXPECT_EQ(scoreNegamax, scoreRollout);
}

TEST_F(BitBullyTest, depthLimitedVisitsFewerNodes) {
  using B = BitBully::Board;
  BitBully::BitBully bb;

  // Use randomBoard which guarantees no immediate win
  srand(42);
  auto [b, moves] = B::randomBoard(11, true);

  // Use negamax with a wide window to force full tree exploration
  bb.resetNodeCounter();
  bb.resetTranspositionTable();
  bb.negamax(b, -100000, 100000, 0, 5);
  auto nodesLimited = bb.getNodeCounter();

  bb.resetNodeCounter();
  bb.resetTranspositionTable();
  bb.negamax(b, -100000, 100000, 0, -1);
  auto nodesFull = bb.getNodeCounter();

  EXPECT_GT(nodesLimited, 0ULL) << "Depth-limited should visit some nodes";
  EXPECT_GT(nodesFull, 0ULL) << "Full search should visit some nodes";
  EXPECT_LT(nodesLimited, nodesFull)
      << "Limited: " << nodesLimited << " Full: " << nodesFull;
}

TEST_F(BitBullyTest, depthLimitedScoreInValidRange) {
  using B = BitBully::Board;
  BitBully::BitBully bb;
  srand(123);
  for (int trial = 0; trial < 20; trial++) {
    auto [board, moves] = B::randomBoard(10, true);
    for (int maxDepth = 0; maxDepth <= 10; maxDepth += 2) {
      bb.resetTranspositionTable();
      int score = bb.mtdf(board, 0, maxDepth);
      int bound = (board.movesLeft() + 1) / 2;
      EXPECT_GE(score, -bound) << "trial=" << trial << " maxDepth=" << maxDepth;
      EXPECT_LE(score, bound) << "trial=" << trial << " maxDepth=" << maxDepth;
    }
  }
}

TEST_F(BitBullyTest, depthLimitedConvergesToExact) {
  // When maxDepth exceeds the remaining game length, the result
  // should match full search
  using B = BitBully::Board;
  BitBully::BitBully bb;
  B b;
  ASSERT_TRUE(b.play("33221144005566"));  // 14 tokens, 28 left

  bb.resetTranspositionTable();
  int scoreExact = bb.mtdf(b, 0, -1);

  bb.resetTranspositionTable();
  int scoreDeep = bb.mtdf(b, 0, 40);  // 40 > 28 moves left
  EXPECT_EQ(scoreDeep, scoreExact);
}

TEST_F(BitBullyTest, depthLimitedDoesNotPolluteTT) {
  using B = BitBully::Board;
  BitBully::BitBully bb;
  B b;
  ASSERT_TRUE(b.play("334411"));

  // Full search first (reference)
  bb.resetTranspositionTable();
  int scoreFullBefore = bb.mtdf(b, 0, -1);

  // Depth-limited search (stores approximate entries in TT)
  bb.mtdf(b, 0, 3);

  // Full search again — TT entries from depth-limited search have lower
  // searchDepth and must be ignored by the full search
  int scoreFullAfter = bb.mtdf(b, 0, -1);
  EXPECT_EQ(scoreFullBefore, scoreFullAfter);
}

TEST_F(BitBullyTest, scoreMovesWithMaxDepth) {
  using B = BitBully::Board;
  BitBully::BitBully bb;
  B b;

  auto scores = bb.scoreMoves(b, 5);
  ASSERT_EQ(scores.size(), static_cast<size_t>(B::N_COLUMNS));

  for (int col = 0; col < B::N_COLUMNS; col++) {
    if (scores[col] > -100) {
      int bound = (b.movesLeft() + 1) / 2;
      EXPECT_GE(scores[col], -bound) << "col=" << col;
      EXPECT_LE(scores[col], bound) << "col=" << col;
    }
  }
}

TEST_F(BitBullyTest, scoreMovesWithMaxDepth2) {
  using B = BitBully::Board;
  BitBully::BitBully bb;

  for (int depth = 3; depth < 16; depth++) {
    B b;
    ASSERT_TRUE(b.play("3343452"));
    auto scores = bb.scoreMoves(b, depth);
    ASSERT_EQ(scores.size(), static_cast<size_t>(B::N_COLUMNS));

    scores.at(1) = 99;  // We do not know the true value
    ASSERT_EQ(scores, decltype(scores)({-17, 99, -17, -17, -17, -17, -17}));
  }
}

TEST_F(BitBullyTest, scoreMovesWithMaxDepth3) {
  using B = BitBully::Board;
  BitBully::BitBully bb;

  for (int depth = 1; depth < 16; depth++) {
    B b;
    ASSERT_TRUE(b.play("343303342141440222"));  // Will loose in a few moves
                                                // even with limited depth
    auto scores = bb.scoreMoves(b, depth);
    ASSERT_EQ(scores.size(), static_cast<size_t>(B::N_COLUMNS));

    EXPECT_EQ(scores, decltype(scores)({-12, -12, -12, -12, -12, -11, -12}));
  }
}

TEST_F(BitBullyTest, scoreMovesWithMaxDepth4) {
  using B = BitBully::Board;
  BitBully::BitBully bb;

  for (int depth = 3; depth < 16; depth++) {
    B b;
    ASSERT_TRUE(b.play("33432"));  // double threat
    auto scores = bb.scoreMoves(b, depth);
    ASSERT_EQ(scores.size(), static_cast<size_t>(B::N_COLUMNS));

    ASSERT_EQ(scores, std::vector<int>(7, -18));
  }
}

TEST_F(BitBullyTest, scoreMovesWithMaxDepth5) {
  using B = BitBully::Board;
  BitBully::BitBully bb;

  for (int depth = 6; depth < 16; depth++) {
    B b;
    ASSERT_TRUE(b.play("3510131133424344"));  // double threat at some point
    auto scores = bb.scoreMoves(b, depth);
    ASSERT_EQ(scores.size(), static_cast<size_t>(B::N_COLUMNS));

    // EXPECT_EQ(scores, decltype(scores)({3,11,4,4,4,9,3}));
    EXPECT_EQ(scores.at(1), 11) << "Depth: " << depth;
    EXPECT_EQ(scores.at(5), 9) << "Depth: " << depth;
  }
}

TEST_F(BitBullyTest, ignoreOldTranspositionsForPerfectPlay) {
  using B = BitBully::Board;
  BitBully::BitBully bb;
  BitBully::BitBully bb_reference;
  srand(42);
  for (int trial = 0; trial < 50; trial++) {
    auto [board, moves] = B::randomBoard(11 + trial % 10, true);
    for (int subTrial = 0; subTrial < 50; subTrial++) {
      int score = bb.negamax(board, -100000, 100000, 0, 1 + subTrial % 7);
      score = bb.mtdf(board, score, 1 + subTrial % 3);
      score = bb.nullWindow(board, 1 + subTrial % 5);
      bb.scoreMoves(board, 1 + subTrial % 7);
    }

    // Transposition table entries from previous searches should not affect the
    // result of a perfect play search (e.g., scoreMoves with large maxDepth)
    // on the same board position, even if the previous searches were done
    // with a smaller maxDepth which stores approximate values in the TT.
    // This is important to ensure that the agent always plays the best move
    // according to the current search, and is not misled by old TT entries
    // from previous searches with smaller maxDepth.
    EXPECT_EQ(bb.scoreMoves(board, -1), bb_reference.scoreMoves(board, -1))
        << "trial=" << trial << " " << board.toString();
  }
}
