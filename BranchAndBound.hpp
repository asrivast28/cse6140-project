/*
 * BranchAndBound.hpp
 *
 *  Created on: Nov 15, 2014
 *      Author: ykim
 */

#ifndef BRANCHANDBOUND_HPP_
#define BRANCHANDBOUND_HPP_

class BranchAndBound {
public:
	BranchAndBound(const TSPInstance& data);

	double
	solve();

private:
	int m_dimension;

	double m_bestCost;
	TreeNode m_bestNode;

	int m_numGeneratedNodes;
	int m_numPrunedNodes;

	std::vector<Edge> m_listEdges;

	void
	branchAndBound(TreeNode& node, int idxEdge);

};

#endif /* BRANCHANDBOUND_HPP_ */
