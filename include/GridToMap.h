#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include <algorithm>

inline std::vector<std::string> split_csv_line(const std::string& s) {
  std::vector<std::string> out;
  std::string cur; 
  std::istringstream ss(s);
  while (std::getline(ss, cur, ',')) out.push_back(cur);
  return out;
}

// 输入 .grid（CSV），生成 MovingAI .map，返回 .map 路径；若已是 .map，原样返回
inline std::string ensure_map_for_lacam(const std::string& map_or_grid) {
  namespace fs = std::filesystem;
  fs::path p(map_or_grid);
  auto ext = p.extension().string();

  // 已经是 .map，直接用
  std::string lower_ext = ext;
  std::transform(lower_ext.begin(), lower_ext.end(), lower_ext.begin(), ::tolower);
  if (lower_ext == ".map") return map_or_grid;

  // 不是 .grid 就报错，避免误用
  if (lower_ext != ".grid")
    throw std::runtime_error("Unsupported map extension (need .map or .grid): " + map_or_grid);

  // 读取 .grid CSV
  std::ifstream fin(map_or_grid);
  if (!fin) throw std::runtime_error("open grid failed: " + map_or_grid);

  std::vector<std::vector<std::string>> rows;
  std::string line;
  while (std::getline(fin, line)) {
    if (!line.empty() && line.back()=='\r') line.pop_back();
    rows.push_back(split_csv_line(line));
  }
  if (rows.size() < 2) throw std::runtime_error("grid too short: " + map_or_grid);

  // 第二行是 H,W（可能是单元格 "H,W"，也可能两个单元格）
  int H=-1, W=-1;
  {
    const auto& r = rows[1];
    if (r.size() == 1) {
      std::istringstream ss(r[0]);
      char comma;
      if (!(ss >> H)) throw std::runtime_error("bad H in grid header");
      if (!(ss >> comma) || comma != ',') throw std::runtime_error("bad comma in grid header");
      if (!(ss >> W)) throw std::runtime_error("bad W in grid header");
    } else {
      if (r.size() < 2) throw std::runtime_error("bad grid header row 2");
      H = std::stoi(r[0]); W = std::stoi(r[1]);
    }
  }
  if (H <= 0 || W <= 0) throw std::runtime_error("invalid H/W in grid header");

  // 初始化全 FREE（与 Python 脚本一致）
  std::vector<std::string> grid(H, std::string(W, '.'));

  // 从第 4 行开始（索引 3）读取：(_, cell_type, x, y, ...)
  for (size_t i = 3; i < rows.size(); ++i) {
    const auto& r = rows[i];
    if (r.size() < 4) continue;
    std::string cell_type = r[1];
    int x = std::stoi(r[2]);
    int y = std::stoi(r[3]);
    if (x < 0 || x >= W || y < 0 || y >= H) continue; // 安全越界保护

    // cell_type == "Obstacle" -> '@'，否则 '.'
    if (cell_type == "Obstacle") grid[y][x] = '@';
    else                         grid[y][x] = '.';
  }

  // 写出到临时 .map（放到系统临时目录）
  fs::path out_path = fs::temp_directory_path() / (p.stem().string() + ".map");
  std::ofstream fout(out_path);
  if (!fout) throw std::runtime_error("write map failed: " + out_path.string());

  fout << "type octile\n";
  fout << "height " << H << "\n";
  fout << "width "  << W << "\n";
  fout << "map\n";
  for (int r = 0; r < H; ++r) {
    fout << grid[r] << "\n";
  }
  fout.close();
  return out_path.string();
}
