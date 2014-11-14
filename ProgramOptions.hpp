#ifndef PROGRAMOPTIONS_HPP_
#define PROGRAMOPTIONS_HPP_

#include <sstream>
#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;

/**
 * This is a utility class for parsing command line arguments.
 */
class ProgramOptions {
public:
  ProgramOptions()
    : m_desc("TSP solver options"),
    m_algorithmType(),
    m_instanceFile(),
    m_cutoffTime(0.0)
  {
    po::options_description desc;
    m_desc.add_options()
      ("help,h", "Print this message.")
      ("alg", po::value<std::string>(&m_algorithmType), "Algorithm to be used for solving.")
      ("inst", po::value<std::string>(&m_instanceFile), "TSP instance file.")
      ("time", po::value<double>(&m_cutoffTime), "Cutoff time (in minutes).")
      ;
  }

  void parse(int argc, char** argv)
  {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, m_desc), vm);
    po::notify(vm);    
    
    if ((argc == 1) || (vm.count("help") > 0)) {
      std::stringstream ss;
      ss << m_desc;
      throw po::error(ss.str());
    }
    else if ((vm.count("alg") != 1) || (vm.count("inst") != 1) || (vm.count("time") > 1)) {
      throw po::error("Wrong combination of arguments!");
    }
  }

  std::string algorithmType() const { return m_algorithmType; }

  std::string instanceFile() const { return m_instanceFile; }

  double cutoffTime() const { return m_cutoffTime; }
  
private:
  po::options_description m_desc;

  std::string m_algorithmType;
  std::string m_instanceFile;

  double m_cutoffTime;
}; // class ProgramOptions

#endif // PROGRAMOPTIONS_HPP_
