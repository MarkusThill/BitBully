#include <filesystem>

#include "OpeningBook.h"
#include "gtest/gtest.h"

class OpeningBookTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}

  ~OpeningBookTest() override = default;
};

TEST_F(OpeningBookTest, init8Ply) {
  auto bookPath = std::filesystem::path("../src/bitbully/assets/book_8ply.dat");
  const BitBully::OpeningBook ob(bookPath, true, false);

  ASSERT_EQ(ob.getBookSize(), 34'286);

  // Check a few entries
  auto entry = ob.getEntry(0);
  ASSERT_EQ(std::get<0>(entry), INT64_C(351484));
  ASSERT_EQ(std::get<1>(entry), 0);

  entry = ob.getEntry(10);
  ASSERT_EQ(std::get<0>(entry), INT64_C(614328));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(100);
  ASSERT_EQ(std::get<0>(entry), INT64_C(1244624));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(1'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(2612040));
  ASSERT_EQ(std::get<1>(entry), 0);

  entry = ob.getEntry(10'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(6970936));
  ASSERT_EQ(std::get<1>(entry), 0);

  entry = ob.getEntry(ob.getBookSize() - 1);
  ASSERT_EQ(std::get<0>(entry), INT64_C(16667232));
  ASSERT_EQ(std::get<1>(entry), 0);
}

TEST_F(OpeningBookTest, init12Ply) {
  auto bookPath =
      std::filesystem::path("../src/bitbully/assets/book_12ply.dat");
  const BitBully::OpeningBook ob(bookPath, false, false);

  ASSERT_EQ(ob.getBookSize(), 1'735'945);

  // Check a few entries
  auto entry = ob.getEntry(0);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2124976388));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(10);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2124431688));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(100);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2108174596));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(1'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2097718536));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(10'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2027967752));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(100'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-1825638740));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(1'000'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-411277128));
  ASSERT_EQ(std::get<1>(entry), -1);

  entry = ob.getEntry(ob.getBookSize() - 1);
  ASSERT_EQ(std::get<0>(entry), INT64_C(2138748968));
  ASSERT_EQ(std::get<1>(entry), 0);
}

TEST_F(OpeningBookTest, init12PlyDistance) {
  auto bookPath =
      std::filesystem::path("../src/bitbully/assets/book_12ply_distances.dat");
  BitBully::OpeningBook ob(bookPath, false, true);

  ASSERT_EQ(ob.getBookSize(), 4'200'899);

  // Check a few entries
  auto entry = ob.getEntry(0);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2124988676));
  ASSERT_EQ(std::get<1>(entry), 75);

  entry = ob.getEntry(10);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2124951620));
  ASSERT_EQ(std::get<1>(entry), 75);

  entry = ob.getEntry(100);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2122462468));
  ASSERT_EQ(std::get<1>(entry), -78);

  entry = ob.getEntry(1'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2101449796));
  ASSERT_EQ(std::get<1>(entry), -72);

  entry = ob.getEntry(10'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-2055999688));
  ASSERT_EQ(std::get<1>(entry), 75);

  entry = ob.getEntry(100'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-1912785736));
  ASSERT_EQ(std::get<1>(entry), -92);

  entry = ob.getEntry(1'000'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-1344544216));
  ASSERT_EQ(std::get<1>(entry), -72);

  entry = ob.getEntry(2'000'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(-571861640));
  ASSERT_EQ(std::get<1>(entry), 95);

  entry = ob.getEntry(4'000'000);
  ASSERT_EQ(std::get<0>(entry), INT64_C(1976257724));
  ASSERT_EQ(std::get<1>(entry), 73);

  entry = ob.getEntry(ob.getBookSize() - 1);
  ASSERT_EQ(std::get<0>(entry), INT64_C(2138808968));
  ASSERT_EQ(std::get<1>(entry), 97);
}
