#include "AlgorithmImplementations.hpp"

#include <map>
#include <numeric>
#include <vector>

#include <boost/heap/fibonacci_heap.hpp>

namespace bh = boost::heap;

const unsigned PLUS_INFINITY = std::numeric_limits<unsigned>::max();

struct node {

  node(unsigned i)
    : id(i), key(PLUS_INFINITY), parent(0)
  { }

  unsigned id;
  unsigned key;
  unsigned parent;
};

struct compare_node {
  bool operator()(const node& n1, const node& n2) const
  {
    return (n1.key > n2.key);
  }
};

static std::vector<unsigned>
mstPrim(
  const unsigned dimension,
  const unsigned root,
  const std::vector<std::vector<unsigned> >& distanceMatrix
)
{
  bh::fibonacci_heap<node, bh::compare<compare_node> > heap;
  typedef bh::fibonacci_heap<node, bh::compare<compare_node> >::handle_type node_handle;  
  std::map<unsigned, node_handle> heap_elements;
  for (unsigned i = 0; i < dimension; ++i) {
    heap_elements.insert(std::make_pair(i+1, heap.push(node(i+1))));
  }

  std::vector<unsigned> mst;

  (*heap_elements[root]).key = 0;

  while (!heap_elements.empty()) {
    const node& u = heap.top();
    for (std::map<unsigned, node_handle>::value_type& v : heap_elements) {
      if (distanceMatrix[v.first][u.id] < (*v.second).key) {
        (*v.second).key = distanceMatrix[v.first][u.id];
        (*v.second).parent = u.id;
        heap.increase(v.second);
      }
    }
    mst.push_back(u.id);
    heap.pop();
  }
  return mst;
}

unsigned
approximateTourCost(
  const unsigned dimension,
  const std::vector<std::vector<unsigned> >& distanceMatrix
)
{
  std::vector<unsigned> mst(mstPrim(dimension, 1, distanceMatrix));
  return 0;
}
