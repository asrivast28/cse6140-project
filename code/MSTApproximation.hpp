#ifndef MSTAPPROXIMATION_HPP_
#define MSTAPPROXIMATION_HPP_

#include <vector>

class MSTApproximation {
public:
  MSTApproximation(
    std::vector<std::vector<unsigned> >&&,
    const unsigned
  );

  unsigned
  getTour(
    std::vector<unsigned>&
  ) const;

private:
  struct node;

  struct compare_node;

private:
  std::vector<std::vector<unsigned> >
  primMST(
    const unsigned
  ) const; 

  unsigned
  preorderWalk(
    const std::vector<std::vector<unsigned> >&,
    const unsigned,
    std::vector<unsigned>&
  ) const;

private:
  static const unsigned PLUS_INFINITY;

  const std::vector<std::vector<unsigned> > m_distanceMatrix;
  const unsigned m_dimension;
};

#endif // MSTAPPROXIMATION_HPP_
