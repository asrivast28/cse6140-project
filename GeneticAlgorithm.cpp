/*
 * GeneticAlgorithm.cpp
 *
 *  Created on: 2014. 11. 25.
 *  Author: Parul Awasthy (<pawasthy@gatech.edu>)
 * 			Young Jin Kim (<ykim362@gatech.edu>)
 * 			Ankit Srivastava (<asrivast@gatech.edu>)
 */

#include "GeneticAlgorithm.hpp"

GeneticAlgorithm::GeneticAlgorithm(const unsigned dimension,
		const std::vector<std::vector<unsigned>>& distanceMatrix,
		const unsigned cutoffTime,
		std::ofstream* trcFile,
		const Timer* timer,
		const unsigned randomSeed)
		: m_dimension(dimension),
		  m_cutoffTime(cutoffTime),
		  m_trcFile(trcFile),
		  m_timer(timer),
		  m_bestCost(std::numeric_limits<unsigned>::max() ){

	// In case of no cutoff time, set default value as 300 seconds
	if (m_cutoffTime == 0) {
		m_cutoffTime = 300;
	}

	// Initialize random number generator with given seed
	m_rand.seed(randomSeed);

}

unsigned
GeneticAlgorithm::solve(std::vector<unsigned>& tour) {




	return m_bestCost;

}


void
GeneticAlgorithm::initialPopulate(const unsigned sizePopulation,
		std::vector<std::vector<unsigned>>& population) {

}

void
GeneticAlgorithm::crossover(const std::vector<unsigned>& parent1,
		const std::vector<unsigned>& parent2,
		std::vector<unsigned>& child1,
		std::vector<unsigned>& child2) {

}

void
GeneticAlgorithm::mutate(std::vector<unsigned>& tour) {

}

void
GeneticAlgorithm::select(std::vector<std::vector<unsigned>>& population,
		std::vector<std::vector<unsigned>>& selected) {

}



