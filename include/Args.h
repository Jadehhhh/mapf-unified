#pragma once
#include <string>

struct CommonArgs {
  std::string map_path;
  std::string task_csv;
  int agents = -1;
  int verbose = 1;
};

struct RHCRArgs : public CommonArgs {
  int      planning_window = 10;
  unsigned seed_map = 0;       // RHCR 
};

struct LaCAMArgs : public CommonArgs {
  int      batch_size = 32;
  unsigned seed_assign = 0;    // LaCAM 
};

bool ParseRHCR(int argc, char** argv, RHCRArgs& out);
bool ParseLaCAM(int argc, char** argv, LaCAMArgs& out);
