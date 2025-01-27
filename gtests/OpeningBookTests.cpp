#include <BitBully.h>

#include <Position.hpp>
#include <climits>
#include <filesystem>

#include "OpeningBook.h"
#include "gtest/gtest.h"

class OpeningBookTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}

  template <typename T>
  int sign(T value) {
    return (value > 0) - (value < 0);
  }

  ~OpeningBookTest() override = default;
};

TEST_F(OpeningBookTest, init8Ply) {
  auto bookPath = std::filesystem::path("../src/bitbully/assets/book_8ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath, true, false);

  ASSERT_EQ(ob.getBookSize(), 34'286);

  // Check a few entries
  auto entry = ob.getEntry(0);
  ASSERT_EQ(std::get<0>(entry), (351484));
  ASSERT_EQ(std::get<1>(entry), 0);

  entry = ob.getEntry(10);
  ASSERT_EQ(std::get<0>(entry), (614328));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(100);
  ASSERT_EQ(std::get<0>(entry), (1244624));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(1'000);
  ASSERT_EQ(std::get<0>(entry), (2612040));
  ASSERT_EQ(std::get<1>(entry), 0);

  entry = ob.getEntry(10'000);
  ASSERT_EQ(std::get<0>(entry), (6970936));
  ASSERT_EQ(std::get<1>(entry), 0);

  entry = ob.getEntry(ob.getBookSize() - 1);
  ASSERT_EQ(std::get<0>(entry), (16667232));
  ASSERT_EQ(std::get<1>(entry), 0);

  // Ensure that the keys are sorted
  int lastKey = std::numeric_limits<int>::min();
  for (int i = 0; i < ob.getBookSize(); ++i) {
    entry = ob.getEntry(i);
    auto k = std::get<0>(entry);
    ASSERT_LT(lastKey, k);
  }
}

TEST_F(OpeningBookTest, init12Ply) {
  auto bookPath =
      std::filesystem::path("../src/bitbully/assets/book_12ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath, false, false);

  ASSERT_EQ(ob.getBookSize(), 1'735'945);

  // Check a few entries
  auto entry = ob.getEntry(0);
  ASSERT_EQ(std::get<0>(entry), (-2124976388));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(10);
  ASSERT_EQ(std::get<0>(entry), (-2124431688));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(100);
  ASSERT_EQ(std::get<0>(entry), (-2108174596));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(1'000);
  ASSERT_EQ(std::get<0>(entry), (-2097718536));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(10'000);
  ASSERT_EQ(std::get<0>(entry), (-2027967752));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(100'000);
  ASSERT_EQ(std::get<0>(entry), (-1825638740));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(1'000'000);
  ASSERT_EQ(std::get<0>(entry), (-411277128));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(ob.getBookSize() - 1);
  ASSERT_EQ(std::get<0>(entry), (2138748968));
  ASSERT_EQ(std::get<1>(entry), 0);

  // Ensure that the keys are sorted
  int lastKey = std::numeric_limits<int>::min();
  for (int i = 0; i < ob.getBookSize(); ++i) {
    entry = ob.getEntry(i);
    auto k = std::get<0>(entry);
    ASSERT_LT(lastKey, k);
  }
}

TEST_F(OpeningBookTest, init12PlyDistance) {
  auto bookPath =
      std::filesystem::path("../src/bitbully/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  BitBully::OpeningBook ob(bookPath, false, true);

  ASSERT_EQ(ob.getBookSize(), 4'200'899);

  // Check a few entries
  auto entry = ob.getEntry(0);
  ASSERT_EQ(std::get<0>(entry), (-2124988676));
  ASSERT_EQ(std::get<1>(entry), 75);

  entry = ob.getEntry(10);
  ASSERT_EQ(std::get<0>(entry), (-2124951620));
  ASSERT_EQ(std::get<1>(entry), 75);

  entry = ob.getEntry(100);
  ASSERT_EQ(std::get<0>(entry), (-2122462468));
  ASSERT_EQ(std::get<1>(entry), -78);

  entry = ob.getEntry(1'000);
  ASSERT_EQ(std::get<0>(entry), (-2101449796));
  ASSERT_EQ(std::get<1>(entry), -72);

  entry = ob.getEntry(10'000);
  ASSERT_EQ(std::get<0>(entry), (-2055999688));
  ASSERT_EQ(std::get<1>(entry), 75);

  entry = ob.getEntry(100'000);
  ASSERT_EQ(std::get<0>(entry), (-1912785736));
  ASSERT_EQ(std::get<1>(entry), -92);

  entry = ob.getEntry(1'000'000);
  ASSERT_EQ(std::get<0>(entry), (-1344544216));
  ASSERT_EQ(std::get<1>(entry), -72);

  entry = ob.getEntry(2'000'000);
  ASSERT_EQ(std::get<0>(entry), (-571861640));
  ASSERT_EQ(std::get<1>(entry), 95);

  entry = ob.getEntry(4'000'000);
  ASSERT_EQ(std::get<0>(entry), 1976257724);
  ASSERT_EQ(std::get<1>(entry), 73);

  entry = ob.getEntry(ob.getBookSize() - 1);
  ASSERT_EQ(std::get<0>(entry), 2138808968);
  ASSERT_EQ(std::get<1>(entry), 97);

  // Ensure that the keys are sorted
  int lastKey = std::numeric_limits<int>::min();
  for (int i = 0; i < ob.getBookSize(); ++i) {
    entry = ob.getEntry(i);
    auto k = std::get<0>(entry);
    ASSERT_LT(lastKey, k);
  }
}

TEST_F(OpeningBookTest, getBoardValue_8ply) {
  auto bookPath = std::filesystem::path("../src/bitbully/assets/book_8ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  using B = BitBully::Board;
  B b;

  // Board with 8 tokens
  B::TBoardArray arr = {{{1, 0, 0, 0, 0, 0},  //
                         {0, 0, 0, 0, 0, 0},  //
                         {0, 0, 0, 0, 0, 0},  //
                         {2, 1, 2, 1, 2, 1},  //
                         {0, 0, 0, 0, 0, 0},  //
                         {2, 0, 0, 0, 0, 0},  //
                         {0, 0, 0, 0, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 8);
  ASSERT_EQ(ob.getBoardValue(b), -1);
}

TEST_F(OpeningBookTest, getBoardValue_12ply) {
  auto bookPath =
      std::filesystem::path("../src/bitbully/assets/book_12ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  BitBully::OpeningBook ob(bookPath);

  using B = BitBully::Board;
  B b;

  // Board with 12 tokens
  B::TBoardArray arr = {{{0, 0, 0, 0, 0, 0},  //
                         {2, 1, 2, 1, 0, 0},  //
                         {0, 0, 0, 0, 0, 0},  //
                         {1, 2, 1, 2, 1, 0},  //
                         {0, 0, 0, 0, 0, 0},  //
                         {2, 1, 2, 0, 0, 0},  //
                         {0, 0, 0, 0, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  ASSERT_EQ(ob.getBoardValue(b), 1);

  // Board with 12 tokens
  arr = {{{1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 1, 1, 2, 1, 0},  //
          {2, 2, 1, 2, 2, 2}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  ASSERT_EQ(ob.getBoardValue(b), 1);

  // Board with 11 tokens (non-supported position)
  arr = {{{1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 1, 1, 2, 1, 0},  //
          {2, 2, 1, 2, 2, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 31);
  ASSERT_EQ(ob.getBoardValue(b), BitBully::OpeningBook::NONE_VALUE);
}

TEST_F(OpeningBookTest, getBoardValue_12ply_dist) {
  auto bookPath =
      std::filesystem::path("../src/bitbully/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  BitBully::OpeningBook ob(bookPath);

  using B = BitBully::Board;
  B b;

  // Board with 12 tokens
  B::TBoardArray arr = {{{0, 0, 0, 0, 0, 0},  //
                         {2, 1, 2, 1, 0, 0},  //
                         {0, 0, 0, 0, 0, 0},  //
                         {1, 2, 1, 2, 1, 0},  //
                         {0, 0, 0, 0, 0, 0},  //
                         {2, 1, 2, 0, 0, 0},  //
                         {0, 0, 0, 0, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  ASSERT_EQ(ob.getBoardValue(b), 1);

  // Board with 12 tokens
  arr = {{{1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 1, 1, 2, 1, 0},  //
          {2, 2, 1, 2, 2, 2}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  ASSERT_EQ(ob.getBoardValue(b), 3);

  // Board with 11 tokens (non-supported position)
  arr = {{{1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {1, 1, 1, 2, 1, 0},  //
          {2, 2, 1, 2, 2, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.movesLeft(), 31);
  ASSERT_EQ(ob.getBoardValue(b), BitBully::OpeningBook::NONE_VALUE);
}

TEST_F(OpeningBookTest, getBoardValue_8ply_2) {
  // Very similar (almost redundant) to getBoardValue_12ply_dist2
  //  For now, keep like this
  auto bookPath = std::filesystem::path("../src/bitbully/assets/book_8ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  using B = BitBully::Board;

  BitBully::BitBully bb;

  for (auto i = 0; i < 25; ++i) {
    auto [b, mvSequence] = B::randomBoard(8);

    ASSERT_EQ(b.countTokens(), 8);
    ASSERT_FALSE(b.hasWin());

    const auto bitbullyValue = bb.mtdf(b, 0);
    const auto bookValue = ob.getBoardValue(b);

    // only check sign
    ASSERT_EQ(sign(bitbullyValue), sign(bookValue));
  }
}

TEST_F(OpeningBookTest, getBoardValue_12ply_dist_2) {
  auto bookPath =
      std::filesystem::path("../src/bitbully/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  using B = BitBully::Board;

  BitBully::BitBully bb;

  for (auto i = 0; i < 100; ++i) {
    auto [b, mvSequence] = B::randomBoard(12);

    ASSERT_EQ(b.countTokens(), 12);
    ASSERT_FALSE(b.hasWin());

    const auto bitbullyValue = bb.mtdf(b, 0);
    const auto bookValue = ob.getBoardValue(b);

    // Check signs first
    ASSERT_EQ(sign(bitbullyValue), sign(bookValue));

    // Now check value
    ASSERT_EQ(bitbullyValue, bookValue);
  }
}

TEST_F(OpeningBookTest, getBoardValue_12ply_2) {
  // Very similar (almost redundant) to getBoardValue_12ply_dist2
  //  For now, keep like this
  auto bookPath =
      std::filesystem::path("../src/bitbully/assets/book_12ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  using B = BitBully::Board;

  BitBully::BitBully bb;

  for (auto i = 0; i < 100; ++i) {
    auto [b, mvSequence] = B::randomBoard(12);

    ASSERT_EQ(b.countTokens(), 12);
    ASSERT_FALSE(b.hasWin());

    const auto bitbullyValue = bb.mtdf(b, 0);
    const auto bookValue = ob.getBoardValue(b);

    // only check sign
    ASSERT_EQ(sign(bitbullyValue), sign(bookValue));
  }
}
