#include "AlgorithmImplementations.hpp"

#include <cassert>
#include <map>
#include <numeric>
#include <vector>

#include <boost/heap/fibonacci_heap.hpp>

namespace bh = boost::heap;

namespace tsp {
namespace approx {

const unsigned PLUS_INFINITY = std::numeric_limits<unsigned>::max();

/**
 * Structure for representing nodes, while constructing MST.
 */
struct node {
  node(unsigned i)
    : id(i), key(PLUS_INFINITY), parent(0)
  { }

  unsigned id;
  unsigned key;
  unsigned parent;
};

/**
 * Structure for comparing nodes in fibonacci heap.
 * The comparison operator is inverted so that we
 * can create min-heap from max-heap implementation.
 */
struct compare_node {
  bool operator()(const node& n1, const node& n2) const
  {
    return (n1.key > n2.key);
  }
};

/**
 * @brief Returns a minimum spanning tree of the graph, computed using Prim's algorithm.
 *
 * @param dimension        Dimension of the graph.
 * @param root             Root node for finding MST.
 * @param distanceMatrix   Matrix which stores distance of each node to every other node.
 *
 * @return 1-indexed vector which represents the computed MST. 
 */
static
std::vector<std::vector<unsigned> >
mstPrim(
  const unsigned dimension,
  const unsigned root,
  const std::vector<std::vector<unsigned> >& distanceMatrix
)
{
  bh::fibonacci_heap<node, bh::compare<compare_node> > heap;
  typedef bh::fibonacci_heap<node, bh::compare<compare_node> >::handle_type node_handle;  
  std::map<unsigned, node_handle> heap_elements;
  for (unsigned i = 1; i <= dimension; ++i) {
    heap_elements.insert(std::make_pair(i, heap.push(node(i))));
  }

  std::vector<std::vector<unsigned> > mst(dimension + 1);

  (*heap_elements[root]).key = 0;
  // Increase is called for the element, instead of decrease,
  // because boost's fibonacci heap is a max-heap wherein increase
  // takes O(1) time and decrease takes O(log(N)) time. Therefore,
  // we have overloaded the comparison operator so that a decrease
  // is actually an increase for the heap.
  heap.increase(heap_elements[root]);

  while (!heap_elements.empty()) {
    const node& u = heap.top();
    if (u.parent > 0) {
      mst[u.parent].push_back(u.id);
    }
    for (std::map<unsigned, node_handle>::value_type& v : heap_elements) {
      if (distanceMatrix[v.first][u.id] < (*v.second).key) {
        (*v.second).key = distanceMatrix[v.first][u.id];
        (*v.second).parent = u.id;
        heap.increase(v.second);
      }
    }
    heap_elements.erase(u.id);
    heap.pop();
  }
  return mst;
}

/**
 * @brief Orders tree nodes in the order they will be visited in a preorder walk.
 *
 * @param tree   Tree on which the walk is to be performed.
 * @param root   Root node of the preorder walk.
 * @param walk   The structure which stores elements in walk order.
 */
static
void
preorderWalk(
  const std::vector<std::vector<unsigned> >& tree,
  const unsigned root,
  std::vector<unsigned>& walk
)
{
  walk.push_back(root);
  const std::vector<unsigned>& children = tree[root];
  for (unsigned child : children) {
    preorderWalk(tree, child, walk);
  }
}

/**
 * @brief Computes approximate tour cost for the given TSP.
 *
 * @param dimension        Dimension of the TSP instance.
 * @param distanceMatrix   Distance matrix for the TSP instance.
 * @param tour             The tour computed by the algorithm.
 *
 * @return Calculated approximate cost of the tour. 
 */
unsigned
tour(
  const unsigned dimension,
  const std::vector<std::vector<unsigned> >& distanceMatrix,
  std::vector<unsigned>& tour
)
{
  const unsigned root = 1;
  std::vector<std::vector<unsigned> > mst(mstPrim(dimension, root, distanceMatrix));
  preorderWalk(mst, root, tour);

  unsigned approximateCost = 0;
  for (unsigned i = 0; i < (dimension - 1); ++i) {
    approximateCost += distanceMatrix[tour[i]][tour[i+1]];
  }
  approximateCost += distanceMatrix[tour[0]][tour[dimension-1]];
  return approximateCost;
}

} // namespace approx
} // namespace tsp
