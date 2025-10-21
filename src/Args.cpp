#include "Args.h"
#include <vector>
#include <string>
#include <iostream>

static std::string nextArg(const std::vector<std::string>& a, size_t& i){
  return (i+1<a.size()? a[++i] : "");
}

bool ParseRHCR(int argc, char** argv, RHCRArgs& o){
  std::vector<std::string> as(argv+1, argv+argc);
  for(size_t i=0;i<as.size();++i){
    const auto& A = as[i];
    if(A=="--map") o.map_path = nextArg(as,i);
    else if(A=="--task") o.task_csv = nextArg(as,i);
    else if(A=="--agents") o.agents = std::stoi(nextArg(as,i));
    else if(A=="--planning_window") o.planning_window = std::stoi(nextArg(as,i));
    else if(A=="--seed_map") o.seed_map = static_cast<unsigned>(std::stoul(nextArg(as,i)));
    else if(A=="--verbose") o.verbose = std::stoi(nextArg(as,i));
    else if(A=="-h"||A=="--help"){
      std::cout << "lifelong (RHCR)\n"
                   "  --map PATH --task PATH [--agents N]\n"
                   "  --planning_window N [--seed_map S] [--verbose V]\n";
      return false;
    }
  }
  if(o.map_path.empty() || o.task_csv.empty()){
    std::cerr << "RHCR needs --map and --task\n"; return false;
  }
  return true;
}

bool ParseLaCAM(int argc, char** argv, LaCAMArgs& o){
  std::vector<std::string> as(argv+1, argv+argc);
  for(size_t i=0;i<as.size();++i){
    const auto& A = as[i];
    if(A=="--map") o.map_path = nextArg(as,i);
    else if(A=="--task") o.task_csv = nextArg(as,i);
    else if(A=="--agents") o.agents = std::stoi(nextArg(as,i));
    else if(A=="--batch_size") o.batch_size = std::stoi(nextArg(as,i));
    else if(A=="--seed_assign") o.seed_assign = static_cast<unsigned>(std::stoul(nextArg(as,i)));
    else if(A=="--verbose") o.verbose = std::stoi(nextArg(as,i));
    else if(A=="-h"||A=="--help"){
      std::cout << "lacam (LaCAM)\n"
                   "  --map PATH --task PATH [--agents N]\n"
                   "  --batch_size N [--seed_assign S] [--verbose V]\n";
      return false;
    }
  }
  if(o.map_path.empty() || o.task_csv.empty()){
    std::cerr << "LaCAM needs --map and --task\n"; return false;
  }
  return true;
}
