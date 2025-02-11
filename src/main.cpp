#include <fstream>
#include <iomanip>  // For setting precision
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <thirdParty/connect4/Solver.hpp>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "BitBully.h"
#include "Board.h"

void writeToCSV(const std::vector<std::tuple<float, float>>& data,
                const std::string& filename) {
  std::ofstream file(filename);  // Open file for writing
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return;
  }

  // Write header (optional)
  file << "Bitbully,Pons-C4\n";

  // Write data
  for (const auto& [val1, val2] : data) {
    file << std::fixed << std::setprecision(5)  // Control float precision
         << val1 << "," << val2 << "\n";
  }

  file.close();
  std::cout << "Data successfully written to " << filename << std::endl;
}

int main() {
  constexpr int nPly = 8;
  constexpr int nRepeats = 1000;
  const std::string filename = "../times_" + std::to_string(nPly) + "_ply.csv";

  std::vector<std::tuple<float, float>> times = {};

  using duration = std::chrono::duration<float>;

  GameSolver::Connect4::Solver solverPons;
  BitBully::BitBully bb;

  for (auto i = 0; i < nRepeats; i++) {
    auto [b, mvSequence] = BitBully::Board::randomBoard(nPly, true);

    // Bitbully:
    auto tStart = std::chrono::high_resolution_clock::now();
    const int scoreBitbully = bb.mtdf(b, 0);
    auto tEnd = std::chrono::high_resolution_clock::now();
    auto timeBitbully = static_cast<float>(duration(tEnd - tStart).count());

    // Pons-C4:
    GameSolver::Connect4::Position P;
    // Convert move sequence into a string representation:
    auto mvSequenceStr =
        std::accumulate(mvSequence.begin(), mvSequence.end(), std::string(""),
                        [](const std::string& a, const int b) {
                          return a + std::to_string(b + 1);
                        });
    if (P.play(mvSequenceStr) != b.countTokens()) {
      std::cerr << "Error: (P.play(mvSequenceStr) != b.countTokens())";
      exit(EXIT_FAILURE);
    }
    tStart = std::chrono::high_resolution_clock::now();
    const int scorePons = solverPons.solve(P, false);
    tEnd = std::chrono::high_resolution_clock::now();
    auto timePons = static_cast<float>(duration(tEnd - tStart).count());
    times.emplace_back(timeBitbully, timePons);

    if (scorePons != scoreBitbully) {
      std::cerr << "Error: " << b.toString() << "Pons: " << scorePons
                << " Mine: " << scoreBitbully << std::endl;
      exit(EXIT_FAILURE);
    }

    if (i % (nPly / 100 + 1) == 0) {
      std::cout << "Done with " << i << " iterations" << std::endl;
    }
  }
  writeToCSV(times, filename);

  std::cout << "Node Count Pons: " << solverPons.getNodeCount() << ", "
            << "Mine: " << bb.getNodeCounter() << " Percent: "
            << static_cast<double>(bb.getNodeCounter() -
                                   solverPons.getNodeCount()) /
                   bb.getNodeCounter() * 100.0
            << " %" << std::endl;
  return 0;
}
