#include "Planner.h"
#include <filesystem>
#include <cstdlib>
#include <iostream>

struct RHCRPlanner final : Planner {
  int plan(const RunOptions& o) override {
    std::string bin = o.rhcr_bin.empty()? "./build/lifelong" : o.rhcr_bin;
    if (!std::filesystem::exists(bin)) {
      std::cerr << "[RHCR] lifelong not found: " << bin << "\n";
      return 1;
    }
    if (!std::filesystem::exists(o.map_file)) {
      std::cerr << "[RHCR] map not found: " << o.map_file << "\n";
      return 1;
    }
    if (!std::filesystem::exists(o.input_task_csv)) {
      std::cerr << "[RHCR] csv not found: " << o.input_task_csv << "\n";
      return 1;
    }

    std::string cmd =
      q(bin) + " --scenario=ONLINE"
      + " -m " + q(o.map_file)
      + " -k " + std::to_string(o.num_agents)
      + " --solver=PBS --single_agent_solver SIPP"
      + " --simulation_window=1"
      + " --planning_window=" + std::to_string(o.batch_size)
      + " --seed=" + std::to_string(o.seed)
      + " --task " + q(o.input_task_csv);

    std::cerr << "[RHCR] " << cmd << "\n";
    int rc = std::system(cmd.c_str());
    return rc==0 ? 0 : (rc>>8);
  }
};

Planner* makeRHCR() { return new RHCRPlanner(); }
