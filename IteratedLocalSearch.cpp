/*
 * IteratedLocalSearch.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: Parul
 */


#include <cassert>
#include <iterator>
#include <limits>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GreedyHeuristic.hpp"
#include "IteratedLocalSearch.hpp"

const unsigned IteratedLocalSearch::PLUS_INFINITY = std::numeric_limits<unsigned>::max();

IteratedLocalSearch::IteratedLocalSearch(
		const unsigned dimension,
		std::vector<std::vector<unsigned> >&& distanceMatrix,
		const unsigned cutoffTime,
		const Timer* timer,
		std::ofstream* trcFile,
		unsigned seed
) : m_distanceMatrix(distanceMatrix),
  m_dimension(dimension),
  m_cutoffTime(cutoffTime),
  m_timer(timer),
  m_trcFile(trcFile),
  m_seed(seed)
{
	if (m_cutoffTime == 0)
		m_cutoffTime = 300;
	MAX_EXCHANGE_ITERATION = m_dimension * m_dimension;
	MAX_PERTURB_ITERATION = m_dimension * m_dimension * m_dimension* m_dimension;
	MAX_FUITLESS_MOVES = m_dimension * m_dimension *2;
	//std::cout << dimension << std::endl;
}

void
IteratedLocalSearch::threeOptExchange(
		  std::vector<unsigned>& tour
		  )
{
	//std::cout<<"Tour before 3-opt"<<std::endl;
	//displayVector(tour);
	unsigned indx1, indx2, indx3,node;

	indx1 = rand()%m_dimension;
	indx2 = rand()%(m_dimension-1);
	indx3 = indx2+1;

	while(true){
		if ((indx1 == indx2)||(indx1 == indx3))
			indx1 = rand()%m_dimension;
		else
			break;
	}

	//std::cout<<indx1<<" "<<indx2<<" "<<indx3<<std::endl;

	node = tour[indx3];
	tour.erase(tour.begin()+indx3);

	tour.insert(tour.begin()+indx1,node);

	//std::cout<<"Tour after 3-opt"<<std::endl;
	//displayVector(tour);

	return;
}

void
IteratedLocalSearch::doubleBridgePerturbation(
		  std::vector<unsigned>& tour
		  )
{
	//std::cout<<"Perturbing"<<std::endl;

	//displayVector(tour);

	unsigned indx[4];
	std::vector<unsigned> newTour, part1, part2, part3,part4;

	for (unsigned i = 0; i<4; i++)
	{
		indx[i] = rand()%m_dimension;
	}

	std::sort(indx, indx+4);

	//std::cout<<indx[0]<<" "<<indx[1]<<" "<<indx[2]<<" "<<indx[3]<<std::endl;

	//inserting order part 2,1,4,3

	//part2
	for(unsigned i = indx[0]+1; i<=indx[1];i++)
		newTour.push_back(tour[i]);

	//part1
	for(unsigned i = indx[3]+1; i<m_dimension;i++)
		newTour.push_back(tour[i]);
	for(unsigned i = 0; i<=indx[0];i++)
		newTour.push_back(tour[i]);

	//part4
	for(unsigned i = indx[2]+1; i<=indx[3];i++)
		newTour.push_back(tour[i]);

	//part3
	for(unsigned i = indx[1]+1; i<=indx[2];i++)
		newTour.push_back(tour[i]);

	//newTour.push_back(newTour[0]);

	//std::cout<<"Tour"<<std::endl;
	//displayVector(newTour);

	tour = newTour;

	return;

}

void IteratedLocalSearch::localSearch( std::vector<unsigned>& tour
	  )
{
	m_best_cost = tourCost(tour);
	m_best_so_far_tour = tour;
	double cost;
	unsigned fruitless_perturb = 0;

	for(unsigned i = 0; i < MAX_PERTURB_ITERATION; i++)
	{
		fruitless_perturb++;
		doubleBridgePerturbation(tour);
		if ((m_timer->elapsed() > m_cutoffTime) )
			break;
		//if (fruitless_perturb >= MAX_FUITLESS_MOVES)
			//		break;

		int count_of_bad_moves = 0;
		for (unsigned j = 0; j < MAX_EXCHANGE_ITERATION; j++)
		{
			if (count_of_bad_moves >= 1){
				tour = m_best_so_far_tour;
				break;
			}

			threeOptExchange(tour);
			cost = tourCost(tour);
			if (cost > m_best_cost)
				count_of_bad_moves++;
			else
			{
				if(cost < m_best_cost)
				{
					std::cout<<"Better solution found with cost: "<<cost<<std::endl;
					displayVector(tour);
					std::cout<<fruitless_perturb<<std::endl;
					// Save to the trace file
					(*m_trcFile)<< m_timer->elapsed() << ", " << cost << std::endl;
				}
				m_best_so_far_tour = tour;
				m_best_cost = cost;
				count_of_bad_moves = 0;
				fruitless_perturb = 0;
			}
		}

	}
	std::cout<<fruitless_perturb<<std::endl;
	tour = m_best_so_far_tour;
	return;
}

unsigned IteratedLocalSearch::tourCost(std::vector<unsigned> tour)
{
	unsigned approximateCost = 0;
	for (unsigned i = 0; i < (m_dimension - 1); ++i) {
	approximateCost += m_distanceMatrix[tour[i]][tour[i+1]];
	}
	approximateCost += m_distanceMatrix[tour[0]][tour[m_dimension-1]];
	//displayVector(tour);

	//std::cout<<"Cost: "<<approximateCost<<std::endl;

	return approximateCost;
}

unsigned IteratedLocalSearch::getTour(
  std::vector<unsigned>& tour
)
{
	GreedyHeuristic greedy_obj(m_dimension,std::move(m_distanceMatrix));

	srand(m_seed);
	greedy_obj.farthestInsertion(tour);
	unsigned cost = tourCost(tour);
	(*m_trcFile)<< m_timer->elapsed() << ", " << cost << std::endl;
	//for (unsigned i = 1; i <=m_dimension;i++)
		//tour.push_back(i);
	//tour.push_back(1);

	displayVector(tour);

	localSearch(tour);

	//tour.pop_back();
	//std::cout<<"Done"<<std::endl;
	return tourCost(tour);
}

void IteratedLocalSearch::displayVector(std::vector<unsigned> v)
{
	//std::cout<<"tour"<<std::endl;
	for (unsigned i = 0; i < v.size(); i++)
		std::cout<< v[i]<<" ";
	std::cout<<std::endl;
}

void IteratedLocalSearch::displayDistMtx(const unsigned dimension,std::vector<std::vector<unsigned> > distanceMatrix )
{
	for (unsigned i = 0; i < dimension; i++)
		for (unsigned j = 0; j < dimension; j++)
			std::cout << i<<"  "<<j<<"  "<<distanceMatrix[i][j]<<std::endl;
}


