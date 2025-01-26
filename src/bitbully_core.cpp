#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <array>
#include <vector>

#include "BitBully.h"

namespace py = pybind11;
using B = BitBully::Board;

PYBIND11_MODULE(bitbully_core, m) {
  m.doc() =
      "Bitbully is a fast Connect-4 solver.";  // optional module docstring

  py::class_<BitBully::BitBully>(m, "BitBully")
      .def(py::init<>())  // Expose the default constructor
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
           "Reset the node counter");

  // Expose the Board class
  // TODO: Check functions.... Many not necessary and some might be missing
  py::class_<B>(m, "Board")
      .def(py::init<>())              // Default constructor
      .def("__str__", &B::toString)   // Override __str__ in Python
      .def("__repr__", &B::toString)  // Override __repr__ in Python
      .def("playMoveFastBB", &B::playMoveFastBB,
           "Play a move on the board (bitboard representation)", py::arg("mv"))
      .def("canWin", py::overload_cast<int>(&B::canWin, py::const_),
           "Check, if current player can win by moving into column.",
           py::arg("column"))
      .def("canWin", py::overload_cast<>(&B::canWin, py::const_),
           "Check, if current player can win with the next move.")
      .def("hash", py::overload_cast<>(&B::hash, py::const_),
           "Hash the current position and return hash value.")
      .def("hasWin", &B::hasWin,
           "Check, if the player who performed the last move has a winning "
           "position (4 in a row).")
      .def("playMove", py::overload_cast<int>(&B::playMove),
           "Play a move by column index", py::arg("column"))
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
      .def("mirror", &B::mirror, "Get the mirrored board")
      .def("sortMoves", &B::sortMoves, "Sort moves based on priority",
           py::arg("moves"))
      .def("findThreats", &B::findThreats, "Find threats on the board",
           py::arg("moves"))
      .def("generateNonLosingMoves", &B::generateNonLosingMoves,
           "Generate non-losing moves")
      .def("doubleThreat", &B::doubleThreat, "Find double threats",
           py::arg("moves"))
      .def("toArray", &B::toArray,
           "Convert the board to a 2D array representation")
      .def("setBoard", py::overload_cast<const std::vector<int>&>(&B::setBoard),
           "Set the board using a 2D array", py::arg("moveSequence"))
      .def("setBoard", py::overload_cast<const B::TBoardArray&>(&B::setBoard),
           "Set the board using a 2D array", py::arg("moveSequence"))
      .def_static("isValid", &B::isValid, "Check, if a board is a valid one.",
                  py::arg("board"))
      .def("uid", &B::uid, "Get the unique identifier for the board")
      .def("__eq__", &B::operator==, "Check if two boards are equal")
      .def("__ne__", &B::operator!=, "Check if two boards are not equal");
}
