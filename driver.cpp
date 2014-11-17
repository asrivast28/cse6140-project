#include "AlgorithmImplementations.hpp"
#include "ProgramOptions.hpp"
#include "Timer.hpp"
#include "TSPInstance.hpp"

#include <iostream>

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
  unsigned tourCost = 0;

  if (algorithm == "BnB") {
  }
  else if (algorithm == "Approx") {
    tourCost = tsp::approx::tourCost(tsp.dimension(), tsp.distanceMatrix());
  }
  else if (algorithm == "Heur") {
  }
  else if (algorithm == "LS1") {
  }
  else if (algorithm == "LS2") {
  }
  else {
    std::cerr << "Unknown algorithm type '" << algorithm << "'." << std::endl;
    return 1;
  }

  std::cout << "Optimal tour cost is: " << tsp.optimalCost() << std::endl;
  std::cout << "Estimated tour cost is: " << tourCost << std::endl;

  return 0;
}
