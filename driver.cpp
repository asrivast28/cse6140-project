#include "ProgramOptions.hpp"
#include "TSPInstance.hpp"

#include <iostream>

int
main(
  int argc,
  char** argv
)
{
  ProgramOptions options;
  try {
    options.parse(argc, argv);
  }
  catch (po::error& pe) {
    std::cerr << pe.what() << std::endl;
    return 1;
  }

  TSPInstance tsp(options.instanceFile());
  tsp.distanceMatrix();

  std::string algorithm = options.algorithm();

  if (algorithm == "BnB") {
  }
  else if (algorithm == "Approx") {
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

  return 0;
}
