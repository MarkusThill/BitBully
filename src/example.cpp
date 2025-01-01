#include <../thirdParty/pybind11/include/pybind11/pybind11.h>

#include "BitBully.h"

int add(int i, int j) { return i + j; }

namespace py = pybind11;

PYBIND11_MODULE(example, m) {
  m.doc() = "pybind11 example plugin";  // optional module docstring
  m.def("add", &add, "A function that adds two numbers");

  py::class_<BitBully::BitBully>(m, "BitBully")
      .def(py::init<>())  // Expose the default constructor
      .def("mtdf", &BitBully::BitBully::mtdf, "MTD(f) algorithm",
           py::arg("board"), py::arg("first_guess"))
      .def("nullWindow", &BitBully::BitBully::nullWindow, "Null-window search",
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
  py::class_<BitBully::Board>(m, "Board")
      .def(py::init<>())  // Default constructor
      .def("playMoveFastBB", &BitBully::Board::playMoveFastBB,
           "Play a move on the board (bitboard representation)", py::arg("mv"))
      // overload cast probably not necessary, since there is only one function
      // with that name:
      .def("playMove", py::overload_cast<int>(&BitBully::Board::playMove),
           "Play a move by column index", py::arg("column"))
      .def("playMoveOnCopy", &BitBully::Board::playMoveOnCopy,
           "Play a move on a copy of the board and return the new board",
           py::arg("mv"))
      .def("generateMoves", &BitBully::Board::generateMoves,
           "Generate possible moves")
      .def("isLegalMove", &BitBully::Board::isLegalMove,
           "Check if a move is legal", py::arg("column"))
      .def("toString", &BitBully::Board::toString,
           "Return a string representation of the board")
      .def("movesLeft", &BitBully::Board::movesLeft,
           "Get the number of moves left")
      .def("mirror", &BitBully::Board::mirror, "Get the mirrored board")
      .def("sortMoves", &BitBully::Board::sortMoves,
           "Sort moves based on priority", py::arg("moves"))
      .def("findThreats", &BitBully::Board::findThreats,
           "Find threats on the board", py::arg("moves"))
      .def("generateNonLosingMoves", &BitBully::Board::generateNonLosingMoves,
           "Generate non-losing moves")
      .def("doubleThreat", &BitBully::Board::doubleThreat,
           "Find double threats", py::arg("moves"))
      .def("toArray", &BitBully::Board::toArray,
           "Convert the board to a 2D array representation")
      .def("setBoard", &BitBully::Board::setBoard,
           "Set the board using a 2D array", py::arg("board"))
      .def("uid", &BitBully::Board::uid,
           "Get the unique identifier for the board")
      .def("__eq__", &BitBully::Board::operator==,
           "Check if two boards are equal")
      .def("__ne__", &BitBully::Board::operator!=,
           "Check if two boards are not equal");
}
