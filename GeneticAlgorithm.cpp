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
#include <list>
#include <map>
#include <stdlib.h>

GeneticAlgorithm::GeneticAlgorithm(const unsigned dimension,
		const std::vector<std::vector<unsigned>>& distanceMatrix,
		const unsigned cutoffTime,
		std::ofstream* trcFile,
		const Timer* timer,
		const unsigned randomSeed,
		const std::vector<unsigned>& initialCosts,
		const std::vector<std::vector<unsigned>>& initialTours)
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
//	srand(randomSeed);


	m_initialCosts = initialCosts;
	m_initialTours = initialTours;

	// Sets the size of the population
	m_sizePopulation = dimension*3;
	if (m_sizePopulation > 1000) {
		m_sizePopulation /= 2;
	}

	// Sets the number of generations
	m_numGeneration = dimension*dimension*16;

	// Sets the mutation rate
	m_rateMutate = 0.1;

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

	std::vector<unsigned> selected1(m_dimension), selected2(m_dimension);

	// Initialize the population
	initialPopulate(m_sizePopulation, population1, fitness1, m_bestCost, m_bestTour);
	(*m_trcFile) << m_timer->elapsed() << ", " << m_bestCost << std::endl;

	// Evolution
	for (unsigned ii = 0; ii < m_numGeneration; ii++) {

		unsigned oldCost = m_bestCost;

		// Handling cutoff time
		if (m_timer->elapsed() > m_cutoffTime) {
			break;
		}

		// Eliticism - Keep the best one
		if (ii % 2 == 0) {
			population2[0] = m_bestTour;
			fitness2[0] = m_bestCost;
		} else {
			population1[0] = m_bestTour;
			fitness1[0] = m_bestCost;
		}

		std::vector<unsigned> child(m_dimension);
		unsigned newFitness = 0;

		for (unsigned jj = 1; jj < m_sizePopulation; jj++) {

			// Selection
			if (ii % 2 == 0) {
				select(population1, fitness1, selected1, selected2);
			} else {
				select(population2, fitness2, selected1, selected2);
			}

			// Crossover
			newFitness = crossoverSCX(selected1, selected2, child);

			// In case of new best solution, keep it and don't mutate
			if (newFitness >= m_bestCost) {

				// Mutation
				// Randomly select genes to mutate
				newFitness = mutate(child, newFitness);

			}

			if (ii % 2 == 0) {
				population2[jj] = child;
				fitness2[jj] = newFitness;
			} else {
				population1[jj] = child;
				fitness1[jj] = newFitness;
			}

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

		std::vector<unsigned> indGene;

		if (ii < m_initialTours.size()) {
			fitness[ii] = m_initialCosts[ii];

			std::vector<unsigned> curTour = m_initialTours[ii];

			// Find the city 1
			unsigned offset = 0;
			for (unsigned jj = 0; jj < m_dimension; jj++) {
				if (curTour[jj] == 1) {
					offset = jj;
					break;
				}
			}

			for (unsigned jj = 0; jj < m_dimension; jj++) {
				if (jj + offset < m_dimension) {
					indGene.push_back(curTour[jj + offset] - 1);
				} else {
					indGene.push_back(curTour[jj + offset - m_dimension] - 1);
				}
			}

			population[ii] = indGene;

		} else {
			// Make a new tour sequnce
			std::copy(boost::counting_iterator<unsigned>(1),
					boost::counting_iterator<unsigned>(m_dimension),
					std::back_inserter(indGene));

			shuffle(indGene.begin(), indGene.end(), m_rand);
//			random_shuffle(indGene.begin(), indGene.end());

			indGene.insert(indGene.begin(), 0);

			// Calculate fitness
			unsigned curDistance = m_distances[indGene[0]][indGene[m_dimension - 1]] ;
			for (unsigned jj = 0; jj < m_dimension - 1; jj++) {
				curDistance += m_distances[indGene[jj]][indGene[jj + 1]];
			}

			fitness[ii] = curDistance;
			population[ii] = indGene;
		}

		if (fitness[ii] < bestCost) {
			bestCost = fitness[ii];
			bestTour = indGene;
		}
	}
}

unsigned
GeneticAlgorithm::crossoverGNX(const std::vector<unsigned>& parent1,
		const std::vector<unsigned>& parent2,
		std::vector<unsigned>& child) {

//	unsigned startIdx = rand() % (m_dimension - 1) + 1; //(unsigned) ((double)rand()/RAND_MAX * (m_dimension - 1)) + 1;
//	unsigned endIdx = rand() % (m_dimension - 1) + 1;
	unsigned startIdx = m_rand() % (m_dimension - 1) + 1;
	unsigned endIdx = m_rand() % (m_dimension - 1) + 1;
	if (startIdx == m_dimension) {
		startIdx = 1;
	}
	if (endIdx == m_dimension) {
		endIdx = 1;
	}

	if (startIdx > endIdx) {
		unsigned tmp = startIdx;
		startIdx = endIdx;
		endIdx = tmp;
	}

	std::list<unsigned> fromParent1;
	std::list<unsigned>::iterator itr = fromParent1.begin();
	for (unsigned ii = startIdx; ii <= endIdx; ii++) {
		fromParent1.push_back(parent1[ii]);
	}

	unsigned curCost = 0;
	unsigned idxParent2 = 1;
	child[0] = 0;
	for (unsigned ii = 1; ii < m_dimension; ii++) {
		if (ii < startIdx || ii > endIdx) {
			bool isExist = true;
			while (isExist) {
				isExist = false;

				itr = std::find(fromParent1.begin(), fromParent1.end(), parent2[idxParent2]);
				if (itr != fromParent1.end()) {
					fromParent1.erase(itr);
					isExist = true;
					idxParent2++;
				}
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
GeneticAlgorithm::crossoverSCX(const std::vector<unsigned>& parent1,
		const std::vector<unsigned>& parent2,
		std::vector<unsigned>& child) {

	unsigned newCost = 0;

	std::map<unsigned, unsigned> mapParent1;
	std::map<unsigned, unsigned> mapParent2;
	std::vector<unsigned> remaining(m_dimension);

	for (unsigned ii = 0; ii < m_dimension - 1; ii++) {
		mapParent1[parent1[ii]] = parent1[ii + 1];
		mapParent2[parent2[ii]] = parent2[ii + 1];
		remaining[ii] = 1;
	}
	mapParent1[parent1[m_dimension - 1]] = parent1[0];
	mapParent2[parent2[m_dimension - 1]] = parent2[0];
	remaining[m_dimension - 1] = 1;

	// loop
	unsigned firstAvailable = 1;
	child[0] = 0;
	remaining[0] = 0;

	for (unsigned ii = 1; ii < m_dimension; ii++) {

		unsigned candidate1 = mapParent1[child[ii - 1]];
		unsigned candidate2 = mapParent2[child[ii - 1]];

		if (remaining[candidate1] != 1) {
			candidate1 = firstAvailable;
		}

		if (remaining[candidate2] != 1) {
			candidate2 = firstAvailable;
		}

		// select better one
		unsigned dist1 = m_distances[child[ii - 1]][candidate1];
		unsigned dist2 = m_distances[child[ii - 1]][candidate2];

		if (dist1 <= dist2) {
			child[ii] = candidate1;
			remaining[candidate1] = 0;
			newCost += dist1;
		} else {
			child[ii] = candidate2;
			remaining[candidate2] = 0;
			newCost += dist2;
		}

		while(remaining[firstAvailable] == 0) {
			firstAvailable++;
		}
	}

	newCost += m_distances[child[0]][child[m_dimension - 1]];

	return newCost;
}

unsigned
GeneticAlgorithm::mutate(std::vector<unsigned>& tour, unsigned curCost) {

	unsigned newCost = curCost;


	for (unsigned ii = 1; ii < m_dimension; ii++) {

		unsigned subtract1 = 0;
		if (ii == 1) {
			subtract1 += m_distances[tour[ii]][tour[ii + 1]];
			subtract1 += m_distances[tour[ii]][tour[0]];
		} else if (ii == m_dimension - 1) {
			subtract1 += m_distances[tour[ii - 1]][tour[ii]];
			subtract1 += m_distances[tour[0]][tour[m_dimension - 1]];
		} else {
			subtract1 += m_distances[tour[ii - 1]][tour[ii]];
			subtract1 += m_distances[tour[ii]][tour[ii + 1]];
		}

//			if ((double)rand()/RAND_MAX < m_rateMutate) {
		if ((double)m_rand()/m_randMax < m_rateMutate) {

//				unsigned swapCity = rand() % (m_dimension - 1) + 1;
			unsigned swapCity = m_rand() % (m_dimension - 1) + 1;
			if (swapCity == m_dimension) {
				swapCity = 1;
			}
			unsigned subtract2 = 0;

			// Calculate new cost
			// Subtract
			if (swapCity == 1) {
				subtract2 += m_distances[tour[swapCity]][tour[swapCity + 1]];
				subtract2 += m_distances[tour[swapCity]][tour[0]];
			} else if (swapCity == m_dimension - 1) {
				subtract2 += m_distances[tour[swapCity - 1]][tour[swapCity]];
				subtract2 += m_distances[tour[0]][tour[m_dimension - 1]];
			} else {
				subtract2 += m_distances[tour[swapCity - 1]][tour[swapCity]];
				subtract2 += m_distances[tour[swapCity]][tour[swapCity + 1]];
			}

			newCost -= (subtract1 + subtract2);

			unsigned tmpCity = tour[ii];
			tour[ii] = tour[swapCity];
			tour[swapCity] = tmpCity;

			// Addition of new paths
			if (ii == 1) {
				newCost += m_distances[tour[ii]][tour[ii + 1]];
				newCost += m_distances[tour[ii]][tour[0]];
			} else if (ii == m_dimension - 1) {
				newCost += m_distances[tour[ii - 1]][tour[ii]];
				newCost += m_distances[tour[0]][tour[m_dimension - 1]];
			} else {
				newCost += m_distances[tour[ii - 1]][tour[ii]];
				newCost += m_distances[tour[ii]][tour[ii + 1]];
			}

			if (swapCity == 1) {
				newCost += m_distances[tour[swapCity]][tour[swapCity + 1]];
				newCost += m_distances[tour[swapCity]][tour[0]];
			} else if (swapCity == m_dimension - 1) {
				newCost += m_distances[tour[swapCity - 1]][tour[swapCity]];
				newCost += m_distances[tour[0]][tour[m_dimension - 1]];
			} else {
				newCost += m_distances[tour[swapCity - 1]][tour[swapCity]];
				newCost += m_distances[tour[swapCity]][tour[swapCity + 1]];
			}

		}

	}

	return newCost;
}

void
GeneticAlgorithm::select(const std::vector<std::vector<unsigned>>& population,
		const std::vector<unsigned>& fitness,
		std::vector<unsigned>& selected1,
		std::vector<unsigned>& selected2) {

	unsigned chFitness1 = std::numeric_limits<unsigned>::max();
	unsigned chFitness2 = std::numeric_limits<unsigned>::max();

	unsigned curFitness = 0;

	// Tournament selection
	for (unsigned ii = 0; ii < m_sizeTournament; ii++) {
//		unsigned randIdx = rand() % m_sizePopulation;
		unsigned randIdx = m_rand() % m_sizePopulation;

		curFitness = fitness[randIdx];
		if (curFitness < chFitness1) {
			chFitness1 = curFitness;

			selected1 = population[randIdx];
		}
	}

	// Tournament selection
	for (unsigned ii = 0; ii < m_sizeTournament; ii++) {
//		unsigned randIdx = rand() % m_sizePopulation;
		unsigned randIdx = m_rand() % m_sizePopulation;

		curFitness = fitness[randIdx];
		if (curFitness < chFitness2) {
			chFitness2 = curFitness;

			selected2 = population[randIdx];
		}
	}

}



