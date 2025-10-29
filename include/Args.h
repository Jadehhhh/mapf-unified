#pragma once
#include <string>

struct CliArgs {
  std::string planner;         // "RHCR" or "LaCAM"
  std::string input_task_csv;
  std::string output_csv = "results.csv";
  int batch_size = 0;
  int num_agents = 0;
  int seed = 0;
  int verbose = 1;

  std::string rhcr_bin;        // path to lifelong
  std::string lacam_bridge;    // path to lacam_bridge

  std::string map_file;        
  std::string scen_file;
};

inline CliArgs parseArgs(int argc, char** argv) {
  CliArgs a;
  for (int i = 1; i < argc; ++i) {
    std::string k = argv[i];
    auto need = [&](int n=1){ return i+n<argc; };
    auto next = [&]{ return std::string(argv[++i]); };

    if (k == "--planner" && need()) a.planner = next();
    else if (k == "--input_task_csv" && need()) a.input_task_csv = next();
    else if (k == "--output_csv" && need()) a.output_csv = next();
    else if (k == "--batch_size" && need()) a.batch_size = std::stoi(next());
    else if (k == "--num_agents" && need()) a.num_agents = std::stoi(next());
    else if (k == "--seed" && need()) a.seed = std::stoi(next());
    else if (k == "--verbose" && need()) a.verbose = std::stoi(next());
    else if (k == "--rhcr-bin" && need()) a.rhcr_bin = next();
    else if (k == "--lacam-bridge" && need()) a.lacam_bridge = next();
    else if (k == "--map" && need()) a.map_file = next();
    else if (k == "--scen" && need()) a.scen_file = next();
  }
  return a;
}
