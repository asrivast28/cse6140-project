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

  unsigned int
  optimalCost() const { return m_optimalCost; }

  unsigned int
  dimension() const { return m_coordinates.size(); }

  std::vector<std::vector<unsigned int> >
  distanceMatrix() const;

private:
  enum EdgeWeightType {
    GEO,
    EUC_2D
  };

private:
  template <EdgeWeightType>
  unsigned int
  distance(
    const std::array<double, 2>&,
    const std::array<double, 2>&
  ) const;


private:
  std::vector<std::array<double, 2> > m_coordinates;
  std::string m_name;
  unsigned int m_optimalCost;
  EdgeWeightType m_edgeWeightType;
}; // class TSPInstance

#endif // TSPINSTANCE_HPP_
