#pragma once
#include <vector>
#include <string>

struct TaskRow {
  int agent_id;
  int sx, sy;   // start
  int gx, gy;   // goal
  // 其他字段...
};

struct TaskCSV {
  std::vector<TaskRow> rows;
};

// TODO: 实现 read/write，如果需要 RHCR <-> LaCAM CSV 格式互转也放这里
