#include "TSPInstance.hpp"

#include <iostream>

TSPInstance::TSPInstance(
  const std::string& fileName
) : m_coordinates(),
  m_name(),
  m_optimalCost(0)
{
  std::ifstream instanceFile(fileName);
  size_t dimension = 0;

  bool headerSection = true;
  do {
    std::string lineStr;
    headerSection = std::getline(instanceFile, lineStr);
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
        keyValuePair >> dimension;
      }
      else if (keyword == "OPTIMAL_COST") {
        keyValuePair >> m_optimalCost;
      }
    }
  } while (headerSection);

  m_coordinates.resize(dimension);

  for (size_t l = 0; l < dimension; ++l) {
    size_t index;
    instanceFile >> index; // do not assume that coordinates will be in a sorted order
    index -= 1; // coordinates are 1-indexed
    instanceFile >> m_coordinates[index][0] >> m_coordinates[index][1];
  }
}

template <>
unsigned int
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
  double deg = std::round(x);
  double min = x - deg;
  return PI * (deg + 5.0 * min / 3.0) / 180.0;
}

template <>
unsigned int
TSPInstance::distance<TSPInstance::GEO>(
  const std::array<double, 2>& coord1,
  const std::array<double, 2>& coord2
) const
{
  const double RRR = 6378.388;

  double q1 = std::cos(convert(coord1[1]) - convert(coord2[1]));
  double q2 = std::cos(convert(coord1[0]) - convert(coord2[0]));
  double q3 = std::cos(convert(coord1[0]) + convert(coord2[0]));
  return static_cast<unsigned int> (RRR * std::acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);
}

std::vector<std::vector<unsigned int> >
TSPInstance::distanceMatrix(
) const
{
  std::vector<std::vector<unsigned int> > distances(dimension(), std::vector<unsigned int>(dimension(), 0));
  for (size_t i = 0; i < dimension(); ++i) {
    for (size_t j = 0; j < dimension(); ++j) {
      if (m_edgeWeightType == EUC_2D) {
        distance<EUC_2D>(m_coordinates[i], m_coordinates[j]);
      }
      else if (m_edgeWeightType == GEO) {
        distance<GEO>(m_coordinates[i], m_coordinates[j]);
      }
    }
  }
  return distances;
}
