#include "ProgramOptions.hpp"

#include <sstream>


ProgramOptions::ProgramOptions(
) : m_desc("TSP solver options"),
  m_algorithm(),
  m_instanceFile(),
  m_cutoffTime(0.0)
{
  po::options_description desc;
  m_desc.add_options()
    ("help,h", "Print this message.")
    ("alg", po::value<std::string>(&m_algorithm), "Algorithm to be used for solving.")
    ("inst", po::value<std::string>(&m_instanceFile), "TSP instance file.")
    ("time", po::value<double>(&m_cutoffTime), "Cutoff time (in minutes).")
    ;
}

void
ProgramOptions::parse(
  int argc,
  char** argv
)
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
