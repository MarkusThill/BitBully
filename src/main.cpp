#include <chrono>
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

#ifdef _WIN32  // Check if we're on a Windows platform
using Clock = std::chrono::steady_clock;  // Use steady_clock on Windows
#else
using Clock = std::chrono::high_resolution_clock;  // Use high_resolution_clock
                                                   // on other platforms
#endif

void writeToCSV(const std::vector<std::tuple<float, float>>& data,
                const std::string& filename) {
  std::ofstream file(filename);  // Open file for writing
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return;
  }

  // Write header
  file << "Bitbully,Pons-C4\n";

  // Write data
  for (const auto& [val1, val2] : data) {
    file << std::fixed << std::setprecision(5)  // Control float precision
         << val1 << "," << val2 << "\n";
  }

  file.close();
  std::cout << "Data successfully written to " << filename << std::endl;
}

std::unordered_map<std::string, std::string> parseArgs(
    const int argc, const char* const argv[]) {
  std::unordered_map<std::string, std::string> args;

  for (int i = 1; i < argc; i += 2) {
    if (i + 1 < argc) {
      args[argv[i]] = argv[i + 1];
    } else {
      std::cerr << "Error: Missing value for argument " << argv[i] << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return args;
}

int main(const int argc, const char* const argv[]) {
  // Default values
  int nPly = 8;
  int nRepeats = 1000;
  std::string filename;

  // Parse command-line arguments
  auto args = parseArgs(argc, argv);
  if (args.find("--nply") != args.end()) nPly = std::stoi(args["--nply"]);
  if (args.find("--nrepeats") != args.end())
    nRepeats = std::stoi(args["--nrepeats"]);
  if (args.find("--filename") != args.end())
    filename = args["--filename"];
  else
    filename = "../times_" + std::to_string(nPly) + "_ply_" +
               std::to_string(nRepeats) + "_pos.csv";

  std::vector<std::tuple<float, float>> times = {};

  using duration = std::chrono::duration<float>;

  GameSolver::Connect4::Solver solverPonsC4;
  BitBully::BitBully bb;

  for (auto i = 0; i < nRepeats; i++) {
    auto [b, mvSequence] = BitBully::Board::randomBoard(nPly, true);

    // Bitbully:
    auto tStart = Clock::now();
    const int scoreBitbully = bb.mtdf(b, 0);
    auto tEnd = Clock::now();
    auto timeBitbully = static_cast<float>(duration(tEnd - tStart).count());

    // Pons-C4:
    GameSolver::Connect4::Position P;
    // Convert move sequence into a string representation:
    auto mvSequenceStr =
        std::accumulate(mvSequence.begin(), mvSequence.end(), std::string(""),
                        [](const std::string& a, const int mv) {
                          return a + std::to_string(mv + 1);
                        });
    if (P.play(mvSequenceStr) != b.countTokens()) {
      std::cerr << "Error: (P.play(mvSequenceStr) != b.countTokens())";
      exit(EXIT_FAILURE);
    }
    tStart = Clock::now();
    const int scorePonsC4 = solverPonsC4.solve(P, false);
    tEnd = Clock::now();
    auto timePonsC4 = static_cast<float>(duration(tEnd - tStart).count());
    times.emplace_back(timeBitbully, timePonsC4);

    if (scorePonsC4 != scoreBitbully) {
      std::cerr << "Error: " << b.toString() << "Pons-C4: " << scorePonsC4
                << " BitBully: " << scoreBitbully << std::endl;
      exit(EXIT_FAILURE);
    }

    if (i % (std::max(nRepeats, 100) / 100) == 0) {
      std::cout << "Done with " << i << " iterations" << std::endl;
    }
  }
  writeToCSV(times, filename);

  std::cout << "Node Count Pons-C4: " << solverPonsC4.getNodeCount() << ", "
            << "BitBully: " << bb.getNodeCounter() << " Percent: "
            << static_cast<double>(bb.getNodeCounter() -
                                   solverPonsC4.getNodeCount()) /
                   bb.getNodeCounter() * 100.0
            << " %" << std::endl;
  return 0;
}
