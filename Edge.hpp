/*
 * Edge.hpp
 *
 *  Created on: Nov 16, 2014
 *  Author: Parul Awasthy (<pawasthy@gatech.edu>)
 * 			Young Jin Kim (<ykim362@gatech.edu>)
 * 			Ankit Srivastava (<asrivast@gatech.edu>)
 */

#ifndef EDGE_HPP_
#define EDGE_HPP_

class Edge {
public:

	Edge() {
		m_node1 = 0;
		m_node2 = 0;
		m_distance = 0;
	}

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

	Edge& operator=(Edge arg) {
	    this->m_node1 = arg.m_node1;
	    this->m_node2 = arg.m_node2;
	    this->m_distance = arg.m_distance;

	    return *this;
	}

private:
	int m_node1;
	int m_node2;

	unsigned m_distance;


};


#endif /* EDGE_HPP_ */
