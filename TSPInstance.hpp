#ifndef TSPINSTANCE_HPP_
#define TSPINSTANCE_HPP_

#include <array>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>


class TSPInstance {
public:
  TSPInstance(const std::string&);

  std::string
  name() const { return m_name; }

  unsigned
  optimalCost() const { return m_optimalCost; }

  unsigned
  dimension() const { return m_dimension; }

  std::vector<std::vector<unsigned> >
  distanceMatrix() const;

  ~TSPInstance() { }

private:
  enum EdgeWeightType {
    GEO,
    EUC_2D
  };

private:
  template <EdgeWeightType>
  unsigned
  distance(
    const std::array<double, 2>&,
    const std::array<double, 2>&
  ) const;


private:
  std::vector<std::array<double, 2> > m_coordinates;
  std::string m_name;
  unsigned m_dimension;
  unsigned m_optimalCost;
  EdgeWeightType m_edgeWeightType;
}; // class TSPInstance

#endif // TSPINSTANCE_HPP_
