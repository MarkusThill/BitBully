#ifndef OPENINGBOOK_H
#define OPENINGBOOK_H

#include <Board.h>

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <tuple>
#include <vector>

namespace BitBully {

// TODO: guess database type from size of file!
class OpeningBook {
 private:
  using key_t = int;
  using value_t = int8_t;
  static constexpr size_t SIZE_BYTES_8PLY_DB = 103'545;  // 102'858;
  static constexpr size_t SIZE_BYTES_12PLY_DB = 6'943'780;
  static constexpr size_t SIZE_BYTES_12PLY_DB_WITH_DIST = 21'004'495;

  static constexpr size_t SIZE_8PLY_DB = 34'515;
  static constexpr size_t SIZE_12PLY_DB = 1'735'945;
  static constexpr size_t SIZE_12PLY_DB_WITH_DIST = 4'200'899;

  std::vector<std::tuple<key_t, value_t>> m_book;
  bool m_withDistances{};
  bool m_is8ply{};
  std::filesystem::path m_bookPath;
  int m_nPly{};

  [[nodiscard]] value_t binarySearch(const key_t& huffmanCode) const {
    // one could also use: std::lower_bound()
    int l = 0;  // dont use size_t to prevent undesired underflows
    int r = m_book.size() - 1;
    while (r >= l) {
      const auto mid = (l + r + 1) / 2;
      auto p = m_book.at(mid);
      if (std::get<0>(p) == huffmanCode) {
        return std::get<1>(p);  // Found! return the value for this position
      }
      if (std::get<0>(p) > huffmanCode) {
        r = mid - 1;
      } else {  // p < huffmanCode
        l = mid + 1;
      }
    }

    // Nothing found:
    return std::numeric_limits<value_t>::min();
  }

 public:
  static constexpr auto NONE_VALUE = std::numeric_limits<value_t>::min();

  explicit OpeningBook(const std::filesystem::path& bookPath,
                       const bool is_8ply, const bool with_distances) {
    init(bookPath, is_8ply, with_distances);
  }

  explicit OpeningBook(const std::filesystem::path& bookPath) {
    if (!std::filesystem::exists(bookPath)) {
      throw std::invalid_argument("Book file does not exist: " +
                                  bookPath.string());
    }

    const auto fileSize = std::filesystem::file_size(bookPath);
    // infer DB type from size:
    const bool is8ply = (fileSize == SIZE_BYTES_8PLY_DB);
    const bool withDistances = (fileSize == SIZE_BYTES_12PLY_DB_WITH_DIST);

    init(bookPath, is8ply, withDistances);
  }

  auto getBook() const { return m_book; }

  void init(const std::filesystem::path& bookPath, const bool is_8ply,
            const bool with_distances) {
    assert(!is_8ply || !with_distances);

    // Validate the file
    if (!std::filesystem::exists(bookPath)) {
      throw std::invalid_argument("Book file does not exist: " +
                                  bookPath.string());
    }

#ifndef NDEBUG
    // Infer database type from file size (if required)
    const auto fileSize = std::filesystem::file_size(bookPath);
#endif
    if (is_8ply) {
      assert(fileSize == SIZE_BYTES_8PLY_DB);  // 8-ply with distances
    } else if (with_distances) {
      assert(fileSize ==
             SIZE_BYTES_12PLY_DB_WITH_DIST);  // 12-ply with distances
    } else {
      assert(fileSize == SIZE_BYTES_12PLY_DB);  // 12-ply without distances
    }

    this->m_withDistances = with_distances;
    this->m_is8ply = is_8ply;
    this->m_book = readBook(bookPath, with_distances, is_8ply);
    this->m_bookPath = bookPath;
    this->m_nPly = (is_8ply ? 8 : 12);

    assert(!with_distances || is_8ply ||
           m_book.size() == SIZE_12PLY_DB_WITH_DIST);  // 12-ply with distances

    assert(with_distances || is_8ply ||
           m_book.size() == SIZE_12PLY_DB);  // 12-ply without distances

    assert(!is_8ply ||
           m_book.size() == SIZE_8PLY_DB);  // 8-ply without distances
  }

  [[nodiscard]] auto getEntry(const size_t entryIdx) const {
    return m_book.at(entryIdx);
  }

  [[nodiscard]] auto getBookSize() const { return m_book.size(); }

  static std::tuple<key_t, int> readline(std::ifstream& file,
                                         const bool with_distances,
                                         const bool is_8ply) {
    const decltype(file.gcount()) bytes_position = is_8ply ? 3 : 4;
    char buffer[4] = {};  // Max buffer size for reading
    file.read(buffer, bytes_position);

    if (file.gcount() != bytes_position) {
      // EOF or read error
      return {0, 0};
    }

    // Convert the read bytes into an integer
    key_t huffman_position = 0;
    for (decltype(file.gcount()) i = 0; i < bytes_position; ++i) {
      huffman_position =
          (huffman_position << 8) | static_cast<unsigned char>(buffer[i]);
    }

    if (!is_8ply) {
      // Handle signed interpretation for 4-byte numbers
      if (huffman_position & (1LL << ((bytes_position * 8) - 1))) {
        huffman_position -= (1LL << (bytes_position * 8));
      }
    }

    value_t score = 0;
    if (with_distances) {
      // Read one additional byte for the score
      char score_byte;
      if (file.read(&score_byte, 1)) {
        score = static_cast<int8_t>(score_byte);
      } else {
        // EOF after reading huffman_position
        return {0, 0};
      }
    } else {
      // Last 2 bits indicate the score
      score = (static_cast<value_t>(huffman_position) & 3) * -1;
      huffman_position = huffman_position & ~3;
    }

    return {huffman_position, score};
  }

  int getNPly() const { return m_nPly; }

  static std::vector<std::tuple<key_t, value_t>> readBook(
      const std::filesystem::path& filename, const bool with_distances = true,
      const bool is_8ply = false) {
    std::vector<std::tuple<key_t, value_t>> book;  // To store the book entries
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
      std::cerr << "Failed to open file: " << filename.string() << '\n';
      return book;  // Return an empty book if the file can't be opened
    }

    while (true) {
      auto [position, score] = readline(file, with_distances, is_8ply);
      if (file.eof()) {
        break;  // End of file reached
      }
      book.emplace_back(position, score);
    }

    return book;
  }

  template <typename T>
  static int sign(T value) {
    return (value > 0) - (value < 0);
  }

  int inline convertValue(const int value, const Board& b) const {
    if (!m_withDistances) return value;

    // adjust value to our scoring system
    int movesLeft = std::abs(value) - 100 + b.movesLeft();
    return sign(value) * (movesLeft / 2 + 1);
  }

  [[nodiscard]] bool isInBook(const Board& b) const {
    // Only check if exactly this position is in the book
    return (binarySearch(b.toHuffman()) != NONE_VALUE);
  }

  [[nodiscard]] int getBoardValue(const Board& b) const {
    if (!((m_is8ply && b.countTokens() == 8) || b.countTokens() == 12)) {
      return NONE_VALUE;
    }

    // # first try this position
    auto p = b.toHuffman();
    int val = binarySearch(p);
    if (val != NONE_VALUE) {
      return convertValue(val, b);
    }

    // # Did not find position. Look for the mirrored equivalent
    p = b.mirror().toHuffman();
    val = binarySearch(p);
    if (!m_withDistances && val == NONE_VALUE) {
      // only for the 8-ply and 12-ply database without distances
      val = 1;  // if a position is not in the database, then this means that
                // player 1 wins

      // obsolete:
      // Apparently, positions with 2 immediate threats for player Red are
      // missing in the 8-ply database
      // if (m_is8ply && !b.generateNonLosingMoves()) {
      //  val = -1;
      //}
    } else if (val == NONE_VALUE) {
      // This is a special case. Positions, where player 1 (yellow) can
      // immediately win, are not encoded in the databases.
      return (b.movesLeft() + 1) / 2;
    }
    assert(val != NONE_VALUE);
    return convertValue(val, b);
  }
};

}  // namespace BitBully

#endif  // OPENINGBOOK_H
