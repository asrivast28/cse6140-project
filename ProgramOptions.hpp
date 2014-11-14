#ifndef PROGRAMOPTIONS_HPP_
#define PROGRAMOPTIONS_HPP_

#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

/**
 * This is a utility class for parsing command line arguments.
 */
class ProgramOptions {
public:
  ProgramOptions();

  void
  parse(int, char**);

  std::string
  algorithmType() const { return m_algorithmType; }

  std::string
  instanceFile() const { return m_instanceFile; }

  double
  cutoffTime() const { return m_cutoffTime; }
  
private:
  po::options_description m_desc;
  std::string m_algorithmType;
  std::string m_instanceFile;
  double m_cutoffTime;
}; // class ProgramOptions

#endif // PROGRAMOPTIONS_HPP_
