/*
 * TreeNode.hpp
 *
 *  Created on: Nov 16, 2014
 *      Author: ykim
 */

#ifndef TREENODE_HPP_
#define TREENODE_HPP_

class TreeNode {
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
	setConstraint(char** constraint);

	char**
	getConstraint();

	int
	addEdge(Edge& addEdge, int idxEdge);

	void
	expand();


};


#endif /* TREENODE_HPP_ */
