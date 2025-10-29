#pragma once
#include <string>

struct RunOptions {
  std::string input_task_csv;
  std::string output_csv;     // LaCAM 输出 csv（默认 results.csv）
  int         batch_size = 0;
  int         num_agents = 0;
  int         seed       = 320;
  int         verbose    = 1;

  std::string rhcr_bin;       // 默认 ./build/lifelong
  std::string lacam_bridge;   // 默认 ./build/lacam/lacam_bridge
  std::string map_file;       // .map (LaCAM) / .grid (RHCR)
  std::string scen_file;      // LaCAM 用（我们会自动从 CSV 生成临时 .scen）
  std::string run_batches_py; // 默认 extern/LaCAM/smaller_batches/run_batches.py
};

struct Planner {
  virtual ~Planner() = default;
  virtual int plan(const RunOptions& opt) = 0;
};

// 工具：简单加引号（跨平台安全）
inline std::string q(const std::string& s) {
#ifdef _WIN32
  return "\"" + s + "\"";
#else
  return "'" + s + "'";
#endif
}
