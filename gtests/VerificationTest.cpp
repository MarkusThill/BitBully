// TODO: Use a simple logger. Use glog of google...
// TODO: Log computation times using a software version into a txt file...
// TODO: Play n games against a random (or more advanced) player: It has to win
// every single game! ...

#include <filesystem>
#include <iostream>
#include <numeric>

#include "Solver.hpp"
#include "gtest/gtest.h"
#include "version.h"

namespace fs = std::filesystem;

class VerificationTest : public ::testing::Test {
 protected:
  void SetUp() override {
    GTEST_SKIP() << "Skipping all tests in this file for now";

    std::cout << "Version: v" << PROJECT_MAJOR_VERSION << "."
              << PROJECT_MINOR_VERSION << "." << PROJECT_PATCH_VERSION
              << std::endl;
  }

  void TearDown() override {}

  ~VerificationTest() override {
    // resources cleanup, no exceptions allowed
  }

  long sgn(long x) { return (x > 0) - (x < 0); }
};

TEST_F(VerificationTest, equals) { ASSERT_TRUE(false); }

TEST_F(VerificationTest, toArray) {}
