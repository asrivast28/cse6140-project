/*
 * Edge.hpp
 *
 *  Created on: Nov 16, 2014
 *      Author: ykim
 */

#ifndef EDGE_HPP_
#define EDGE_HPP_

class Edge {
public:
	Edge(int node1, int node2, double distance) {
		this->node1 = node1;
		this->node2 = node2;
		this->distance = distance;
	}

	int getAnyNode() {
		return this->node1;
	}

	int getTheOther() {
		return this->node2;
	}

	int getTheOther(int thisNode) {
		if (this->node1 == thisNode)
			return this->node2;
		else
			return this->node1;
	}

	double getDistance() {
		return this->distance;
	}


private:
	final int node1;
	final int node2;

	final double distance;

};


#endif /* EDGE_HPP_ */
