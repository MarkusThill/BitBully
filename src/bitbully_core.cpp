#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <array>
#include <filesystem>
#include <vector>

#include "BitBully.h"
#include "Board.h"
#include "OpeningBook.h"

namespace py = pybind11;
using B = BitBully::Board;

PYBIND11_MODULE(bitbully_core, m) {
  m.doc() =
      "Bitbully is a fast Connect-4 solver.";  // optional module docstring

  py::class_<BitBully::BitBully>(m, "BitBullyCore")
      .def(py::init<>())  // Expose the default constructor
      .def(py::init<std::filesystem::path>(), py::arg("openingBookPath"))
      .def("mtdf", &BitBully::BitBully::mtdf, "MTD(f) algorithm",
           py::arg("board"), py::arg("first_guess"))
      .def("nullWindow", &BitBully::BitBully::nullWindow, "Null-window search",
           py::arg("board"))
      .def("negamax", &BitBully::BitBully::negamax, "negamax search",
           py::arg("board"), py::arg("alpha"), py::arg("beta"),
           py::arg("depth"))
      .def("scoreMoves", &BitBully::BitBully::scoreMoves, "evaluate all moves",
           py::arg("board"))
      .def("resetTranspositionTable",
           &BitBully::BitBully::resetTranspositionTable,
           "Reset the transposition table")
      .def("getNodeCounter", &BitBully::BitBully::getNodeCounter,
           "Get the current node counter")
      .def("resetNodeCounter", &BitBully::BitBully::resetNodeCounter,
           "Reset the node counter")
      .def("isBookLoaded", &BitBully::BitBully::isBookLoaded,
           "Check, if opening book is loaded")
      .def("isBookLoaded", &BitBully::BitBully::isBookLoaded,
           "Check, if opening book is loaded");

  // Expose the Board class
  // TODO: Check functions.... Many not necessary and some might be missing
  py::class_<B>(m, "BoardCore")
      .def(py::init<>())              // Default constructor
      .def(py::init<const B&>())      // Copy-Konstruktor
      .def("__str__", &B::toString)   // Override __str__ in Python
      .def("__repr__", &B::toString)  // Override __repr__ in Python
      .def("canWin", py::overload_cast<int>(&B::canWin, py::const_),
           "Check, if current player can win by moving into column.",
           py::arg("column"))
      .def("copy", &B::copy, "Create a deep copy of the board.")
      .def("canWin", py::overload_cast<>(&B::canWin, py::const_),
           "Check, if current player can win with the next move.")
      .def("hash", py::overload_cast<>(&B::hash, py::const_),
           "Hash the current position and return hash value.")
      .def("hasWin", &B::hasWin,
           "Check, if the player who performed the last move has a winning "
           "position (4 in a row).")
      .def("play", py::overload_cast<int>(&B::play),
           "Play a move by column index", py::arg("column"))
      .def("play", py::overload_cast<const std::vector<int>&>(&B::play),
           "Play a sequence of moves by column index", py::arg("moveSequence"))
      .def("play", py::overload_cast<const std::string&>(&B::play),
           "Play a sequence of moves by column index", py::arg("moveSequence"))
      .def("playMoveOnCopy", &B::playMoveOnCopy,
           "Play a move on a copy of the board and return the new board",
           py::arg("mv"))
      .def("generateMoves", &B::generateMoves, "Generate possible moves")
      .def("isLegalMove", &B::isLegalMove, "Check if a move is legal",
           py::arg("column"))
      .def("toString", &B::toString,
           "Return a string representation of the board")
      .def("movesLeft", &B::movesLeft, "Get the number of moves left")
      .def("countTokens", &B::countTokens,
           "Get the number of Tokens on the board")
      .def("mirror", &B::mirror,
           "Get the mirrored board (mirror around center column)")
      .def("sortMoves", &B::sortMoves, "Sort moves based on priority",
           py::arg("moves"))
      .def("allPositions", &B::allPositions,
           "Generate all positions that can be reached from the current board "
           "with n tokens.",
           py::arg("upToNPly"), py::arg("exactlyN"))
      .def("findThreats", &B::findThreats, "Find threats on the board",
           py::arg("moves"))
      .def("generateNonLosingMoves", &B::generateNonLosingMoves,
           "Generate non-losing moves")
      .def("doubleThreat", &B::doubleThreat, "Find double threats",
           py::arg("moves"))
      .def("toArray", &B::toArray,
           "Convert the board to a 2D array representation")
      .def("setBoard", py::overload_cast<const std::vector<int>&>(&B::setBoard),
           "Set the board using a list", py::arg("moveSequence"))
      .def("setBoard", py::overload_cast<const B::TBoardArray&>(&B::setBoard),
           "Set the board using a 2D array", py::arg("array"))
      .def("setBoard", py::overload_cast<const B::TBoardArrayT&>(&B::setBoard),
           "Set the board using a 2D array", py::arg("array"))
      .def("setBoard", py::overload_cast<const std::string&>(&B::setBoard),
           "Play a sequence of moves by column index", py::arg("moveSequence"))
      .def_static("isValid", &B::isValid, "Check, if a board is a valid one.",
                  py::arg("board"))
      .def_static("randomBoard", &B::randomBoard,
                  "Create a random board with n tokens.", py::arg("nPly"),
                  py::arg("forbidDirectWin"))
      .def("toHuffman", &B::toHuffman,
           "Encode position into a huffman-code compressed sequence.")
      .def("uid", &B::uid, "Get the unique identifier for the board")
      .def("__eq__", &B::operator==, "Check if two boards are equal")
      .def("__ne__", &B::operator!=, "Check if two boards are not equal");

  // Expose OpeningBook:
  py::class_<BitBully::OpeningBook>(m, "OpeningBookCore")
      // Constructors
      .def(py::init<const std::filesystem::path&, bool, bool>(),
           py::arg("bookPath"), py::arg("is_8ply"), py::arg("with_distances"),
           "Initialize an OpeningBook with explicit settings.")
      .def(py::init<const std::filesystem::path&>(), py::arg("bookPath"),
           "Initialize an OpeningBook by inferring database type from file "
           "size.")

      // Member functions
      .def("init", &BitBully::OpeningBook::init, py::arg("bookPath"),
           py::arg("is_8ply"), py::arg("with_distances"),
           "Reinitialize the OpeningBook with new settings.")
      .def("getEntry", &BitBully::OpeningBook::getEntry, py::arg("entryIdx"),
           "Get an entry from the book by index.")
      .def("getBook", &BitBully::OpeningBook::getBook,
           "Return the raw book table.")
      .def("getBookSize", &BitBully::OpeningBook::getBookSize,
           "Get the size of the book.")
      .def("getBoardValue", &BitBully::OpeningBook::getBoardValue,
           py::arg("board"), "Get the value of a given board.")
      .def("isInBook", &BitBully::OpeningBook::isInBook, py::arg("board"),
           "Check, if the given board is in the opening book. Note, that "
           "usually boards are only present in one mirrored variant.")
      .def("convertValue", &BitBully::OpeningBook::convertValue,
           py::arg("value"), py::arg("board"),
           "Convert a value to the internal scoring system.")
      .def("getNPly", &BitBully::OpeningBook::getNPly,
           "Get the ply depth of the book.")

      // Static functions
      .def_static("readBook", &BitBully::OpeningBook::readBook,
                  py::arg("filename"), py::arg("with_distances") = true,
                  py::arg("is_8ply") = false, "Read a book from a file.");
}
