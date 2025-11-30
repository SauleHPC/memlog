#ifndef PERF_LOG_H
#define PERF_LOG_H

#include <chrono>
#include <numeric>
#include <vector>

template <typename ClockType = std::chrono::system_clock::duration >
class perf_log {
  ClockType average;
  size_t nb_aggregation;
  std::vector<ClockType> individual_recent_data;
  int every;
  bool log;
  std::string name;
  
public:
  perf_log(std::string name = "unknown")
    :average(0), nb_aggregation(0), individual_recent_data(), every(10), log(true), name(name) {
    individual_recent_data.reserve(every);
  }
  
  void addDataPoint(const ClockType& ct) {
    individual_recent_data.push_back(ct);
    if (individual_recent_data.size() == every) {
      auto sum_me = std::accumulate(individual_recent_data.begin(), individual_recent_data.end(), ClockType(0));
      auto recent_average = sum_me/every;
      
      individual_recent_data.clear();
      average = ((average*nb_aggregation ) + recent_average)/(nb_aggregation+1);
      nb_aggregation++;
      
      if (log) {
	using seconds_double = std::chrono::duration<double>;
	std::cerr<<name<<" "<<duration_cast<seconds_double>(recent_average)<<" "<<duration_cast<seconds_double>(average)<<"\n";
      }
    }
  }
};

#endif
