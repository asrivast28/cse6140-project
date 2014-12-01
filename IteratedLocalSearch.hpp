/*
 * IteratedLocalSearch.hpp
 *
 *  Created on: Nov 30, 2014
 *      Author: Parul
 */

#ifndef ITERATEDLOCALSEARCH_HPP_
#define ITERATEDLOCALSEARCH_HPP_


#include <vector>
#include "Timer.hpp"

class IteratedLocalSearch {
	public:
	IteratedLocalSearch(
	    const unsigned,
	    std::vector<std::vector<unsigned> >&&,
	    const unsigned cutoffTime,
	    const Timer* timer,
	    std::ofstream* trcFile,
	    unsigned seed
	  );

	unsigned
	  getTour(
	    std::vector<unsigned>&
	  ) ;

	private:
	void threeOptExchange(
	  		  std::vector<unsigned>& tour
	  		  );

	void
	doubleBridgePerturbation(
			  std::vector<unsigned>& tour
			  );

	unsigned
	getIndex(
			std::vector<unsigned>& tour,
			unsigned value
			);

	void localSearch( std::vector<unsigned>& tour
	  );

	void displayVector(
			std::vector<unsigned> v);

	void displayDistMtx(
			const unsigned dimension, std::vector<std::vector<unsigned> > distanceMatrix
			);

	unsigned tourCost(
			std::vector<unsigned> tour
			);

	private:
	  static const unsigned PLUS_INFINITY;
	  unsigned MAX_EXCHANGE_ITERATION;
	  unsigned MAX_PERTURB_ITERATION;
	  unsigned MAX_FUITLESS_MOVES;

	  std::vector<std::vector<unsigned> > m_distanceMatrix;
	  unsigned m_dimension;
	  const Timer* m_timer;
	  unsigned m_cutoffTime;
	  std::ofstream* m_trcFile;
	  unsigned m_seed;

	  double m_best_cost;
	  std::vector<unsigned> m_best_so_far_tour;

	  //GreedyHeuristic m_greedy_obj;
	};


#endif /* ITERATEDLOCALSEARCH_HPP_ */
