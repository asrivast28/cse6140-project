/*
 * GreedyHeuristic.cpp
 *
 *  Created on: Nov 18, 2014
 *      Author: Parul
 */

#include <cassert>
#include <iterator>
#include <limits>
#include <iostream>
#include "GreedyHeuristic.hpp"

const unsigned GreedyHeuristic::PLUS_INFINITY = std::numeric_limits<unsigned>::max();


GreedyHeuristic::GreedyHeuristic(
		const unsigned dimension,
		std::vector<std::vector<unsigned> >&& distanceMatrix
) : m_distanceMatrix(distanceMatrix),
  m_dimension(dimension)
{
	//std::cout << dimension << std::endl;
}

void
	GreedyHeuristic::farthestInsertion(
		  std::vector<unsigned>& tour
		  )const
{
	unsigned minDist, maxDist, node, nodeInd;
	unsigned minNode[2], maxNode[2];

	//displayDistMtx(m_dimension, m_distanceMatrix);

	std::vector<unsigned> notInTour;

	maxDist = 0;
	for (unsigned i = 1; i <= m_dimension; i++)
		for (unsigned j = 1; j <= m_dimension; j++)		//add second node to tour s.t. it is at max dist from node 1
			if(m_distanceMatrix[i][j] > maxDist)
			{
				maxDist = m_distanceMatrix[i][j];
				maxNode[0] = i;
				maxNode[1] = j;
				//std::cout << maxDist;
			}
	tour.push_back(maxNode[0]);		//add first node
	tour.push_back(maxNode[1]);
	tour.push_back(maxNode[0]);			//complete the cycle

	for (unsigned i = 1; i <= m_dimension; i++)		//add the remaining nodes to notIntour
		if((i!= maxNode[0])&&(i!=maxNode[1]))
			notInTour.push_back(i);

	while( notInTour.size() != 0)
		{
			maxDist = 0;
			for (unsigned j = 0; j < notInTour.size() ; j++)	//for all nodes not in tour
				for (unsigned i = 0; i < tour.size(); i++)	//for all nodes in the tour
					if(maxDist < m_distanceMatrix[tour[i]][notInTour[j]])	//find the node tour[i] and notInTour[j] that are maximum distance apart
					{
						nodeInd = j;
						maxDist = m_distanceMatrix[tour[i]][notInTour[j]];
					}

			minDist = PLUS_INFINITY;
			for (unsigned i = 0; i < tour.size()-1; i++)	//find the position for selected notInTour node in tour such that tour length is reduced minimally
			{
				if (minDist > m_distanceMatrix[tour[i]][notInTour[nodeInd]]+m_distanceMatrix[notInTour[nodeInd]][tour[i+1]]-m_distanceMatrix[tour[i]][tour[i+1]])
				{
					minDist = m_distanceMatrix[tour[i]][notInTour[nodeInd]]+m_distanceMatrix[notInTour[nodeInd]][tour[i+1]]-m_distanceMatrix[tour[i]][tour[i+1]];
					minNode[0] = nodeInd;
					minNode[1] = i;
				}
			}
			//std::cout << minNode[0];
			tour.insert(tour.begin()+minNode[1]+1,notInTour[minNode[0]]);
			//displayVector(tour);
			notInTour.erase(notInTour.begin()+minNode[0]);
			//displayVector(notInTour);

		}
	tour.pop_back();
	return;
}

unsigned GreedyHeuristic::getTour(
  std::vector<unsigned>& tour
)const
{
  farthestInsertion(tour);

  unsigned approximateCost = 0;
  for (unsigned i = 0; i < (m_dimension - 1); ++i) {
    approximateCost += m_distanceMatrix[tour[i]][tour[i+1]];
  }
  approximateCost += m_distanceMatrix[tour[0]][tour[m_dimension-1]];
  // displayVector(tour);
  return approximateCost;
}

void GreedyHeuristic::displayVector(std::vector<unsigned> v)const
{
	std::cout<<"tour"<<std::endl;
	for (unsigned i = 0; i < v.size(); i++)
		std::cout<< v[i]<<std::endl;
}

void GreedyHeuristic::displayDistMtx(const unsigned dimension,std::vector<std::vector<unsigned> > distanceMatrix )const
{
	for (unsigned i = 0; i < dimension; i++)
		for (unsigned j = 0; j < dimension; j++)
			std::cout << i<<"  "<<j<<"  "<<distanceMatrix[i][j]<<std::endl;
}
