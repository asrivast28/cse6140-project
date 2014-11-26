#include "TSPInstance.hpp"

#include <iostream>

TSPInstance::TSPInstance(
  const std::string& fileName
) : m_coordinates(),
  m_name(),
  m_dimension(0),
  m_optimalCost(0)
{
  std::ifstream instanceFile(fileName);

  bool headerSection = true;
  do {
    std::string lineStr;
    headerSection = (bool)std::getline(instanceFile, lineStr);
    if (lineStr == "NODE_COORD_SECTION") {
      headerSection = false;
    }
    else {
      std::istringstream keyValuePair(lineStr);
      std::string keyword;
      std::getline(keyValuePair, keyword, ':');

      if (keyword == "NAME") {
        keyValuePair >> m_name;
      }
      else if (keyword == "EDGE_WEIGHT_TYPE") {
        std::string value;
        keyValuePair >> value;
        if (value == "GEO") {
          m_edgeWeightType = GEO;
        }
        else if (value == "EUC_2D") {
          m_edgeWeightType = EUC_2D;
        }
      }
      else if (keyword == "DIMENSION") {
        keyValuePair >> m_dimension;
      }
      else if (keyword == "OPTIMAL_COST") {
        keyValuePair >> m_optimalCost;
      }
    }
  } while (headerSection);

  m_coordinates.resize(m_dimension + 1);

  for (size_t l = 0; l < m_dimension; ++l) {
    size_t index;
    instanceFile >> index; // do not assume that coordinates will be in a sorted order
    instanceFile >> m_coordinates[index][0] >> m_coordinates[index][1];
  }
}

/**
 * @brief Calculated distance between two EUC_2D coordinates.
 *
 * @param coord1   First EUC_2D coordinate.
 * @param coord2   Second EUC_2D coordinate.
 *
 * @return The calculated distance. 
 */
template <>
unsigned
TSPInstance::distance<TSPInstance::EUC_2D>(
  const std::array<double, 2>& coord1,
  const std::array<double, 2>& coord2
) const
{
  double xd = coord2[0] - coord1[0];
  double yd = coord2[1] - coord1[1];
  return std::round(std::sqrt(xd*xd + yd*yd));
}

static
double
convert(
  const double x
)
{
  const double PI = 3.141592;
  double deg = static_cast<int>(x);
  double min = x - deg;
  return PI * (deg + 5.0 * min / 3.0) / 180.0;
}

/**
 * @brief Calculates distance, in kilometers, between two GEO coordinates.
 *
 * @param coord1   First GEO coordinate.
 * @param coord2   Second GEO coordinate.
 *
 * @return The calculated distance. 
 */
template <>
unsigned
TSPInstance::distance<TSPInstance::GEO>(
  const std::array<double, 2>& coord1,
  const std::array<double, 2>& coord2
) const
{
  const double RRR = 6378.388;

  double q1 = std::cos(convert(coord1[1]) - convert(coord2[1]));
  double q2 = std::cos(convert(coord1[0]) - convert(coord2[0]));
  double q3 = std::cos(convert(coord1[0]) + convert(coord2[0]));
  return static_cast<unsigned> (RRR * std::acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);
}

/**
 * Returns a 1-indexed distance matrix for this TSP instance.
 */
std::vector<std::vector<unsigned> >
TSPInstance::distanceMatrix(
) const
{
  std::vector<std::vector<unsigned> > distances(m_dimension + 1, std::vector<unsigned>(m_dimension + 1, 0));
  for (unsigned i = 1; i <= m_dimension; ++i) {
    const std::array<double, 2>& coord1 = m_coordinates[i];
    for (unsigned j = 1; j <= m_dimension; ++j) {
      const std::array<double, 2>& coord2 = m_coordinates[j];
      if (m_edgeWeightType == EUC_2D) {
        distances[i][j] = distance<EUC_2D>(coord1, coord2);
      }
      else if (m_edgeWeightType == GEO) {
        distances[i][j] = distance<GEO>(coord1, coord2);
      }
    }
  }
  return distances;
}
