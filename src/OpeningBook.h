/**
 * @file OpeningBook.h
 * @brief Read-only access to BitBully's pre-computed Connect-4 opening books.
 *
 * Two database flavours are supported:
 *  - **8-ply** (small, 8-stones positions): used for shallow lookups.
 *  - **12-ply** (large, 12-stones positions): the main book; available
 *    either with packed scores in the lower bits of the key or with a
 *    separate distance-to-mate byte.
 *
 * The on-disk layout follows the format produced by the offline solver and
 * the file size unambiguously identifies the variant; clients usually rely
 * on the inferring single-argument @c OpeningBook constructor.
 */
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

/**
 * @class OpeningBook
 * @brief Sorted in-memory table mapping Huffman-encoded positions to scores.
 *
 * Lookups use binary search over @ref Board::toHuffman() keys. Lookups
 * automatically check the mirrored variant if the original key is not in
 * the book, since only one of the two symmetric positions is stored on disk.
 */
// TODO: guess database type from size of file!
class OpeningBook {
 private:
  /// Type of the Huffman-encoded position key (sorted lookup index).
  using key_t = int;
  /// Type of the score / distance-to-mate value stored alongside each key.
  using value_t = int8_t;
  /// On-disk size (bytes) of the 8-ply book.
  static constexpr size_t SIZE_BYTES_8PLY_DB = 103'545;  // 102'858;
  /// On-disk size of the 12-ply book without distances.
  static constexpr size_t SIZE_BYTES_12PLY_DB = 6'943'780;
  /// On-disk size of the 12-ply book with distances.
  static constexpr size_t SIZE_BYTES_12PLY_DB_WITH_DIST = 21'004'495;

  /// Number of entries in the 8-ply book.
  static constexpr size_t SIZE_8PLY_DB = 34'515;
  /// Number of entries in the 12-ply book without distances.
  static constexpr size_t SIZE_12PLY_DB = 1'735'945;
  /// Number of entries in the 12-ply book with distances.
  static constexpr size_t SIZE_12PLY_DB_WITH_DIST = 4'200'899;

  std::vector<std::tuple<key_t, value_t>>
      m_book;              ///< Sorted (key, value) pairs.
  bool m_withDistances{};  ///< Whether the loaded book stores distances.
  bool m_is8ply{};         ///< Whether the loaded book is the 8-ply database.
  std::filesystem::path m_bookPath;  ///< Path the book was loaded from.
  int m_nPly{};  ///< Stones-on-board the book covers (8 or 12).

  /**
   * @brief Locate @p huffmanCode inside the sorted @c m_book array.
   * @param huffmanCode Huffman-encoded position key.
   * @return Score / distance-to-mate associated with the key, or
   *         @ref NONE_VALUE if the key is missing.
   */
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
  /// Sentinel returned when a position is not present in the book.
  static constexpr auto NONE_VALUE = std::numeric_limits<value_t>::min();

  /**
   * @brief Load an opening book with explicit flavour selection.
   * @param bookPath       Path to the binary book file.
   * @param is_8ply        @c true to load the 8-ply database, @c false for
   *                       the 12-ply database.
   * @param with_distances @c true if the file stores distance-to-mate
   *                       information in a dedicated byte (only valid for
   *                       the 12-ply database).
   * @throws std::invalid_argument if @p bookPath does not exist.
   */
  explicit OpeningBook(const std::filesystem::path& bookPath,
                       const bool is_8ply, const bool with_distances) {
    init(bookPath, is_8ply, with_distances);
  }

  /**
   * @brief Load an opening book and auto-detect its flavour.
   *
   * The flavour is inferred from the on-disk file size.
   *
   * @param bookPath Path to the binary book file.
   * @throws std::invalid_argument if @p bookPath does not exist.
   */
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

  /// @brief Copy of the underlying sorted (key, value) array.
  auto getBook() const { return m_book; }

  /**
   * @brief Re-initialise the book in place.
   *
   * Replaces the currently loaded data with the contents of @p bookPath.
   * Uses the same flavour parameters as the explicit three-argument
   * @c OpeningBook constructor.
   *
   * @param bookPath       Path to the binary book file.
   * @param is_8ply        Select the 8-ply database.
   * @param with_distances Indicate that the book stores distances.
   * @throws std::invalid_argument if @p bookPath does not exist.
   */
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

  /**
   * @brief Random-access getter for raw book entries.
   * @param entryIdx Zero-based entry index.
   * @return Tuple of (Huffman key, value).
   * @throws std::out_of_range if @p entryIdx exceeds the book size.
   */
  [[nodiscard]] auto getEntry(const size_t entryIdx) const {
    return m_book.at(entryIdx);
  }

  /// @brief Number of entries currently held in memory.
  [[nodiscard]] auto getBookSize() const { return m_book.size(); }

  /**
   * @brief Read a single entry from a binary book stream.
   *
   * Decodes one (Huffman key, value) pair according to the chosen flavour.
   *
   * @param file           Input stream positioned at the start of an entry.
   * @param with_distances @c true if the book stores a separate distance
   *                       byte; @c false if the value is packed into the
   *                       lowest two bits of the key.
   * @param is_8ply        @c true to read 3-byte 8-ply keys, @c false for
   *                       4-byte 12-ply keys.
   * @return The decoded (key, value) pair, or `{0, 0}` if the read fails.
   */
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

  /// @brief Number of stones-on-board the book covers (8 or 12).
  int getNPly() const { return m_nPly; }

  /**
   * @brief Slurp an entire opening book file into memory.
   *
   * @param filename       Path to the binary book file.
   * @param with_distances Whether the file stores distance-to-mate bytes.
   * @param is_8ply        Whether the file is the 8-ply database.
   * @return Sorted vector of (key, value) pairs. Empty if the file cannot
   *         be opened.
   */
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

  /**
   * @brief Numerical sign function returning -1, 0 or +1.
   * @tparam T  Any signed arithmetic type.
   * @param value Input value.
   * @return Sign of @p value.
   */
  template <typename T>
  static int sign(T value) {
    return (value > 0) - (value < 0);
  }

  /**
   * @brief Translate a raw book value into the engine's score convention.
   *
   * If the loaded book does not include distance information the value is
   * already in the engine's scoring system and is returned unchanged.
   * Otherwise the distance-to-mate stored in @p value is converted to the
   * compact `+/- ((moves_left)/2 + 1)` representation used elsewhere.
   *
   * @param value Raw book value.
   * @param b     Position the value was retrieved for.
   * @return Score in the engine's convention.
   */
  int inline convertValue(const int value, const Board& b) const {
    if (!m_withDistances) return value;

    // adjust value to our scoring system
    int movesLeft = std::abs(value) - 100 + b.movesLeft();
    return sign(value) * (movesLeft / 2 + 1);
  }

  /**
   * @brief Test whether the exact (non-mirrored) position is in the book.
   *
   * @note Most positions are stored only in one of the two mirror-symmetric
   *       variants. To accept either variant use @ref getBoardValue() and
   *       compare against @ref NONE_VALUE.
   *
   * @param b Position to check.
   * @return @c true if the position is stored verbatim.
   */
  [[nodiscard]] bool isInBook(const Board& b) const {
    return (binarySearch(b.toHuffman()) != NONE_VALUE);
  }

  /**
   * @brief Retrieve the engine score for a position covered by the book.
   *
   * Looks up the position by its Huffman key, falls back to the mirrored
   * variant, and applies a couple of well-known special cases for missing
   * 8-ply / 12-ply entries (e.g.\ player Yellow with an immediate win is
   * intentionally absent from the on-disk databases).
   *
   * @param b Position to look up. Must contain exactly @ref getNPly()
   *          stones; otherwise the function returns @ref NONE_VALUE.
   * @return Engine score, or @ref NONE_VALUE if @p b is not covered by the
   *         book.
   */
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
