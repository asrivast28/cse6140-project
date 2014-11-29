/*
 * GeneticAlgorithm.cpp
 *
 *  Created on: 2014. 11. 25.
 *  Author: Parul Awasthy (<pawasthy@gatech.edu>)
 * 			Young Jin Kim (<ykim362@gatech.edu>)
 * 			Ankit Srivastava (<asrivast@gatech.edu>)
 */

#include "GeneticAlgorithm.hpp"
#include "Timer.hpp"
#include <algorithm>
#include <boost/iterator/counting_iterator.hpp>
#include <forward_list>

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
	m_randMax = m_rand.max();

	// Sets the size of the population
	m_sizePopulation = dimension*dimension/8;

	// Sets the number of generations
	m_numGeneration = m_sizePopulation*4;

	// Sets the mutation rate
	m_rateMutate = 0.05;

	// Set the size of tournaments
	m_sizeTournament = m_sizePopulation/10;

	// Initialize the edge list
	for (unsigned ii = 1; ii <= m_dimension; ii++) {

		std::vector<unsigned> oneRow(m_dimension);

		for (unsigned jj = 1; jj <= m_dimension; jj++) {
			unsigned distance = distanceMatrix[ii][jj];

			// Distances
			oneRow[jj - 1] = distance;
		}

		m_distances.push_back(oneRow);
	}

}

unsigned
GeneticAlgorithm::solve(std::vector<unsigned>& tour) {

	// Population
	std::vector<std::vector<unsigned>> population1(m_sizePopulation, std::vector<unsigned>(m_dimension, 0));
	std::vector<unsigned> fitness1(m_sizePopulation);
	std::vector<std::vector<unsigned>> population2(m_sizePopulation, std::vector<unsigned>(m_dimension, 0));
	std::vector<unsigned> fitness2(m_sizePopulation);
	std::vector<std::vector<unsigned>>* popParent = nullptr;
	std::vector<unsigned>* fitParent = nullptr;
	std::vector<std::vector<unsigned>>* popChild = nullptr;
	std::vector<unsigned>* fitChild = nullptr;

	std::vector<unsigned> selected1(m_dimension), selected2(m_dimension);

	// Initialize the population
	initialPopulate(m_sizePopulation, population1, fitness1, m_bestCost, m_bestTour);

	// Evolution
	for (unsigned ii = 0; ii < m_numGeneration; ii++) {

		unsigned oldCost = m_bestCost;

		if (ii % 2 == 0) {
			popParent = &population1;
			fitParent = &fitness1;
			popChild = &population2;
			fitChild = &fitness2;
		} else {
			popParent = &population2;
			fitParent = &fitness2;
			popChild = &population1;
			fitChild = &fitness1;
		}


		// Handling cutoff time
		if (m_timer->elapsed() > m_cutoffTime) {
			break;
		}

		// Eliticism - Keep the best one
		(*popChild)[0] = m_bestTour;
		(*fitChild)[0] = m_bestCost;

		std::vector<unsigned> child(m_dimension);
		unsigned newFitness = 0;

		for (unsigned jj = 1; jj < m_sizePopulation; jj++) {

			// Selection
			select(popParent, fitParent, selected1, selected2);

			// Crossover
			newFitness = crossover(selected1, selected2, child);

			// Mutation
			// Randomly select genes to mutate
//			newFitness = mutate(child);

			(*popChild)[jj] = child;
			(*fitChild)[jj] = newFitness;

			// Update best tour
			if (m_bestCost > newFitness) {
				m_bestCost = newFitness;
				m_bestTour = child;
			}
		}

		// Trace the solution when any improvement has acquired
		if (oldCost > m_bestCost) {
			// Save to the trace file
			(*m_trcFile) << m_timer->elapsed() << ", " << m_bestCost << std::endl;
		}

	}

	// Record the best solution
	tour.clear();
	for (unsigned ii = 0; ii < m_dimension; ii++) {
		tour.push_back(m_bestTour[ii] + 1);
	}

	return m_bestCost;

}


void
GeneticAlgorithm::initialPopulate(const unsigned sizePopulation,
		std::vector<std::vector<unsigned>>& population,
		std::vector<unsigned>& fitness,
		unsigned& bestCost,
		std::vector<unsigned>& bestTour) {

	bestCost = std::numeric_limits<unsigned>::max();

	for (unsigned ii = 0; ii < sizePopulation; ii++) {

		// Make a new tour sequnce
		std::vector<unsigned> indGene;
		std::copy(boost::counting_iterator<unsigned>(0),
				boost::counting_iterator<unsigned>(m_dimension),
				std::back_inserter(indGene));

		shuffle(indGene.begin(), indGene.end(), m_rand);

		// Calculate fitness
		unsigned curDistance = m_distances[indGene[0]][indGene[m_dimension - 1]] ;
		for (unsigned jj = 0; jj < m_dimension - 1; jj++) {
			curDistance += m_distances[indGene[jj]][indGene[jj + 1]];
		}

		fitness[ii] = curDistance;
		population[ii] = indGene;

		if (curDistance < bestCost) {
			bestCost = curDistance;
			bestTour = indGene;
		}
	}
}

unsigned
GeneticAlgorithm::crossover(const std::vector<unsigned>& parent1,
		const std::vector<unsigned>& parent2,
		std::vector<unsigned>& child) {

	unsigned startIdx = (unsigned) ((double)m_rand()/m_randMax * m_dimension);
	unsigned endIdx = (unsigned) ((double)m_rand()/m_randMax * m_dimension);
	if (startIdx == m_dimension) {
		startIdx = 0;
	}
	if (endIdx == m_dimension) {
		endIdx = 0;
	}

	if (startIdx > endIdx) {
		unsigned tmp = startIdx;
		startIdx = endIdx;
		endIdx = tmp;
	}

	std::forward_list<unsigned> fromParent1;
	std::forward_list<unsigned>::iterator itr = fromParent1.before_begin();
	for (unsigned ii = startIdx; ii <= endIdx; ii++) {
		itr = fromParent1.insert_after(itr, parent1[ii]);
	}

	unsigned curCost = 0;
	unsigned idxParent2 = 0;
	for (unsigned ii = 0; ii < m_dimension; ii++) {
		if (ii < startIdx || ii > endIdx) {
			bool isExist = true;
			while (isExist) {
				isExist = false;

//				for (std::forward_list<unsigned>::iterator itrFind = fromParent1.begin(); itrFind != fromParent1.end(); itrFind++) {

//					if ((*itrFind) == parent2[idxParent2]) {
				if (std::find(fromParent1.begin(), fromParent1.end(), parent2[idxParent2]) != fromParent1.end()) {
					isExist = true;
					idxParent2++;
//					break;
				}
//				}
			}

			child[ii] = parent2[idxParent2];
			idxParent2++;

			if (ii > 0)
				curCost += m_distances[child[ii]][child[ii - 1]];
		} else {
			child[ii] = parent1[ii];

			if (ii > 0)
				curCost += m_distances[child[ii]][child[ii - 1]];
		}
	}

	curCost += m_distances[child[0]][child[m_dimension - 1]];

	return curCost;
}

unsigned
GeneticAlgorithm::mutate(std::vector<unsigned>& tour) {
	return 0;
}

void
GeneticAlgorithm::select(const std::vector<std::vector<unsigned>>* population,
		const std::vector<unsigned>* fitness,
		std::vector<unsigned>& selected1,
		std::vector<unsigned>& selected2) {

	unsigned chFitness1 = std::numeric_limits<unsigned>::max();
	unsigned chFitness2 = std::numeric_limits<unsigned>::max();

	unsigned curFitness = 0;

	// Tournament selection
	for (unsigned ii = 0; ii < m_sizeTournament; ii++) {
		unsigned randIdx = (unsigned) ((double)m_rand()/m_randMax * m_sizePopulation);
		if (randIdx == m_dimension) {
			randIdx = 0;
		}

		curFitness = (*fitness)[randIdx];
		if (curFitness < chFitness1) {
			chFitness1 = curFitness;

			selected1 = (*population)[randIdx];
		}
	}

	// Tournament selection
	for (unsigned ii = 0; ii < m_sizeTournament; ii++) {
		unsigned randIdx = (unsigned) ((double)m_rand()/m_randMax * m_sizePopulation);
		if (randIdx == m_dimension) {
			randIdx = 0;
		}

		curFitness = (*fitness)[randIdx];
		if (curFitness < chFitness2) {
			chFitness2 = curFitness;

			selected2 = (*population)[randIdx];
		}
	}

}



