// TODO: use project-own google tests!!! Use git sub module!!!
// TODO: Use a simple logger. Use glog of google...
// TODO: Log computation times using a software version into a txt file...
// TODO: Play n games against a random (or more advanced) player: It has to win
// every single game! ...
// TODO: clang format!
// TODO: Github CI/CD Pipeline
// TODO: Namespace for Pons/FierzC4??
// TODO: Use git sub module for google test and for Pons/Fierz

#include "Solver.hpp"
#include "cpart.cpp"
#include "gtest/gtest.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <numeric>

using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
using duration = std::chrono::duration<float>;
namespace fs = std::filesystem;

class VerificationTest : public ::testing::Test {

protected:
  void SetUp() override {
    // TODO: We need a constructor
    c4.Reset(); // entleert das Spielfeld. Die Variablen m_fieldP1 und m_fieldP2
                // werden auf 0 gesetzt.
    c4.BuchLaden("../../book.dat"); // Die Er�ffnungsdatenbank wird direkt zu
                                    // beginn aus der Datei eingelesen
    c4.BuchLaden2("../../data8ply.dat");
    c4.setMaxInstance(
        100); // Der Standardwert fÜr die Schwierigkeitsstufe wird gesetzt. Die
              // Schwierigkeitsstufe ergibt sich aus (m_maxSearchDepth-2)/2
    c4.initHash();
    c4.ResetHash();

    std::cout << "Version: v" << VERSION_MAJOR << "." << VERSION_MINOR << "."
              << VERSION_PATCH << std::endl;
  }

  void TearDown() override {}

  ~VerificationTest() override {
    // resources cleanup, no exceptions allowed
  }

  long sgn(long x) { return (x > 0) - (x < 0); }

  Connect4 c4;
};

TEST_F(VerificationTest, toMoveSequence) {
  // TODO: Test empty board
  srand(static_cast<uint32_t>(time(nullptr)));
  for (auto i = 0UL; i < 1000; i++) {
    c4.Reset();
    c4.setFeld(0LL, 0LL);
    c4.ResetHash();
    c4.HoeheErmitteln();
    c4.setDepthTie();
    c4.setMaxInstance(100);

    auto nPieces = rand() % 42;

    bool player = false; // TODO: Vey vey ugly
    // while(!c4.isGameOver()) {
    for (auto j = 0; j < nPieces; ++j) {
      // TODO: Produces a lot of illegal positions. Worry?
      int randColumn = rand() % 7;
      while (c4.isColumnFull(randColumn))
        randColumn = rand() % 7;
      c4.SteinSetzen(randColumn, player);
      player = !player;
    }
    // std::cout << std::endl << c4.toString() ;

    auto moveSequence = c4.toMoveSequence();
    Connect4 c4New;
    c4New.Reset();
    c4New.setFeld(0LL, 0LL);
    c4New.HoeheErmitteln();
    c4New.setDepthTie();
    c4New.setMaxInstance(100);
    player = false; // TODO: Vey vey ugly
    for (auto m : moveSequence) {
      c4New.SteinSetzen(m, player);
      player = !player;
    }
    // std::cout << std::endl << c4New.toString() << std::endl;

    // TODO: Implement == operator: c4New == c4
    ASSERT_TRUE(c4New.equals(c4));
    ASSERT_TRUE(c4.equals(c4New));
  }
}

TEST_F(VerificationTest, randomOpponent) {
  srand(static_cast<uint32_t>(time(nullptr)));
  for (auto i = 0UL; i < 10; i++) {
    c4.Reset();
    c4.setFeld(0LL, 0LL);
    c4.ResetHash();
    c4.HoeheErmitteln();
    c4.setDepthTie();
    c4.setMaxInstance(100);

    c4.SteinSetzen(3, false); // false = bYellow

    int winner = 0;
    while (c4.BrettCount() < 41) {
      int randColumn =
          rand() % 7; // TODO: We need a random move generator for c4
      while (c4.isColumnFull(randColumn))
        randColumn = rand() % 7;
      if (c4.Gewinnstellung1(randColumn, c4.getHeight(randColumn))) {
        winner = 1;
        break;
      }
      c4.SteinSetzen(randColumn, true); // true = Red
      c4.HoeheErmitteln();

      c4.setDepthTie();
      c4.setMaxInstance(100);
      auto mv = c4.WurzelMethodeStartComputerAnziehender(0, -9999, 9999,
                                                         c4.HoeheErmitteln());
      if (c4.Gewinnstellung2(mv, c4.getHeight(mv))) {
        winner = 2;
        break;
      }
      c4.SteinSetzen(mv, false); // false = bYellow
    }
    ASSERT_EQ(winner, 2);
  }
}

TEST_F(VerificationTest, fastVerification) {
  ;
  time_point time_start = std::chrono::high_resolution_clock::now();
  for (auto i = 0; i < 86892; i += 1000) { // TODO: Hard-coded number!
    auto entry = c4.getOpening(i);
    c4.setFeld(entry.m_positionP1, entry.m_positionP2);
    c4.ResetHash();
    c4.HoeheErmitteln();

    c4.setDepthTie();
    c4.setMaxInstance(100);
    auto [x, _] = c4.WurzelMethodeComputerAnziehender(0, -9999, 9999,
                                                      c4.HoeheErmitteln());
    EXPECT_FALSE(x == 1000 && entry.m_value != 2) << "For position: " << i;
    EXPECT_FALSE(x == -1000 && entry.m_value != 1) << "For position: " << i;
    EXPECT_FALSE(x == 0 && entry.m_value != 0) << "For position: " << i;
    EXPECT_FALSE(x != -1000 && x != 1000 && x != 0) << "For position: " << i;
    if (i % 1000 == 0)
      std::cout << "Done with " << i << std::endl;
  }
  time_point time_end = std::chrono::high_resolution_clock::now();

  float dur = float(duration(time_end - time_start).count());
  std::cout << "Total Time for fastVerificationTest(): " << dur << " seconds"
            << std::endl;
}

TEST_F(VerificationTest, ponsC4Verification8Ply) {

  time_point time_start = std::chrono::high_resolution_clock::now();

  // Pons C4 Setup
  using namespace GameSolver::Connect4;
  Solver solver;
  bool weak = false;
  bool analyze = false;
  std::vector<float> timesPons, timesMine;
  for (auto i = 0; i < 86892; i += 1000) { // TODO: Hard-coded number!
    auto entry = c4.getOpening(i);

    // Get result from C4
    c4.setFeld(entry.m_positionP1, entry.m_positionP2);
    c4.ResetHash(); // TODO: necessary?
    c4.HoeheErmitteln();
    c4.setDepthTie();
    c4.setMaxInstance(100);
    auto moveSequence = c4.toMoveSequence();

    time_point tstart = std::chrono::high_resolution_clock::now();
    auto [x, _] = c4.WurzelMethodeComputerAnziehender(0, -9999, 9999,
                                                      c4.HoeheErmitteln());
    time_point tend = std::chrono::high_resolution_clock::now();
    float d = float(duration(tend - tstart).count());
    timesMine.push_back(d);

    // Get result from Pons C4
    // std::string line{"44444222"};
    Position P;
    // P.play(line);
    for (const auto &mv : moveSequence) {
      ASSERT_TRUE(P.canPlay(mv));
      P.playCol(mv);
    }

    /*
        if(P.play(line) != line.size()) {
          std::cerr << "Line " << 1 << ": Invalid move " << (P.nbMoves() + 1) <<
       " \"" << line << "\"" << std::endl; } else { std::cout << line;
          if(analyze) {
            std::vector<int> scores = solver.analyze(P, weak);
            for(int i = 0; i < Position::WIDTH; i++) std::cout << " " <<
       scores[i];
          }
          else {
            int score = solver.solve(P, weak);
            std::cout << " " << score;
          }
          std::cout << std::endl;
        }*/
    // std::cout << "Running Pons: " << std::endl;
    solver.reset(); // TODO: necessary/helpful?
    tstart = std::chrono::high_resolution_clock::now();
    int score = solver.solve(P, weak);
    tend = std::chrono::high_resolution_clock::now();
    d = float(duration(tend - tstart).count());
    timesPons.push_back(d);

    // std::cout << "Mine: " << x  << " Pons: " << score << " Database: " <<
    // entry.m_value << std::endl;
    std::cout << "Time mine: " << timesMine.back()
              << ". Time PonsC4: " << timesPons.back() << std::endl;

    EXPECT_TRUE(sgn(x) == sgn(score)) << "For position: " << i;

    EXPECT_FALSE(x == 1000 && entry.m_value != 2) << "For position: " << i;
    EXPECT_FALSE(x == -1000 && entry.m_value != 1) << "For position: " << i;
    EXPECT_FALSE(x == 0 && entry.m_value != 0) << "For position: " << i;
    EXPECT_FALSE(x != -1000 && x != 1000 && x != 0) << "For position: " << i;
    if (i % 1000 == 0)
      std::cout << "Done with " << i << std::endl;
  }
  time_point time_end = std::chrono::high_resolution_clock::now();

  auto sumTimesMine = std::accumulate(timesMine.begin(), timesMine.end(), 0.0f);
  auto sumTimesPons = std::accumulate(timesPons.begin(), timesPons.end(), 0.0f);
  std::cout << "Time Total mine: " << sumTimesMine
            << ". Time Total PonsC4: " << sumTimesPons << std::endl;
  // TODO: in how many cases am I faster than PonsC4?
  float dur = float(duration(time_end - time_start).count());
  std::cout << "Total Time for fastVerificationTest(): " << dur << " seconds"
            << std::endl;
}

TEST_F(VerificationTest, ponsC4VerificationXPly) {

  time_point time_start = std::chrono::high_resolution_clock::now();
  // using namespace GameSolver::Connect4;
  GameSolver::Connect4::Solver solver;
  bool weak = false;
  std::vector<float> timesPons, timesMine;
  for (auto i = 0; i < 500; i++) {
    c4.Reset();
    c4.setFeld(0LL, 0LL);

    auto nPieces = (rand() % 10) * 2 + 8;

    bool player = false;
    std::vector<int> moveSequence;
    int j;
    for (j = 0; j < nPieces; ++j) { // TODO: We need a random board generator...
      int randColumn = rand() % 7;
      while (c4.isColumnFull(randColumn))
        randColumn = rand() % 7;
      if (player && c4.Gewinnstellung1(randColumn, c4.getHeight(randColumn)))
        break;
      if (!player && c4.Gewinnstellung2(randColumn, c4.getHeight(randColumn)))
        break;
      c4.SteinSetzen(randColumn, player);
      player = !player;
      moveSequence.push_back(randColumn);
    }
    if (j != nPieces)
      continue;
    if (c4.SpielBeenden2())
      continue;

    ASSERT_EQ(moveSequence.size(), nPieces);

    c4.ResetHash();
    c4.HoeheErmitteln();
    c4.setDepthTie();
    c4.setMaxInstance(100);

    time_point tstart = std::chrono::high_resolution_clock::now();
    auto [x, _] = c4.WurzelMethodeComputerAnziehender(0, -9999, 9999,
                                                      c4.HoeheErmitteln());
    time_point tend = std::chrono::high_resolution_clock::now();
    float d = float(duration(tend - tstart).count());
    timesMine.push_back(d);

    GameSolver::Connect4::Position P;
    for (const auto &mv : moveSequence) {
      ASSERT_TRUE(P.canPlay(mv));
      P.playCol(mv);
    }

    solver.reset(); // TODO: necessary/helpful?
    tstart = std::chrono::high_resolution_clock::now();
    int score = solver.solve(P, weak);
    tend = std::chrono::high_resolution_clock::now();
    d = float(duration(tend - tstart).count());
    timesPons.push_back(d);

    std::stringstream ss;
    for (auto it = moveSequence.begin(); it != moveSequence.end(); it++) {
      ss << (*it + 1);
    }

    // std::cout << "Mine: " << x  << " Pons: " << score << " Database: " <<
    // entry.m_value << std::endl; std::cout << "Time mine: " <<
    // timesMine.back() << ". Time PonsC4: " << timesPons.back() << std::endl;

    EXPECT_TRUE(sgn(x) == sgn(score))
        << "Mine: " << x << " Pons: " << score << "\n"
        << c4.toString() << "\n"
        << ss.str();

    // if(i % 1 == 0) std::cout << "Done with " << i << std::endl;
  }
  time_point time_end = std::chrono::high_resolution_clock::now();

  auto sumTimesMine = std::accumulate(timesMine.begin(), timesMine.end(), 0.0f);
  auto sumTimesPons = std::accumulate(timesPons.begin(), timesPons.end(), 0.0f);
  std::cout << "Time Total mine: " << sumTimesMine
            << ". Time Total PonsC4: " << sumTimesPons << std::endl;

  float dur = float(duration(time_end - time_start).count());
  auto name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
  std::cout << "Total Time for " << name << ": " << dur << " seconds"
            << std::endl;
}

TEST_F(VerificationTest, equals) { ASSERT_TRUE(false); }

TEST_F(VerificationTest, toArray) {}

// void setlevel(int l)
//{
// level=l;
//}

/*
int getcomputermove(int color, int *play)
{
    static int level=86000000;//  =10; //level set to intermediate as default.
    static int originalboard [7][6];

// settings passed to engine
    int usethebook = 1;
    int randomize = 0;
    int searchwin = 0;
    int gametheoretic = 0;
    int evaltype = EVAL_SMART;

    enum levels
{BEGINNER,INTERMEDIATE,EXPERT,MASTER,INFLEVEL,GAMETHEORETIC,SEARCHWIN} levell;
    int leveltime[7] = {0,8,30,300,8600000,86000000,86000000};

    char out[255];
    int bestmove;

    double maxtime;
    maxtime=((double)level)/10.0;

    sprintf(out,"calling findmove");
    bestmove = findmove(originalboard,color,maxtime,out, play,searchwin,
randomize, usethebook, gametheoretic, evaltype ); std::cout << out << std::endl;

    return(bestmove);
}
 */

/*
 * Fierz C4 example...
TEST_F(VerificationTest, doenerTest) {
  // int color = 1; // 1== YELLOW, -1 ==RED???

  Agent4InARow _4inarow;

  // levell = INFLEVEL;
  // gametheoretic = 0;
  // searchwin = 0;
  // setlevel(leveltime[levell]);

  int gametheoretic = 0;
  int searchwin = 0;

  static int originalboard[7][6];

  originalboard[3][0] = 1;
  originalboard[3][1] = -1;
  originalboard[3][2] = 1;
  originalboard[3][3] = -1;
  originalboard[3][4] = 1;
  originalboard[1][0] = -1;
  originalboard[1][1] = 1;
  originalboard[1][2] = -1;

  char out[255];
  int bestmove;

  sprintf(out, "calling findmove");

  bestmove = _4inarow.findmove(
      originalboard,
      out); // originalboard,color,maxtime,out, &playnow,searchwin, randomize,
            // usethebook, gametheoretic, evaltype
  std::cout << out << std::endl;

  std::cout << "best move:" << bestmove << std::endl;

  // extern int64 hashhits, hashmisses;
  // std::cout << "hashhits: " << hashhits << ", hashmisses: " << hashmisses <<
  // std::endl;

  for (int gt_value : _4inarow.getGTvalues()) {
    std::cout << gt_value << " | ";
  }
}
 */

TEST_F(VerificationTest, genGetColumnHeights) {
  Connect4 c4;
  auto name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
  std::cout << "/*"
            << "Generated by: " << name << "*/" << std::endl;
  for (auto i = 0; i < 50; i++) {
    c4.Reset();
    c4.setFeld(0LL, 0LL);

    auto nPieces = (rand() % 10) * 2 + 8;

    bool player = false;
    std::vector<int> moveSequence;
    int j;
    for (j = 0; j < nPieces; ++j) {
      int randColumn = rand() % 7;
      while (c4.isColumnFull(randColumn))
        randColumn = rand() % 7;
      if (player && c4.Gewinnstellung1(randColumn, c4.getHeight(randColumn)))
        break;
      if (!player && c4.Gewinnstellung2(randColumn, c4.getHeight(randColumn)))
        break;
      c4.SteinSetzen(randColumn, player);
      player = !player;
      moveSequence.push_back(randColumn);
    }
    if (j != nPieces)
      continue;
    if (c4.SpielBeenden2())
      continue;

    std::stringstream ss;
    for (int c = 0; c < 7; c++) {
      ss << c4.getHeight(c) << (c < 6 ? "," : "");
    }

    auto [x1, x2] = c4.getBoard();
    std::cout << std::hex << std::uppercase << "xYellow=UINT64_C(0x" << x1
              << "); "
              << "xRed=UINT64_C(0x" << x2 << "); expHeight={" << ss.str()
              << "};"
              << "EXPECT_EQ(expHeight, B::getColumnHeights(xYellow, xRed));\n"
              << std::endl;
  }
}

TEST_F(VerificationTest, genSetBoard) {
  Connect4 c4;
  auto name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
  std::cout << "/* "
            << "Generated by: " << name << " */" << std::endl;
  for (auto i = 0; i < 500; i++) {
    c4.Reset();
    c4.setFeld(0LL, 0LL);

    auto nPieces = 41; //(rand() % 30) + 13;

    bool player = false;
    std::vector<int> moveSequence;
    int j;
    for (j = 0; j < nPieces; ++j) {
      int randColumn = rand() % 7;
      while (c4.isColumnFull(randColumn))
        randColumn = rand() % 7;
      if (player && c4.Gewinnstellung1(randColumn, c4.getHeight(randColumn)))
        break;
      if (!player && c4.Gewinnstellung2(randColumn, c4.getHeight(randColumn)))
        break;
      c4.SteinSetzen(randColumn, player);
      player = !player;
      moveSequence.push_back(randColumn);
    }
    if (j != nPieces)
      continue;
    if (c4.SpielBeenden2())
      continue;

    auto b = c4.toArray();
    std::stringstream ss;
    ss << "arr = {{";
    for (auto c = 0; c < 7; ++c) {
      ss << "{";
      for (auto r = 0; r < 6; r++) {
        ss << (b[c][r] > 0 ? 3 - b[c][r] : 0);
        if (r < 5)
          ss << ", ";
      }
      ss << "}";
      if (c < 6)
        ss << ", //\n";
    }
    ss << "}};";
    std::cout << ss.str() << std::endl;
    std::cout << "ASSERT_TRUE(b.setBoard(arr));\n"
                 "ASSERT_EQ(b.toArray(), arr);\n"
              << std::endl;
  }
}

TEST_F(VerificationTest, genhasWinYellow) {
  srand(static_cast<uint32_t>(time(nullptr)));
  Connect4 c4;
  auto name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
  std::cout << "/* "
            << "Generated by: " << name << " */" << std::endl;
  for (auto i = 0; i < 30; i++) {
    c4.Reset();
    c4.setFeld(0LL, 0LL);

    auto nPieces = (rand() % 30) + 13;

    bool player = false;
    std::vector<int> moveSequence;
    int j;
    for (j = 0; j < nPieces; ++j) {
      int randColumn = rand() % 7;
      while (c4.isColumnFull(randColumn))
        randColumn = rand() % 7;
      if (player && c4.Gewinnstellung1(randColumn, c4.getHeight(randColumn)))
        break;
      if (!player && c4.Gewinnstellung2(randColumn, c4.getHeight(randColumn))) {
        std::cout << "// column: " << randColumn << std::endl;
        break;
      }
      c4.SteinSetzen(randColumn, player);
      player = !player;
      moveSequence.push_back(randColumn);
    }
    // if (j != nPieces)
    //   continue;
    if (!c4.SpielBeenden2())
      continue;

    auto b = c4.toArray();
    std::stringstream ss;
    ss << "arr = {{";
    for (auto c = 0; c < 7; ++c) {
      ss << "{";
      for (auto r = 0; r < 6; r++) {
        ss << (b[c][r] > 0 ? 3 - b[c][r] : 0);
        if (r < 5)
          ss << ", ";
      }
      ss << "}";
      if (c < 6)
        ss << ", //\n";
    }
    ss << "}};";
    std::cout << ss.str() << std::endl;
    std::cout << "ASSERT_TRUE(b.setBoard(arr));\n"
                 "ASSERT_EQ(b.toArray(), arr);\n"
                 "ASSERT_TRUE(b.hasWinYellow());\n"
              << std::endl;
  }
}