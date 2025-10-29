#pragma once
#include <string>

struct RunOptions {
  std::string input_task_csv;
  std::string output_csv;     // LaCAM output csv
  int         batch_size = 0;
  int         num_agents = 0;
  int         seed       = 320;
  int         verbose    = 1;

  std::string rhcr_bin;       //  ./build/lifelong
  std::string lacam_bridge;   // ./build/lacam/lacam_bridge
  std::string map_file;       // .map (LaCAM) / .grid (RHCR)
  std::string scen_file;      // for LaCAM （from CSV to temporary .scen）
  std::string run_batches_py; // extern/LaCAM/smaller_batches/run_batches.py
};

struct Planner {
  virtual ~Planner() = default;
  virtual int plan(const RunOptions& opt) = 0;
};

inline std::string q(const std::string& s) {
#ifdef _WIN32
  return "\"" + s + "\"";
#else
  return "'" + s + "'";
#endif
}
