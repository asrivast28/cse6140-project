#include "ProgramOptions.hpp"
#include "Timer.hpp"
#include "TSPInstance.hpp"
#include "BranchAndBound.hpp"
#include "MSTApproximation.hpp"
#include "GreedyHeuristic.hpp"
#include "GeneticAlgorithm.hpp"
#include "IteratedLocalSearch.hpp"

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
  if (algorithm == "LS1" || algorithm == "LS2") {
	  deterministic = false;
  }

  // Make a solution file
  std::stringstream solnFileName;
  solnFileName << boost::filesystem::path(options.instanceFile()).stem().string();
  solnFileName << "_" << algorithm << "_" << options.cutoffTime();
  if (!deterministic) {
    solnFileName << "_" << options.randomSeed();
  }
  solnFileName << ".sol";

  std::ofstream solnFile(solnFileName.str());

  // Make a trace file for BnB and LS
  std::stringstream trcFileName;
  if (algorithm != "Approx" && algorithm != "Heur") {
	  trcFileName << boost::filesystem::path(options.instanceFile()).stem().string();
	  trcFileName << "_" << algorithm << "_" << options.cutoffTime();
	  if (!deterministic) {
		  trcFileName << "_" << options.randomSeed();
	  }
	  trcFileName << ".trace";
  }

  if (algorithm == "BnB") {
	  std::ofstream trcFile(trcFileName.str());

	  // Find initial bound using Heuristics
	  GreedyHeuristic greedyAlgorithm(tsp.dimension(), tsp.distanceMatrix());
	  std::vector<unsigned> initialTour;
	  unsigned initialCost = greedyAlgorithm.getTour(initialTour);

	  timer.start();
	  BranchAndBound bnbAlgorithm(tsp.dimension(),
			  tsp.distanceMatrix(),
			  options.cutoffTime(),
			  &trcFile,
			  &timer,
			  initialCost,
			  initialTour);
	  tourCost = bnbAlgorithm.solve(tour);
	  timer.stop();
  }
  else if (algorithm == "Approx") {
    timer.start();
    MSTApproximation approxAlgorithm(tsp.distanceMatrix(), tsp.dimension());
    tourCost = approxAlgorithm.getTour(tour);
    timer.stop();
  }
  else if (algorithm == "Heur") {
    timer.start();
    GreedyHeuristic greedyAlgorithm(tsp.dimension(), tsp.distanceMatrix());
    tourCost = greedyAlgorithm.getTour(tour);
    timer.stop();
  }
  else if (algorithm == "LS1") {
	  std::ofstream trcFile(trcFileName.str());

	  timer.start();

	  IteratedLocalSearch LS1(
	  			  tsp.dimension(),
	  			  tsp.distanceMatrix(),
	  			  options.cutoffTime(),
	  			  &timer,
	  			  &trcFile,
	  			  options.randomSeed());
	  tourCost = LS1.getTour(tour);

	  timer.stop();
  }
  else if (algorithm == "LS2") {
	  std::ofstream trcFile(trcFileName.str());

	  std::vector<unsigned> initialCosts;
	  std::vector<std::vector<unsigned>> initialTours;

	  // Find initial good guess for the population
	  GreedyHeuristic greedyAlgorithm(tsp.dimension(), tsp.distanceMatrix());
	  std::vector<unsigned> initialTourHeur;
	  unsigned initialCostHeur = greedyAlgorithm.getTour(initialTourHeur);

	  initialCosts.push_back(initialCostHeur);
	  initialTours.push_back(initialTourHeur);

	  MSTApproximation approxAlgorithm(tsp.distanceMatrix(), tsp.dimension());
	  std::vector<unsigned> initialTourAppr;
	  unsigned initialCostAppr = approxAlgorithm.getTour(initialTourAppr);

	  initialCosts.push_back(initialCostAppr);
	  initialTours.push_back(initialTourAppr);


	  timer.start();

	  GeneticAlgorithm geneticAlgorithm(tsp.dimension(),
			  tsp.distanceMatrix(),
			  options.cutoffTime(),
			  &trcFile,
			  &timer,
			  options.randomSeed(),
			  initialCosts,
			  initialTours);
	  tourCost = geneticAlgorithm.solve(tour);

	  timer.stop();
  }
  else {
    std::cerr << "Unknown algorithm type '" << algorithm << "'." << std::endl;
    return 1;
  }

  assert(tour.size() == tsp.dimension());

  // Write the final solution
  solnFile << tourCost << std::endl;
  for (unsigned i = 0; i < (tsp.dimension() - 1); ++i) {
    solnFile << tour[i] << ",";
  }
  solnFile << tour[tsp.dimension() - 1];

  std::cout << "Optimal tour cost is: " << tsp.optimalCost() << std::endl;
  std::cout << "Estimated tour cost is: " << tourCost << std::endl;
  std::cout << "Relative error: " << (tourCost - tsp.optimalCost()) * 1.0 / tsp.optimalCost() << std::endl;
  std::cout << "Time taken for estimation: " << timer.elapsed() << " seconds" << std::endl;

  return 0;
}
