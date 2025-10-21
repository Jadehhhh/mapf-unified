#include "LaCamPlanner.h"
#include <random>
#include <iostream>

bool LaCamPlanner::run(const LaCAMArgs& a){
  std::mt19937 rng(a.seed_assign);
  if(a.verbose>=2) {
    std::cout << "[LaCAM] map="<<a.map_path
              << " task="<<a.task_csv
              << " agents="<<a.agents
              << " batch_size="<<a.batch_size
              << " seed_assign="<<a.seed_assign << "\n";
  }
  // TODO: 这里以后接入 LaCAM 的真实批处理/求解
  return true; // 先返回成功
}
