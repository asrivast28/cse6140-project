/*
 * BranchAndBound.hpp
 *
 *  Created on: Nov 15, 2014
 *  Author: Parul Awasthy (<pawasthy@gatech.edu>)
 * 			Young Jin Kim (<ykim362@gatech.edu>)
 * 			Ankit Srivastava (<asrivast@gatech.edu>)
 */

#ifndef BRANCHANDBOUND_HPP_
#define BRANCHANDBOUND_HPP_

#include "TSPInstance.hpp"
#include "TreeNode.hpp"

class BranchAndBound {

public:
	BranchAndBound(const unsigned dimension, const std::vector<std::vector<unsigned>>& distanceMatrix);

	unsigned
	solve(std::vector<unsigned>& tour);

private:
	unsigned m_dimension;

	unsigned m_bestCost;
	TreeNode* m_bestNode;

	unsigned m_numGeneratedNodes;
	unsigned m_numPrunedNodes;



	void
	branchAndBound(TreeNode* node, unsigned idxEdge);

};

#endif /* BRANCHANDBOUND_HPP_ */
