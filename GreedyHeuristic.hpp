/*
 * GreedyHeuristic.hpp
 *
 *  Created on: Nov 16, 2014
 *      Author: Parul
 */

#ifndef GREEDYHEURISTIC_HPP_
#define GREEDYHEURISTIC_HPP_

#include <vector>

class GreedyHeuristic {
	public:
	GreedyHeuristic(
	    const unsigned,
	    std::vector<std::vector<unsigned> >&&
	  );

	unsigned
	  getTour(
	    std::vector<unsigned>&
	  ) const;

	private:
	void farthestInsertion(
	  		  std::vector<unsigned>& tour
	  		  )const;

	void displayVector(
			std::vector<unsigned> v)const;

	void displayDistMtx(
			const unsigned dimension, std::vector<std::vector<unsigned> > distanceMatrix
			)const;

	private:
	  static const unsigned PLUS_INFINITY;

	  const std::vector<std::vector<unsigned> > m_distanceMatrix;
	  const unsigned m_dimension;
	};


#endif /* GREEDYHEURISTIC_HPP_ */
