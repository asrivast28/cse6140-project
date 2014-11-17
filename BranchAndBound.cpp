/*
 * BranchAndBound.cpp
 *
 *  Created on: Nov 15, 2014
 *      Author: ykim
 */

#include "BranchAndBound.hpp"

BranchAndBound::BranchAndBound(const TSPInstance& tsp) {

	// Initialize member variables
	this->m_dimension = tsp.dimension();
	this->m_bestCost = std::numeric_limits<int>::max();
	this->m_numGeneratedNodes = 0;
	this->m_numPrunedNodes = 0;

	std::vector<std::vector<unsigned>> distances = tsp.distanceMatrix();

	// Initialize the edge list
	m_listEdges = new std::vector<Edge>(m_dimension);
	for (int ii = 1; ii <= m_dimension; ii++) {
		for (int jj = ii + 1; jj < m_dimension; jj++) {
			Edge newEdge(ii, jj, (distances.at(ii)).at(jj));
			m_listEdges.push_back(newEdge);

			Edge inverseEdge(-ii, -jj, (distances.at(ii)).at(jj));
			m_listEdges.push_back(inverseEdge);
		}
	}

}

double
BranchAndBound::solve() {

	// Create root node
	TreeNode root(m_dimension);
	m_numGeneratedNodes++;

	root.calcLowerBound();

	// Call recursive branch and bound routine
	branchAndBound(root, -1);

	return m_bestCost;
}


void
BranchAndBound::branchAndBound(TreeNode& node, int idxEdge) {

	if (idxEdge >= m_listEdges.size()) {
		return;
	}

	// Update upper bound
	if (node.isSolution()) {
		node.recordSolution();

		if (node.getCost() < m_bestCost) {
			m_bestCost = node.getCost();
			m_bestNode = node;

			return;
		}
	}

	// Branch..
	if (node.getLowerBound() < 2*m_bestCost) {

		// Left child node
		TreeNode* leftChild = new TreeNode(m_dimension);
		m_numGeneratedNodes++;

		leftChild->setConstraint(node.getConstraint());

		if (idxEdge != -1 && idxEdge % 2 == 1) {
			idxEdge += 2;
		} else {
			idxEdge++;
		}

		if (idxEdge >= m_listEdges.size()) {
			delete leftChild;
			return;
		}

		Edge leftAddEdge = m_listEdges.at(idxEdge);

		int idxLeftEdge = leftChild->addEdge(leftAddEdge, idxEdge);
		leftChild->expand();
		leftChild->calcLowerBound();

		if (leftChild->getLowerBound() >= 2*m_bestCost) {
			delete leftChild;

			leftChild = nullptr;
			m_numPrunedNodes++;
		}

		// Right child node
		TreeNode* rightChild = new TreeNode(m_dimension);
		m_numGeneratedNodes++;
		rightChild->setConstraint(node.getConstraint());
		if (idxLeftEdge >= m_listEdges.size()) {
			if (leftChild != nullptr) {
				delete leftChild;
			}

			if (rightChild != nullptr) {
				delete rightChild;
			}

			return;
		}

		Edge rightAddEdge = m_listEdges.at(idxLeftEdge + 1);

		int idxRightEdge = rightChild->addEdge(rightAddEdge, idxLeftEdge + 1);
		rightChild->expand();
		rightChild->calcLowerBound();

		if (rightChild->getCost() > 2*m_bestCost) {
			delete rightChild;

			rightChild = nullptr;
			m_numPrunedNodes++;
		}

		// Expand tree
		if (leftChild != nullptr && rightChild == nullptr) {
			branchAndBound(*leftChild, idxLeftEdge);
		} else if (rightChild != nullptr && leftChild == nullptr) {
			branchAndBound(*rightChild, idxRightEdge);
		} else if (leftChild != nullptr && rightChild != nullptr
				&& leftChild->getLowerBound() <= rightChild->getLowerBound()) {
			if (leftChild->getLowerBound() < 2*m_bestCost) {
				branchAndBound(*leftChild, idxLeftEdge);
			} else {
				delete leftChild;
				leftChild = nullptr;
				m_numPrunedNodes++;
			}

			if (rightChild->getLowerBound() < 2*m_bestCost) {
				branchAndBound(*rightChild, idxRightEdge);
			}
			else {
				delete rightChild;
				rightChild = nullptr;
				m_numPrunedNodes++;
			}
		} else if (rightChild != nullptr) {
			if (rightChild->getLowerBound() < 2*m_bestCost) {
				branchAndBound(*rightChild, idxRightEdge);
			}
			else {
				delete rightChild;
				rightChild = nullptr;
				m_numPrunedNodes++;
			}

			if (leftChild->getLowerBound() < 2*m_bestCost) {
				branchAndBound(*leftChild, idxLeftEdge);
			} else {
				delete leftChild;
				leftChild = nullptr;
				m_numPrunedNodes++;
			}
		}


		if (leftChild != nullptr) {
			delete leftChild;
		}

		if (rightChild != nullptr) {
			delete rightChild;
		}
	}

}

