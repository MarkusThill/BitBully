#ifndef OPENINGBOOK_H
#define OPENINGBOOK_H

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

namespace BitBully {

// TODO: guess database type from size of file!
class OpeningBook {
 private:
  std::vector<std::tuple<int64_t, int>> book;
  bool with_distances;
  bool is_8ply;
  std::filesystem::path bookPath;

 public:
  // Adjusted example usage
  explicit OpeningBook(const std::filesystem::path& bookPath,
                       const bool is_8ply, const bool with_distances) {
    assert(!is_8ply || !with_distances);

    // Validate the file
    if (!std::filesystem::exists(bookPath)) {
      throw std::invalid_argument("Book file does not exist: " +
                                  bookPath.string());
    }

    // Infer database type from file size (if required)
    const auto fileSize = std::filesystem::file_size(bookPath);
    if (is_8ply) {                  // TODO: use constants
      assert(fileSize == 102'858);  // 8-ply with distances
    } else if (with_distances) {
      assert(fileSize == 21'004'495);  // 12-ply with distances
    } else {
      assert(fileSize == 6'943'780);  // 12-ply without distances
    }

    this->with_distances = with_distances;
    this->is_8ply = is_8ply;
    book = read_book(bookPath, with_distances, is_8ply);
    this->bookPath = bookPath;

    assert(!with_distances || is_8ply ||
           book.size() == 4'200'899);  // 12-ply with distances

    assert(with_distances || is_8ply ||
           book.size() == 1'735'945);  // 12-ply without distances

    assert(!is_8ply || book.size() == 34'286);  // 8-ply without distances
  }

  [[nodiscard]] auto getEntry(const size_t entryIdx) const {
    return book.at(entryIdx);
  }

  [[nodiscard]] auto getBookSize() const { return book.size(); }

  static std::tuple<int64_t, int> readline(std::ifstream& file,
                                           bool with_distances,
                                           const bool is_8ply) {
    const size_t bytes_position = is_8ply ? 3 : 4;
    char buffer[4] = {};  // Max buffer size for reading
    file.read(buffer, bytes_position);

    if (file.gcount() != bytes_position) {
      // EOF or read error
      return {0, 0};
    }

    // Convert the read bytes into an integer
    int64_t huffman_position = 0;
    for (size_t i = 0; i < bytes_position; ++i) {
      huffman_position =
          (huffman_position << 8) | static_cast<unsigned char>(buffer[i]);
    }

    if (!is_8ply) {
      // Handle signed interpretation for 4-byte numbers
      if (huffman_position & (1LL << ((bytes_position * 8) - 1))) {
        huffman_position -= (1LL << (bytes_position * 8));
      }
    }

    int score = 0;
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
      score = (huffman_position & INT64_C(3)) * -1;
      huffman_position = huffman_position & ~INT64_C(3);
    }

    return {huffman_position, score};
  }

  static std::vector<std::tuple<int64_t, int>> read_book(
      const std::filesystem::path& filename, const bool with_distances = true,
      const bool is_8ply = false) {
    std::vector<std::tuple<int64_t, int>> book;  // To store the book entries
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
};

}  // namespace BitBully

#endif  // OPENINGBOOK_H
