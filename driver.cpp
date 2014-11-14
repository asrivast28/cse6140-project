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
    std::cerr << pe.what();
    return 1;
  }

  TSPInstance tsp(options.instanceFile());

  return 0;
}
