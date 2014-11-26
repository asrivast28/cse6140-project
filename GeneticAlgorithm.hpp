/*
 * GeneticAlgorithm.hpp
 *
 *  Created on: 2014. 11. 25.
 *  Author: Parul Awasthy (<pawasthy@gatech.edu>)
 * 			Young Jin Kim (<ykim362@gatech.edu>)
 * 			Ankit Srivastava (<asrivast@gatech.edu>)
 */

#ifndef GENETICALGORITHM_HPP_
#define GENETICALGORITHM_HPP_

#include "TSPInstance.hpp"
#include <random>

class Timer;

class GeneticAlgorithm {

public:
	GeneticAlgorithm(const unsigned dimension,
			const std::vector<std::vector<unsigned>>& distanceMatrix,
			const unsigned cutoffTime,
			std::ofstream* trcFile,
			const Timer* timer,
			const unsigned randomSeed);

	unsigned
	solve(std::vector<unsigned>& tour);

private:
	unsigned m_dimension;

	unsigned m_cutoffTime;

	std::ofstream* m_trcFile;

	const Timer* m_timer;

	unsigned m_bestCost;

	std::minstd_rand m_rand;

	void
	initialPopulate(const unsigned sizePopulation,
			std::vector<std::vector<unsigned>>& population);

	void
	crossover(const std::vector<unsigned>& parent1,
			const std::vector<unsigned>& parent2,
			std::vector<unsigned>& child1,
			std::vector<unsigned>& child2);

	void
	mutate(std::vector<unsigned>& tour);

	void
	select(std::vector<std::vector<unsigned>>& population,
			std::vector<std::vector<unsigned>>& selected);

};


#endif /* GENETICALGORITHM_HPP_ */
