#include "Args.h"
#include "Planner.h"
#include <iostream>
#include <memory>

Planner* makeRHCR();
Planner* makeLaCAM();

int main(int argc, char** argv) {
  auto a = parseArgs(argc, argv);

  RunOptions opt;
  opt.input_task_csv = a.input_task_csv;
  opt.output_csv     = a.output_csv;
  opt.batch_size     = a.batch_size;
  opt.num_agents     = a.num_agents;
  opt.seed           = a.seed;
  opt.verbose        = a.verbose;
  opt.rhcr_bin       = a.rhcr_bin;
  opt.lacam_bridge   = a.lacam_bridge;
  opt.map_file       = a.map_file;
  opt.scen_file      = a.scen_file;

  if (a.planner != "RHCR" && a.planner != "LaCAM") {
    std::cerr << "Usage: --planner {RHCR|LaCAM} [--input_task_csv ...] "
                 "[--rhcr-bin PATH] [--lacam-bridge PATH] "
                 "[--map ... --scen ...]\n";
    return 2;
  }

  std::unique_ptr<Planner> p(
    (a.planner=="RHCR") ? makeRHCR() : makeLaCAM()
  );
  int code = p->plan(opt);
  if (code != 0)
    std::cerr << "Planner process exited with code " << code << "\n";
  return code;
}
