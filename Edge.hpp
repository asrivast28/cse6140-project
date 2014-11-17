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
	Edge(int node1, int node2, unsigned distance) {
		m_node1 = node1;
		m_node2 = node2;
		m_distance = distance;
	}

	int getAnyNode() {
		return m_node1;
	}

	int getTheOther() {
		return m_node2;
	}

	int getTheOther(int thisNode) {
		if (m_node1 == thisNode)
			return m_node2;
		else
			return m_node1;
	}

	unsigned getDistance() {
		return m_distance;
	}

private:
	int m_node1;
	int m_node2;

	unsigned m_distance;


};


#endif /* EDGE_HPP_ */
