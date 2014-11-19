/*
 * TreeNode.cpp
 *
 *  Created on: Nov 16, 2014
 *  Author: Parul Awasthy (<pawasthy@gatech.edu>)
 * 			Young Jin Kim (<ykim362@gatech.edu>)
 * 			Ankit Srivastava (<asrivast@gatech.edu>)
 */

#include "TreeNode.hpp"
#include <bitset>
#include <limits>
#include <cmath>
#include <iostream>

std::vector<Edge> TreeNode::s_listEdges;

std::vector<std::vector<unsigned>> TreeNode::s_distances;

unsigned TreeNode::MAX_UINT = std::numeric_limits<unsigned>::max();


TreeNode::TreeNode(unsigned size) {

	// Initialize member variables
	m_dimension = size;

	m_lowerBound = MAX_UINT;
	m_totalCost = MAX_UINT;

	m_alive = false;

	// Initialize vector
	for (unsigned ii = 0; ii < m_dimension; ii++) {
		std::vector<char> destForOneCity(m_dimension);

		for (unsigned jj = 0; jj < m_dimension; jj++) {
			destForOneCity[jj] = 0;
		}

		m_constraint.push_back(destForOneCity);
	}
}

void
TreeNode::calcLowerBound() {
	unsigned lowerBound = 0;

	std::vector<unsigned> costOfNode(m_dimension);

	for (unsigned ii = 0; ii < m_dimension; ii++) {
		for (unsigned jj = 0; jj < m_dimension; jj++) {
			unsigned testCost = s_distances[ii][jj];
			costOfNode[jj] = s_distances[ii][jj];
		}

		costOfNode[ii] = MAX_UINT;

		std::vector<unsigned> included(m_dimension);
		int numIncluded = 0;

		for (unsigned jj = 0; jj < m_dimension; jj++) {
			if (m_constraint[ii][jj] == 1) {
				numIncluded++;
				included[numIncluded - 1] = costOfNode[jj];
				unsigned testCost = included[numIncluded - 1];
				costOfNode[jj] = MAX_UINT;
			}
		}

		unsigned smallest = 0;
		unsigned secondSmallest = 0;

		findSmallestTwo(costOfNode, smallest, secondSmallest);

		if (numIncluded == 1) {
			secondSmallest = smallest;
			smallest = included[0];
		} else if (numIncluded == 2) {
			smallest = included[0];
			secondSmallest = included[1];
		}

		if (smallest == MAX_UINT) {
			smallest = 0;
		}
		if (secondSmallest == MAX_UINT) {
			secondSmallest = 0;
		}

		lowerBound += (smallest + secondSmallest);
	}

	m_lowerBound = lowerBound/2;

	std::cout << "Lower bound: " << m_lowerBound << std::endl;
}

bool
TreeNode::isSolution() {
	// Find starting edge from node 1
	unsigned u = 0, v = 0;
	for (unsigned jj = 1; jj < m_dimension; jj++) {
		if (m_constraint[0][jj] == 1) {
			v = jj;
			break;
		}
	}

	if (v >= 0) {
		return checkCycle(u, v) == m_dimension;
	} else {
		return false;
	}
}

void
TreeNode::recordSolution() {

	m_travelPath.clear();

	// Find starting edge from node 1
	unsigned u = 0, v = 0;
	for (unsigned jj = 1; jj < m_dimension; jj++) {
		if (m_constraint[0][jj] == 1) {
			v = jj;
			break;
		}
	}

	m_totalCost = s_distances[u][v];
	m_travelPath.push_back(u);
	m_travelPath.push_back(v);

	unsigned start = 0;
	unsigned curPos = v;

	while (curPos != 0) {

		for (unsigned jj = 0; jj < m_dimension; jj++) {
			if (jj != start && m_constraint[curPos][jj] == 1) {
				start = curPos;
				curPos = jj;
				m_totalCost = s_distances[start][curPos];
				m_travelPath.push_back(curPos);
				break;
			}
		}

	}
}

unsigned
TreeNode::getCost() {
	return m_totalCost;
}

unsigned
TreeNode::getLowerBound() {
	return m_lowerBound;
}

void
TreeNode::setConstraint(std::vector<std::vector<char>> constraint) {
	m_constraint = constraint;
}

std::vector<std::vector<char>>
TreeNode::getConstraint() {
	return m_constraint;
}

unsigned
TreeNode::addEdge(unsigned idxEdge) {

	Edge curEdge;

	while (idxEdge < s_listEdges.size()) {

		curEdge = s_listEdges.at(idxEdge);
		int x = std::abs(curEdge.getAnyNode());
		int y = std::abs(curEdge.getTheOther());

		if (m_constraint[x - 1][y - 1] == 0) {
			break;
		}

		idxEdge++;
	}

	if (idxEdge < s_listEdges.size()) {
		if (curEdge.getAnyNode() < 0) {
			setIndConstraint(std::abs(curEdge.getAnyNode()) - 1, std::abs(curEdge.getTheOther()) - 1, -1);
		} else {
			setIndConstraint(std::abs(curEdge.getAnyNode()) - 1, std::abs(curEdge.getTheOther()) - 1, 1);
		}

	}

	return idxEdge;
}

void
TreeNode::expand() {

	// Exclude edges
	for (unsigned ii = 0; ii < m_dimension; ii++) {
		// Count the from cities
		int cntIncluded = 0;
		for (unsigned jj = 0; jj < m_dimension; jj++) {
			if (ii != jj && m_constraint[ii][jj] == 1) {
				cntIncluded++;
			}
		}

		if (cntIncluded >= 2) {
			for (unsigned jj = 0; jj < m_dimension; jj++) {
				if (ii != jj && m_constraint[ii][jj] == 0) {
					m_constraint[ii][jj] = -1;
					m_constraint[jj][ii] = -1;
				}
			}
		}
	}

	// Check per mature cycle
	for (unsigned ii = 0; ii < m_dimension; ii++) {
		for (unsigned jj = 0; jj < m_dimension; jj++) {
			unsigned cycleDimension = checkCycle(ii, jj);
			if (ii != jj && cycleDimension != 0 && cycleDimension < m_dimension
					&& m_constraint[ii][jj] == 0) {
				m_constraint[ii][jj] = -1;
				m_constraint[jj][ii] = -1;
			}
		}
	}

	// Include edges
	for (unsigned ii = 0; ii < m_dimension; ii++) {
		unsigned cntExcluded = 0;
		for (unsigned jj = 0; jj < m_dimension; jj++) {
			if (ii != jj && m_constraint[ii][jj] == -1) {
				cntExcluded++;
			}
		}

		if (cntExcluded >= m_dimension - 3) {
			for (unsigned jj = 0; jj < m_dimension; jj++) {
				if (ii != jj && m_constraint[ii][jj] == 0) {
					m_constraint[ii][jj] = 1;
					m_constraint[jj][ii] = 1;
				}
			}
		}
	}

	// Again

}

void
TreeNode::setIndConstraint(unsigned city1, unsigned city2, char val) {
//	std::vector<char> oneCity1 = m_constraint.at(city1);
//	oneCity1.assign(city2, val);

	m_constraint[city1][city2] = val;

//	std::vector<char> oneCity2 = m_constraint.at(city2);
//	oneCity2.assign(city1, val);

	m_constraint[city2][city1] = val;
}

unsigned
TreeNode::checkCycle(unsigned city1, unsigned city2) {

	std::vector<char> visited(m_dimension);
	unsigned cntCities = 0;

	visited[city1] = 1;
	visited[city2] = 1;

	unsigned start = city1;
	unsigned curPos = city2;

	unsigned passedEdges = 1;

	bool stop = true;

	while (curPos != city1 && passedEdges <= m_dimension && !stop) {
		stop = true;

		for (unsigned jj = 0; jj < m_dimension; jj++) {
			if (jj != start && m_constraint[curPos][jj] == 1) {
				visited[jj] = 1;
				passedEdges++;
				start = curPos;
				curPos = jj;
				stop = false;
				break;
			}
		}
	}

	// Not a cycle
	if ((curPos != m_dimension) && (passedEdges < m_dimension))
	{
		cntCities = 0;
	} else {
		// Count the number of cities
		for (unsigned ii = 0; ii < m_dimension; ii++) {
			if (visited[ii] == 1) {
				cntCities++;
			}
		}
	}

	return cntCities;
}

void
TreeNode::findSmallestTwo(const std::vector<unsigned>& costOfNode, unsigned& smallest, unsigned& secondSmallest) {
	if (costOfNode[0] <= costOfNode[1]) {
		smallest = costOfNode[0];
		secondSmallest = costOfNode[1];
	} else {
		smallest = costOfNode[1];
		secondSmallest = costOfNode[0];
	}

	for (unsigned ii = 2; ii < m_dimension; ii++) {
		if (costOfNode[ii] < smallest) {
			secondSmallest = smallest;
			smallest = costOfNode[ii];
		} else if (costOfNode[ii] < secondSmallest) {
			secondSmallest = costOfNode[ii];
		}
	}
}

//ostream& operator<<(ostream& os, const TreeNode& p) {
//
//	os << "{ ";
//	for (const Edge& e : p.include_) { os << "(" << e.u << " " << e.v << ") "; }
//	os << " } ";
//	return os;
//}
