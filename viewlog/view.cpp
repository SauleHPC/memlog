#include <iostream>
#include <vector>
#include <fstream>

struct log_entry {
  void* ptr;
};

struct log {
  std::vector<log_entry> entries;
};

log parse_log(std::string filename) {
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

int main () {
  log l = parse_log("../log-1000");


  int count = 0;
  for (auto & le: l.entries) {
    std::cout<<le.ptr<<"\n";
    count++;
    if (count > 10)
      break;
  }

  return 0;
}
