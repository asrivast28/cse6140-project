/*
 * TreeNode.hpp
 *
 *  Created on: Nov 16, 2014
 *      Author: ykim
 */

#ifndef TREENODE_HPP_
#define TREENODE_HPP_

#include "Edge.hpp"

class TreeNode {
public:
	static std::vector<Edge> s_listEdges;

public:
	TreeNode(int size);

	void
	calcLowerBound();

	bool
	isSolution();

	void
	recordSolution();

	double
	getCost();

	double
	getLowerBound();

	void
	setConstraint(std::vector<std::vector<char>> constraint);

	std::vector<std::vector<char>>
	getConstraint();

	int
	addEdge(int idxEdge);

	void
	expand();

private:
	unsigned
	m_dimension;

	std::vector<std::vector<char>>
	m_constraint;

	void
	setIndConstraint(unsigned city1, unsigned city2, char val);

};


#endif /* TREENODE_HPP_ */
