/*
 * BranchAndBound.cpp
 *
 *  Created on: Nov 15, 2014
 *  Author: Parul Awasthy (<pawasthy@gatech.edu>)
 * 			Young Jin Kim (<ykim362@gatech.edu>)
 * 			Ankit Srivastava (<asrivast@gatech.edu>)
 */

#include <limits>
#include <iostream>
#include "BranchAndBound.hpp"
#include "Timer.hpp"


BranchAndBound::BranchAndBound(const unsigned dimension,
		const std::vector<std::vector<unsigned>>& distanceMatrix,
		const unsigned cutoffTime,
		std::ofstream* trcFile,
		const Timer* timer)
		: m_dimension(dimension),
		  m_cutoffTime(cutoffTime),
		  m_trcFile(trcFile),
		  m_timer(timer) {

	// In case of no cutoff time, set default value as 300 seconds
	if (m_cutoffTime == 0) {
		m_cutoffTime = 300;
	}

	this->m_bestCost = std::numeric_limits<unsigned>::max();
	this->m_numGeneratedNodes = 0;
	this->m_numPrunedNodes = 0;
	this->m_bestNode = nullptr;

	// Initialize the edge list
	for (unsigned ii = 1; ii <= m_dimension; ii++) {

		std::vector<unsigned> oneRow(m_dimension);

		for (unsigned jj = ii + 1; jj < m_dimension; jj++) {
			unsigned distance = distanceMatrix[ii][jj];

			Edge newEdge(ii, jj, distance);
			TreeNode::s_listEdges.push_back(newEdge);

			Edge inverseEdge(-ii, -jj, distance);
			TreeNode::s_listEdges.push_back(inverseEdge);
		}

		for (unsigned jj = 1; jj <= m_dimension; jj++) {
			unsigned distance = distanceMatrix[ii][jj];

			// Distances
			oneRow[jj - 1] = distance;
		}

		TreeNode::s_distances.push_back(oneRow);
	}
}

unsigned
BranchAndBound::solve(std::vector<unsigned>& tour) {

	// Create root node
	TreeNode* root = new TreeNode(m_dimension);
	m_numGeneratedNodes++;

	root->calcLowerBound();

	// Call recursive branch and bound routine
	branchAndBound(root, std::numeric_limits<unsigned>::max());

	if (!root->getAlive()) {
		delete root;
	}

	if (m_bestNode != nullptr) {
		tour = m_bestNode->getTour();
		delete m_bestNode;
	}

	std::cout << "Total generated node: " << m_numGeneratedNodes << std::endl;
	std::cout << "Total pruned node: " << m_numPrunedNodes << std::endl;

	return m_bestCost;
}


void
BranchAndBound::branchAndBound(TreeNode* node, unsigned idxEdge) {

	// Check cutoff time
	if (m_timer->elapsed() > m_cutoffTime) {
		return;
	}

	if (idxEdge != std::numeric_limits<unsigned>::max()
			&& idxEdge >= TreeNode::s_listEdges.size()) {
		return;
	}

	// Update upper bound
	if (node->isSolution()) {
		node->recordSolution();


		// Save to the trace file
		(*m_trcFile) << m_timer->elapsed() << ", " << node->getCost() << std::endl;

		if (node->getCost() < m_bestCost) {
			m_bestCost = node->getCost();

			if (m_bestNode != nullptr) {
				delete m_bestNode;
			}
			m_bestNode = node;
			m_bestNode->setAlive(true);
		}
	} else {

		// Branch..
		if (node->getLowerBound() < m_bestCost) {

			// Left child node
			TreeNode* leftChild = new TreeNode(m_dimension);
			m_numGeneratedNodes++;

			leftChild->setConstraint(node->getConstraint());

			if (idxEdge != std::numeric_limits<unsigned>::max() && idxEdge % 2 == 0) {
				idxEdge += 2;
			} else {
				idxEdge++;
			}

			if (idxEdge >= TreeNode::s_listEdges.size()) {
				delete leftChild;
				return;
			}

			unsigned idxLeftEdge = leftChild->addEdge(idxEdge);
			leftChild->expand();
			leftChild->calcLowerBound();

			if (leftChild->getLowerBound() >= m_bestCost) {
				delete leftChild;

				leftChild = nullptr;
				m_numPrunedNodes++;
			}

			// Right child node
			TreeNode* rightChild = new TreeNode(m_dimension);
			m_numGeneratedNodes++;
			rightChild->setConstraint(node->getConstraint());
			if (idxLeftEdge >= TreeNode::s_listEdges.size()) {
				if (leftChild != nullptr) {
					delete leftChild;
				}

				if (rightChild != nullptr) {
					delete rightChild;
				}

				return;
			}

			int idxRightEdge = rightChild->addEdge(idxLeftEdge + 1);
			rightChild->expand();
			rightChild->calcLowerBound();

			if (rightChild->getLowerBound() > m_bestCost) {
				delete rightChild;

				rightChild = nullptr;
				m_numPrunedNodes++;
			}

			// Expand tree
			if (leftChild != nullptr && rightChild == nullptr) {
				branchAndBound(leftChild, idxLeftEdge);
			} else if (rightChild != nullptr && leftChild == nullptr) {
				branchAndBound(rightChild, idxRightEdge);
			} else if (leftChild != nullptr && rightChild != nullptr
					&& leftChild->getLowerBound() <= rightChild->getLowerBound()) {
				if (leftChild->getLowerBound() < m_bestCost) {
					branchAndBound(leftChild, idxLeftEdge);
				} else {
					delete leftChild;
					leftChild = nullptr;
					m_numPrunedNodes++;
				}

				if (rightChild->getLowerBound() < m_bestCost) {
					branchAndBound(rightChild, idxRightEdge);
				}
				else {
					delete rightChild;
					rightChild = nullptr;
					m_numPrunedNodes++;
				}
			} else if (rightChild != nullptr) {
				if (rightChild->getLowerBound() < m_bestCost) {
					branchAndBound(rightChild, idxRightEdge);
				}
				else {
					delete rightChild;
					rightChild = nullptr;
					m_numPrunedNodes++;
				}

				if (leftChild->getLowerBound() < m_bestCost) {
					branchAndBound(leftChild, idxLeftEdge);
				} else {
					delete leftChild;
					leftChild = nullptr;
					m_numPrunedNodes++;
				}
			}


			if (leftChild != nullptr && !leftChild->getAlive()) {
				delete leftChild;
			}

			if (rightChild != nullptr && !rightChild->getAlive()) {
				delete rightChild;
			}
		}

	}

}

