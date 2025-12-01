#ifndef LOG_FILE
#define LOG_FILE

#include <iostream>
#include <vector>
#include <fstream>

namespace loglibrary {

struct log_entry {
  void* ptr;
};

struct log {
  std::vector<log_entry> entries;
  void* offset = 0;

  static log parse_log(std::string filename) {
    log l;
    std::ifstream in (filename);
    
    while (in.good()) {
      log_entry le;
      in>>le.ptr;
      if (in.good()) {
	l.entries.push_back(le);
      }
    }
    
    return l;
  }


  void shift_offset () {
    void *p = entries[0].ptr;
    for (const auto& aa : entries) {
      if (aa.ptr < p)
	p = aa.ptr;
    }
    offset += (size_t)p;

    std::cerr<<"Offset: "<<offset<<'\n';
    
    for (auto& a : entries) {
      a.ptr -= (size_t)p;
    }
  }

  void stats() {
    void *min = entries[0].ptr, *max = entries[0].ptr;
    for (const auto& a: entries) {
      if (min > a.ptr)
	min = a.ptr;
      if (max < a.ptr)
	max = a.ptr;
    }
    std::cerr<<"range: "<<min<<" "<<max<<'\n';    
  }
};

}
#endif
