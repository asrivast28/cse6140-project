/*
 * TreeNode.cpp
 *
 *  Created on: Nov 16, 2014
 *      Author: ykim
 */


std::vector<Edge> TreeNode::s_listEdges;


TreeNode::TreeNode(int size) {

	// Initialize member variables
	m_dimension = size;

	// Initialize vector
	for (int ii = 0; ii < m_dimension; ii++) {
		std::vector<char> destForOneCity(m_dimension);

		for (int jj = 0; jj < m_dimension; jj++) {
			destForOneCity.assign(jj, 0);
		}

		m_constraint.push_back(destForOneCity);
	}
}

void
TreeNode::calcLowerBound() {

}

bool
TreeNode::isSolution() {

}

void
TreeNode::recordSolution() {

}

double
TreeNode::getCost() {

}

double
TreeNode::getLowerBound() {

}

void
TreeNode::setConstraint(std::vector<std::vector<char>> constraint) {
	m_constraint = constraint;
}

std::vector<std::vector<char>>
TreeNode::getConstraint() {
	return m_constraint;
}

int
TreeNode::addEdge(int idxEdge) {

	Edge curEdge;

	while (idxEdge < s_listEdges.size()) {

		curEdge = s_listEdges.at(idxEdge);
		int x = std::abs(curEdge.getAnyNode());
		int y = std::abs(curEdge.getTheOther());

		if (m_constraint[x][y] == 0) {
			break;
		}

		idxEdge++;
	}

	if (idxEdge < s_listEdges.size()) {
		if (curEdge.getAnyNode() < 0) {
			setIndConstraint(std::abs(curEdge.getAnyNode(), std::abs(curEdge.getTheOther()), -1);
		} else {
			setIndConstraint(std::abs(curEdge.getAnyNode(), std::abs(curEdge.getTheOther()), 1);
		}

	}

	return idxEdge;
}

void
TreeNode::expand() {

}

void
TreeNode::setIndConstraint(unsigned city1, unsigned city2, char val) {
	std::vector<char> oneCity1 = m_constraint.at(city1);
	oneCity1.assign(city2, val);

	m_constraint.assign(city1, oneCity1);

	std::vector<char> oneCity2 = m_constraint.at(city2);
	oneCity2.assign(city1, val);

	m_constraint.assign(city2, oneCity2);
}
