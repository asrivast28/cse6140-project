#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <boost/timer/timer.hpp>

namespace bt = boost::timer;

/*
 * Utility class for keeping track of execution time.
 */
class Timer {
public:
  Timer() : m_timer(), m_cutoff(0.0) { }

  void
  start() { m_timer.start(); }

  void
  stop() { m_timer.stop(); }

  double
  elapsed() const { return m_timer.elapsed().wall * 1e-9; }

  void
  setCutoff(const double cutoff) { m_cutoff = cutoff; }

  double
  remaining() const { return m_cutoff - elapsed(); }

  ~Timer() { }

private:
  bt::cpu_timer m_timer;
  double m_cutoff;
}; // class Timer

#endif // TIMER_HPP_
