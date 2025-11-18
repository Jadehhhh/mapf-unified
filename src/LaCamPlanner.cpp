#include "Planner.h"
#include "CsvToScen.h"
#include "GridToMap.h"   
#include <filesystem>
#include <cstdlib>
#include <iostream>

struct LaCAMPlanner final : Planner {
  int plan(const RunOptions& o) override {
    std::string bridge = o.lacam_bridge.empty()? "./build/lacam/lacam_bridge" : o.lacam_bridge;
    std::string runner = o.run_batches_py.empty()? "extern/LaCAM/smaller_batches/run_batches.py" : o.run_batches_py;
    std::string outcsv = o.output_csv.empty()? "results.csv" : o.output_csv;

    if (!std::filesystem::exists(bridge)) {
      std::cerr << "[LaCAM] lacam_bridge not found: " << bridge << "\n";
      return 1;
    }
    if (!std::filesystem::exists(runner)) {
      std::cerr << "[LaCAM] run_batches.py not found: " << runner << "\n";
      return 1;
    }
    // convert .grid to .map
    std::string map_for_lacam;
    try {
     map_for_lacam = ensure_map_for_lacam(o.map_file);
    } catch (const std::exception& e) {
     std::cerr << "[LaCAM] map/grid prepare failed: " << e.what() << "\n";
      return 1;
    }
    if (!std::filesystem::exists(map_for_lacam)) {
      std::cerr << "[LaCAM] map not found: " << map_for_lacam << "\n";
      return 1;
    }
    if (!std::filesystem::exists(o.input_task_csv)) {
      std::cerr << "[LaCAM] csv not found: " << o.input_task_csv << "\n";
      return 1;
    }

    // convert csv to scen
    std::string scen = o.scen_file;
    if (scen.empty()) {
      scen = (std::filesystem::temp_directory_path() / "unified_tmp.scen").string();
    }
    try {
      size_t n = csv_to_scen(map_for_lacam, o.input_task_csv, scen, o.num_agents);
      if (n==0) { std::cerr << "[LaCAM] empty scen generated\n"; return 1; }
      std::cerr << "[LaCAM] SCEN generated: " << scen << " ("<<n<<" tasks)\n";
    } catch (const std::exception& e) {
      std::cerr << "[LaCAM] CSV->SCEN failed: " << e.what() << "\n";
      return 1;
    }

    // 2) use Python run_batches + bridge
    std::string cmd =
      std::string("PYTHONUNBUFFERED=1 python ")
      + q(runner)
      + " --map " + q(map_for_lacam)
      + " --scen " + q(scen)
      + " --agents " + std::to_string(o.num_agents)
      + " --batch-size " + std::to_string(o.batch_size)
      + " --init-random --seed " + std::to_string(o.seed)
      + " --bridge-exe " + q(bridge)
      + " --csv " + q(outcsv)
      + " --run-id Unified --verbose " + std::to_string(o.verbose)
      + " --prefer-near";

    std::cerr << "[LaCAM] " << cmd << "\n";
    int rc = std::system(cmd.c_str());
    return rc==0 ? 0 : (rc>>8);
  }
};

Planner* makeLaCAM() { return new LaCAMPlanner(); }
