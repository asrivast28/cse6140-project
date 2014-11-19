#include "ProgramOptions.hpp"
#include "Timer.hpp"
#include "TSPInstance.hpp"
#include "BranchAndBound.hpp"
#include "MSTApproximation.hpp"


#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>

int
main(
  int argc,
  char** argv
)
{
  Timer timer;
  ProgramOptions options;
  try {
    options.parse(argc, argv);
  }
  catch (po::error& pe) {
    std::cerr << pe.what() << std::endl;
    return 1;
  }

  TSPInstance tsp(options.instanceFile());

  std::string algorithm = options.algorithm();

  std::vector<unsigned> tour;
  unsigned tourCost = 0;
  bool deterministic = true;

  if (algorithm == "BnB") {
	  BranchAndBound bnbAlgorithm(tsp.dimension(), tsp.distanceMatrix());
	  tourCost = bnbAlgorithm.solve(tour);
  }
  else if (algorithm == "Approx") {
    timer.start();
    MSTApproximation approxAlgorithm(tsp.distanceMatrix(), tsp.dimension());
    tourCost = approxAlgorithm.getTour(tour);
    timer.stop();
  }
  else if (algorithm == "Heur") {
  }
  else if (algorithm == "LS1") {
    deterministic = false;
  }
  else if (algorithm == "LS2") {
    deterministic = false;
  }
  else {
    std::cerr << "Unknown algorithm type '" << algorithm << "'." << std::endl;
    return 1;
  }

  assert(tour.size() == tsp.dimension());

  std::stringstream solnFileName;
  solnFileName << boost::filesystem::path(options.instanceFile()).stem().string();
  solnFileName << "_" << algorithm << "_" << options.cutoffTime();
  if (!deterministic) {
    solnFileName << "_" << options.randomSeed();
  }
  solnFileName << ".sol";

  std::ofstream solnFile(solnFileName.str());

  solnFile << tourCost << std::endl;
  for (unsigned i = 0; i < (tsp.dimension() - 1); ++i) {
    solnFile << tour[i] << ",";
  }
  solnFile << tour[tsp.dimension() - 1];

  std::cout << "Optimal tour cost is: " << tsp.optimalCost() << std::endl;
  std::cout << "Estimated tour cost is: " << tourCost << std::endl;
  std::cout << "Time taken for estimation: " << timer.elapsed() << " seconds" << std::endl;

  return 0;
}
