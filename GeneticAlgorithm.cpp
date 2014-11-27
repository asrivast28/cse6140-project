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

	// Sets the size of the population
	m_sizePopulation = dimension*dimension/8;

	// Sets the number of generations
	m_numGeneration = m_sizePopulation*2;

	// Sets the mutation rate
	m_rateMutate = 0.05;

}

unsigned
GeneticAlgorithm::solve(std::vector<unsigned>& tour) {

	// Population
	std::vector<std::vector<unsigned>> population;
	std::vector<std::vector<unsigned>> selected;

	// Initialize the population
	initialPopulate(m_sizePopulation, population);

	// Evolution
	for (unsigned ii = 0; ii < m_numGeneration; ii++) {

		std::vector<unsigned> child1;
		std::vector<unsigned> child2;

		// Selection
		select(population, selected);

		// Crossover
		crossover(selected[0],
				selected[1],
				child1,
				child2);

		// Mutation
		// Randomly select genes to mutate
		mutate(child1);
	}

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



