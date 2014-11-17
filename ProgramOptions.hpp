#ifndef PROGRAMOPTIONS_HPP_
#define PROGRAMOPTIONS_HPP_

#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

/**
 * Utility class for parsing command line arguments.
 */
class ProgramOptions {
public:
  ProgramOptions();

  void
  parse(int, char**);

  std::string
  algorithm() const { return m_algorithm; }

  std::string
  instanceFile() const { return m_instanceFile; }

  double
  cutoffTime() const { return m_cutoffTime; }

  ~ProgramOptions() { }
  
private:
  po::options_description m_desc;
  std::string m_algorithm;
  std::string m_instanceFile;
  double m_cutoffTime;
}; // class ProgramOptions

#endif // PROGRAMOPTIONS_HPP_
