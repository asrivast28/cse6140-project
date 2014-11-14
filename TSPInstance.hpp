#ifndef TSPINSTANCE_HPP_
#define TSPINSTANCE_HPP_

#include <array>
#include <fstream>
#include <sstream>
#include <vector>

class TSPInstance {
public:
  enum EdgeWeightType {
    GEO,
    EUC_2D
  };

  TSPInstance(const std::string& fileName)
    : m_coordinates(),
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

  std::string name() const { return m_name; }

  size_t optimalCost() const { return m_optimalCost; }

  size_t dimension() const { return m_coordinates.size(); }

  EdgeWeightType edgeWeightType() const { return m_edgeWeightType; }

private:
  std::vector<std::array<double, 2> > m_coordinates;
  std::string m_name;
  size_t m_optimalCost;
  EdgeWeightType m_edgeWeightType;
}; // class TSPInstance

#endif // TSPINSTANCE_HPP_

