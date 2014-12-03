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
			const unsigned randomSeed,
			const std::vector<unsigned>& initialCosts,
			const std::vector<std::vector<unsigned>>& initialTours);

	unsigned
	solve(std::vector<unsigned>& tour);

private:
	unsigned m_dimension;

	unsigned m_cutoffTime;

	std::ofstream* m_trcFile;

	const Timer* m_timer;

	unsigned m_bestCost;
	std::vector<unsigned> m_bestTour;

	std::vector<unsigned> m_initialCosts;
	std::vector<std::vector<unsigned>> m_initialTours;

//	std::minstd_rand m_rand;
//	unsigned m_randMax;

	unsigned m_sizePopulation;
	unsigned m_numGeneration;
	double m_rateMutate;
	unsigned m_sizeTournament;

	std::vector<std::vector<unsigned>> m_distances;

	void
	initialPopulate(const unsigned sizePopulation,
			std::vector<std::vector<unsigned>>& population,
			std::vector<unsigned>& fitness,
			unsigned& bestCost,
			std::vector<unsigned>& bestTour);

	unsigned
	crossoverGNX(const std::vector<unsigned>& parent1,
			const std::vector<unsigned>& parent2,
			std::vector<unsigned>& child);

	unsigned
	crossoverSCX(const std::vector<unsigned>& parent1,
			const std::vector<unsigned>& parent2,
			std::vector<unsigned>& child);

	unsigned
	mutate(std::vector<unsigned>& tour, unsigned curCost);

	void
	select(const std::vector<std::vector<unsigned>>* population,
			const std::vector<unsigned>* fitness,
			std::vector<unsigned>& selected1,
			std::vector<unsigned>& selected2);

};


#endif /* GENETICALGORITHM_HPP_ */
