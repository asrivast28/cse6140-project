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

class Timer;

class BranchAndBound {

public:
	BranchAndBound(const unsigned dimension,
			const std::vector<std::vector<unsigned>>& distanceMatrix,
			const unsigned cutoffTime,
			std::ofstream* trcFile,
			const Timer* timer,
			const unsigned initialCost,
			const std::vector<unsigned>& initialTour);

	unsigned
	solve(std::vector<unsigned>& tour);

private:
	unsigned m_dimension;

	double m_bestCost;
	TreeNode* m_bestNode;

	unsigned m_numGeneratedNodes;
	unsigned m_numPrunedNodes;

	unsigned m_cutoffTime;

	std::ofstream* m_trcFile;

	const Timer* m_timer;

	std::vector<unsigned> m_initialTour;


	void
	branchAndBound(TreeNode* node, unsigned idxEdge);

};

#endif /* BRANCHANDBOUND_HPP_ */
