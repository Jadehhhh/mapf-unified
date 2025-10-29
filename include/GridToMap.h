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

// input .grid，generate MovingAI .map，return .map path
inline std::string ensure_map_for_lacam(const std::string& map_or_grid) {
  namespace fs = std::filesystem;
  fs::path p(map_or_grid);
  auto ext = p.extension().string();

  // if it's already .map
  std::string lower_ext = ext;
  std::transform(lower_ext.begin(), lower_ext.end(), lower_ext.begin(), ::tolower);
  if (lower_ext == ".map") return map_or_grid;

 // if it's not .grid
  if (lower_ext != ".grid")
    throw std::runtime_error("Unsupported map extension (need .map or .grid): " + map_or_grid);

  // read .grid CSV
  std::ifstream fin(map_or_grid);
  if (!fin) throw std::runtime_error("open grid failed: " + map_or_grid);

  std::vector<std::vector<std::string>> rows;
  std::string line;
  while (std::getline(fin, line)) {
    if (!line.empty() && line.back()=='\r') line.pop_back();
    rows.push_back(split_csv_line(line));
  }
  if (rows.size() < 2) throw std::runtime_error("grid too short: " + map_or_grid);

  // the second row can be H,W
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

  //initialize
  std::vector<std::string> grid(H, std::string(W, '.'));

  // read from the fourth row：(_, cell_type, x, y, ...)
  for (size_t i = 3; i < rows.size(); ++i) {
    const auto& r = rows[i];
    if (r.size() < 4) continue;
    std::string cell_type = r[1];
    int x = std::stoi(r[2]);
    int y = std::stoi(r[3]);
    if (x < 0 || x >= W || y < 0 || y >= H) continue; 

    // cell_type == "Obstacle" -> '@'，else '.'
    if (cell_type == "Obstacle") grid[y][x] = '@';
    else                         grid[y][x] = '.';
  }

  // temporary .map
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
