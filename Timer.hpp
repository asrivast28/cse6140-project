#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <boost/timer/timer.hpp>

namespace bt = boost::timer;

/*
 * Utility class for keeping track of execution time.
 */
class Timer {
public:
  Timer() : m_timer(), m_cutoff(0) { }

  void
  reset() { m_timer.start(); }

  void
  setCutoff(const double cutoff) { m_cutoff = cutoff * 60 * 1e9; }

  double
  remaining() const { return ((m_cutoff - m_timer.elapsed().wall) / 60.0) * 1e-9; }

  ~Timer() { }

private:
  bt::cpu_timer m_timer;
  bt::nanosecond_type m_cutoff;
}; // class Timer

#endif // TIMER_HPP_
