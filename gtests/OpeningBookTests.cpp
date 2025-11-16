#include <BitBully.h>

#include <Position.hpp>
#include <algorithm>
#include <filesystem>

#include "OpeningBook.h"
#include "gtest/gtest.h"

class OpeningBookTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}

  template <typename T>
  static int sign(T value) {
    return (value > 0) - (value < 0);
  }

  std::vector<std::string> readFileLines(
      const std::filesystem::path& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file) {
      std::cerr << "Error opening file: " << filename << std::endl;
      return lines;
    }

    std::string line;
    while (std::getline(file, line)) {
      lines.push_back(line);  // Store each line in the vector
    }

    return lines;
  }
  void writeLeastSignificant3Bytes(
      const std::vector<std::tuple<int, int8_t>>& vec,
      const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
      std::cerr << "Error opening file for writing!\n";
      return;
    }
    for (auto [k, v] : vec) {
      ASSERT_TRUE(v <= 0);
      if (v < 0) {
        k |= 1;
      }
      //}
      // for (int num : vec) {
      uint8_t bytes[3];  // 3-byte buffer
      bytes[2] =
          static_cast<uint8_t>(k & 0xFF);  // LSB (Least Significant Byte)
      bytes[1] = static_cast<uint8_t>((k >> 8) & 0xFF);
      bytes[0] = static_cast<uint8_t>((k >> 16) & 0xFF);  // Third byte

      outFile.write(reinterpret_cast<const char*>(bytes), 3);  // Write 3 bytes
    }

    outFile.close();
  }

  int findIndexSorted(const std::vector<std::tuple<int, signed char>>& vec,
                      int key) {
    auto it = std::lower_bound(
        vec.begin(), vec.end(), key,
        [](const auto& item, int k) { return std::get<0>(item) < k; });

    if (it != vec.end() && std::get<0>(*it) == key) {
      return std::distance(vec.begin(), it);
    }
    return -1;  // Not found
  }
  using key_t = int;       // Huffman position (3 or 4 bytes)
  using value_t = int8_t;  // Score (1 byte)

  void serializeVector(const std::vector<std::tuple<key_t, value_t>>& vec,
                       const std::string& filename, bool with_distances,
                       bool is_8ply) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
      std::cerr << "Error opening file for writing.\n";
      return;
    }

    for (const auto& [key, value] : vec) {
      char buffer[4] = {};

      // Store only the **least-significant 3 or 4 bytes** of `key`
      if (is_8ply) {  // Write only **3 bytes** for `is_8ply == true`
        buffer[0] = static_cast<char>(key & 0xFF);
        buffer[1] = static_cast<char>((key >> 8) & 0xFF);
        buffer[2] = static_cast<char>((key >> 16) & 0xFF);
        outFile.write(buffer, 3);
      } else {  // Write **4 bytes** for `is_8ply == false`
        buffer[3] = static_cast<char>(key & 0xFF);
        buffer[2] = static_cast<char>((key >> 8) & 0xFF);
        buffer[1] = static_cast<char>((key >> 16) & 0xFF);
        buffer[0] = static_cast<char>((key >> 24) & 0xFF);
        outFile.write(buffer, 4);
      }

      // Write **1 byte for `value_t`** if `with_distances == true`
      if (with_distances) {
        outFile.write(reinterpret_cast<const char*>(&value), sizeof(value_t));
      }
    }

    outFile.close();
    std::cout << "Data successfully written to " << filename << "\n";
  }

  ~OpeningBookTest() override = default;
};

TEST_F(OpeningBookTest, init8Ply) {
  auto bookPath = std::filesystem::path("../gtests/assets/book_8ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath, true, false);

  ASSERT_EQ(ob.getNPly(), 8);

  ASSERT_EQ(ob.getBookSize(), 34'515);

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
  ASSERT_EQ(std::get<0>(entry), (6958064));
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
  auto bookPath = std::filesystem::path("../gtests/assets/book_12ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath, false, false);

  ASSERT_EQ(ob.getNPly(), 12);

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
      std::filesystem::path("../gtests/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  BitBully::OpeningBook ob(bookPath, false, true);

  ASSERT_EQ(ob.getNPly(), 12);

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
  auto bookPath = std::filesystem::path("../gtests/assets/book_8ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  ASSERT_EQ(ob.getNPly(), 8);

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

  arr = {{{1, 0, 0, 0, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 8);
  ASSERT_EQ(ob.getBoardValue(b), 1);

  arr = {{{1, 0, 0, 0, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 8);
  ASSERT_EQ(ob.getBoardValue(b), 1);
}

TEST_F(OpeningBookTest, getBoardValue_12ply) {
  auto bookPath = std::filesystem::path("../gtests/assets/book_12ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  BitBully::OpeningBook ob(bookPath);

  ASSERT_EQ(ob.getNPly(), 12);

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
      std::filesystem::path("../gtests/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  BitBully::OpeningBook ob(bookPath);

  ASSERT_EQ(ob.getNPly(), 12);

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
  EXPECT_EQ(ob.getBoardValue(b), 1);

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
  EXPECT_EQ(ob.getBoardValue(b), 3);

  // https://github.com/MarkusThill/Connect-Four/issues/3
  // State 1:
  arr = {{{2, 0, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {2, 1, 0, 0, 0, 0},  //
          {1, 2, 0, 0, 0, 0},  //
          {1, 1, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {2, 2, 0, 0, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  EXPECT_EQ(ob.getBoardValue(b), 7);

  // https://github.com/MarkusThill/Connect-Four/issues/3
  // State 2:
  arr = {{{0, 0, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  EXPECT_EQ(ob.getBoardValue(b), 4);

  // https://github.com/MarkusThill/Connect-Four/issues/3
  // State 3:
  arr = {{{0, 0, 0, 0, 0, 0},  //
          {1, 2, 2, 1, 1, 2},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 1, 2, 1, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  EXPECT_EQ(ob.getBoardValue(b), 2);

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
  //  For now, keep like this...
  auto bookPath = std::filesystem::path("../gtests/assets/book_8ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  ASSERT_EQ(ob.getNPly(), 8);

  using B = BitBully::Board;

  BitBully::BitBully bb;

  ASSERT_FALSE(bb.isBookLoaded());

  for (auto i = 0; i < 25; ++i) {
    auto [b, mvSequence] = B::randomBoard(8, false);

    ASSERT_EQ(b.countTokens(), 8);
    ASSERT_FALSE(b.hasWin());

    const auto bitbullyValue = bb.mtdf(b, 0);
    const auto bookValue = ob.getBoardValue(b);

    // only check sign
    ASSERT_EQ(sign(bitbullyValue), sign(bookValue)) << b.toString();
  }
}

TEST_F(OpeningBookTest, getBoardValue_12ply_dist_2) {
  auto bookPath =
      std::filesystem::path("../gtests/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  ASSERT_EQ(ob.getNPly(), 12);

  using B = BitBully::Board;

  BitBully::BitBully bb;
  ASSERT_FALSE(bb.isBookLoaded());

  for (auto i = 0; i < 100; ++i) {
    auto [b, mvSequence] = B::randomBoard(12, false);

    ASSERT_EQ(b.countTokens(), 12);
    ASSERT_FALSE(b.hasWin());

    const auto bitbullyValue = bb.mtdf(b, 0);
    const auto bookValue = ob.getBoardValue(b);

    // Check signs first
    EXPECT_EQ(sign(bitbullyValue), sign(bookValue));

    // Now check value
    EXPECT_EQ(bitbullyValue, bookValue);
  }
}

TEST_F(OpeningBookTest, getBoardValue_12ply_2) {
  // Very similar (almost redundant) to getBoardValue_12ply_dist2
  //  For now, keep like this
  auto bookPath = std::filesystem::path("../gtests/assets/book_12ply.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  ASSERT_EQ(ob.getNPly(), 12);

  using B = BitBully::Board;

  BitBully::BitBully bb;
  ASSERT_FALSE(bb.isBookLoaded());

  for (auto i = 0; i < 100; ++i) {
    auto [b, mvSequence] = B::randomBoard(12, false);

    ASSERT_EQ(b.countTokens(), 12);
    ASSERT_FALSE(b.hasWin());

    const auto bitbullyValue = bb.mtdf(b, 0);
    const auto bookValue = ob.getBoardValue(b);

    // only check sign
    ASSERT_EQ(sign(bitbullyValue), sign(bookValue));
  }
}

TEST_F(OpeningBookTest, checkCompleteness12PlyDist) {
  GTEST_SKIP() << "Skipping this test, since it only needs to be run when the "
                  "12-ply database changes!";

  //  Ensure that ALL positions are in the 12play database!
  //  First get all positions with 12 tokens:
  BitBully::Board b;
  const auto positions = b.allPositions(12, true);

  auto bookPath =
      std::filesystem::path("../gtests/assets/book_12ply_distances.dat");
  if (!exists(bookPath)) {
    bookPath = ".." / bookPath;
  }
  ASSERT_TRUE(exists(bookPath));
  const BitBully::OpeningBook ob(bookPath);

  ASSERT_EQ(ob.getNPly(), 12);

  for (auto p : positions) {
    const auto pInBook = ob.isInBook(p) || ob.isInBook(p.mirror());
    const auto pWin = p.hasWin() || p.canWin();

    // positions that have a win or where yellow (player 1) can win with the
    // next move are not encoded in the database
    ASSERT_TRUE(pInBook ^ pWin)
        << p.toString() << "pInBook: " << pInBook << ", "
        << "pWin: " << pWin;
  }
}

TEST_F(OpeningBookTest, incorrectPositions12PlyDist) {
  // The values of three positions in the original 12-ply database with ditances
  // were wrong. See:
  // https://github.com/MarkusThill/Connect-Four/issues/3
  // We correct their values here:
  // This should NOT be in a test file... TODO:outsource this code
  GTEST_SKIP() << "Skipping this, since it is not a real test";
  auto bookPath_ =
      std::filesystem::path("../gtests/assets/book_12ply_distances.dat");
  ASSERT_TRUE(exists(bookPath_));
  BitBully::OpeningBook ob(bookPath_);

  auto book = ob.getBook();
  auto oldBookSize = book.size();
  std::cout << "Book size before: " << oldBookSize << std::endl;

  // https://github.com/MarkusThill/Connect-Four/issues/3
  // State 1:
  BitBully::Board b;
  BitBully::Board::TBoardArray arr = {{{2, 0, 0, 0, 0, 0},  //
                                       {1, 2, 0, 0, 0, 0},  //
                                       {2, 1, 0, 0, 0, 0},  //
                                       {1, 2, 0, 0, 0, 0},  //
                                       {1, 1, 0, 0, 0, 0},  //
                                       {1, 0, 0, 0, 0, 0},  //
                                       {2, 2, 0, 0, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);

  std::cout << "\nStarting with State 1:";
  auto index = findIndexSorted(book, b.toHuffman());
  auto [key, value] = book.at(index);
  std::cout << index << std::endl << key << ", " << int(value) << std::endl;
  std::cout << "Converted value: " << ob.convertValue(value + 8, b)
            << ", Expected: " << 7 << std::endl;
  // Correct:
  book.at(index) = {key, value + 8};
  std::tie(key, value) = book.at(index);
  std::cout << index << std::endl << key << ", " << int(value) << std::endl;
  std::cout << "Converted value: " << ob.convertValue(value, b)
            << ", Expected: " << 7 << std::endl;

  // https://github.com/MarkusThill/Connect-Four/issues/3
  // State 2:
  arr = {{{0, 0, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {1, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {2, 2, 1, 1, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  // EXPECT_EQ(ob.getBoardValue(b), 4);

  std::cout << "\nStarting with State 2:";
  index = findIndexSorted(book, b.toHuffman());
  std::tie(key, value) = book.at(index);
  std::cout << index << std::endl << key << ", " << int(value - 4) << std::endl;
  std::cout << "Converted value: " << ob.convertValue(value, b)
            << ", Expected: " << 4 << std::endl;
  // Correct:
  book.at(index) = {key, value - 4};
  std::tie(key, value) = book.at(index);
  std::cout << index << std::endl << key << ", " << int(value - 4) << std::endl;
  std::cout << "Converted value: " << ob.convertValue(value, b)
            << ", Expected: " << 4 << std::endl;

  // https://github.com/MarkusThill/Connect-Four/issues/3
  // State 3:
  arr = {{{0, 0, 0, 0, 0, 0},  //
          {1, 2, 2, 1, 1, 2},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {0, 0, 0, 0, 0, 0},  //
          {2, 2, 0, 0, 0, 0},  //
          {1, 1, 2, 1, 0, 0}}};

  ASSERT_TRUE(b.setBoard(arr));
  ASSERT_EQ(b.toArray(), arr);
  ASSERT_EQ(b.countTokens(), 12);
  // EXPECT_EQ(ob.getBoardValue(b), 2);

  std::cout << "\nStarting with State 3:";
  index = findIndexSorted(book, b.toHuffman());
  std::tie(key, value) = book.at(index);
  std::cout << index << std::endl << key << ", " << int(value + 2) << std::endl;
  std::cout << "Converted value: " << ob.convertValue(value, b)
            << ", Expected: " << 2 << std::endl;
  // Correct:
  book.at(index) = {key, value + 2};
  std::tie(key, value) = book.at(index);
  std::cout << index << std::endl << key << ", " << int(value + 2) << std::endl;
  std::cout << "Converted value: " << ob.convertValue(value, b)
            << ", Expected: " << 2 << std::endl;

  serializeVector(book, "../../book_12ply_distances_new.dat", true, false);
}

TEST_F(OpeningBookTest, missingPositions8PlyDB) {
  // It seems like that there are missing positions in the 8 ply database which
  // we constructed from the DB here: http://www.lbremer.de/viergewinnt.html
  // Lets add them to the database and write a new .dat database file.
  // This should NOT be in a test file... TODO:outsource this code
  GTEST_SKIP() << "Skipping this, since it is not a real test";
  auto bookPath = std::filesystem::path("../gtests/assets/8.txt");
  std::vector<std::string> lines = readFileLines(bookPath);
  using B = BitBully::Board;

  std::map<uint64_t, B> positions;

  // Print all lines
  for (const auto& line : lines) {
    B::TBoardArray arr;
    int i = 0;
    for (const auto chr : line) {
      const auto r = i % 6;
      const auto c = i / 6;
      switch (chr) {
        case 'b':
          arr[c][r] = 0;
          break;
        case 'x':
          arr[c][r] = 1;
          break;
        case 'o':
          arr[c][r] = 2;
          break;
        default:
          break;
      }
      ++i;
    }
    B b;
    ASSERT_TRUE(b.setBoard(arr));
    positions.insert({b.uid(), b});
  }

  const B b;
  auto allPositions = b.allPositions(8, true);

  std::vector<B> missing;
  for (auto p : allPositions) {
    if (positions.find(p.uid()) == positions.end() &&
        positions.find(p.mirror().uid()) == positions.end() && !p.canWin() &&
        !p.hasWin()) {
      if (std::find(missing.begin(), missing.end(), p) == missing.end() &&
          std::find(missing.begin(), missing.end(), p.mirror()) ==
              missing.end()) {
        missing.emplace_back(p);
      }
    }
  }
  std::cout << "Missing in .txt database: " << missing.size() << std::endl;

  auto bookPath_ = std::filesystem::path("../gtests/assets/book_8ply.dat");
  ASSERT_TRUE(exists(bookPath_));
  BitBully::OpeningBook ob(bookPath_);

  BitBully::BitBully bb(
      std::filesystem::path("../gtests/assets/book_12ply_distances.dat"));
  ASSERT_TRUE(bb.isBookLoaded());

  auto book = ob.getBook();
  auto oldBookSize = book.size();
  std::cout << "Book size before: " << oldBookSize << std::endl;
  for (auto b : missing) {
    ASSERT_EQ(b.countTokens(), 8);
    ASSERT_FALSE(b.hasWin());

    const auto bitbullyValue = bb.mtdf(b, 0);
    if (bitbullyValue > 0) {
      ASSERT_TRUE(ob.getBoardValue(b) > 0)
          << b.toString() << std::endl
          << "Book:" << ob.getBoardValue(b) << std::endl
          << "BitBully:" << bitbullyValue << std::endl;
      continue;
    }

    // Continue, if this position is already in the database
    if (ob.getBoardValue(b) <= 0) {
      // If the value is <= 0 it is already in the DB
      continue;
    }

    ASSERT_TRUE(bitbullyValue <= 0);
    book.push_back({b.toHuffman(), bitbullyValue == 0 ? 0 : -1});
  }
  std::cout << "Book size after: " << book.size() << std::endl;

  // Sort by key_t (first element in tuple)
  std::sort(book.begin(), book.end(), [](const auto& a, const auto& b) {
    return std::get<0>(a) < std::get<0>(b);
  });

  if (oldBookSize != book.size()) {
    std::cout << "Book size changed. Writing new book now"
              << "std::endl";
    writeLeastSignificant3Bytes(book, "../../book_8ply_new.dat");
  }
}
