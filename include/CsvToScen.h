#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <filesystem>

//get width and height of map
inline std::pair<int,int> parse_map_size(const std::string& map_path) {
  std::ifstream fin(map_path);
  if (!fin) throw std::runtime_error("open map failed: " + map_path);
  std::string line; int W=-1, H=-1; bool seen_map=false;
  std::vector<std::string> lines;
  while (std::getline(fin, line)) {
    if (!line.empty() && line.back()=='\r') line.pop_back();
    lines.push_back(line);
    std::string t=line; std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    if (t.rfind("width",0)==0)  { std::stringstream ss(line.substr(5)); ss>>W; }
    if (t.rfind("height",0)==0) { std::stringstream ss(line.substr(6)); ss>>H; }
    if (t=="map") seen_map=true;
  }
  if ((W<0 || H<0) && seen_map) {
    auto it = std::find(lines.begin(), lines.end(), "map");
    if (it!=lines.end()) {
      std::vector<std::string> grid(it+1, lines.end());
      grid.erase(std::remove_if(grid.begin(), grid.end(),
        [](const std::string& s){return s.empty();}), grid.end());
      if (!grid.empty()) { H = (int)grid.size(); W = (int)grid[0].size(); }
    }
  }
  if (W<=0 || H<=0) throw std::runtime_error("cannot parse map size: " + map_path);
  return {W,H};
}

inline std::pair<int,int> idx_to_xy(long long idx, int W) {
  if (idx < 0) throw std::runtime_error("negative index");
  return {(int)(idx % W), (int)(idx / W)};
}

// csv to scen
inline size_t csv_to_scen(const std::string& map_path,
                          const std::string& csv_path,
                          const std::string& out_scen,
                          int num_agents_limit) {
  auto [W,H] = parse_map_size(map_path);
  std::ifstream fin(csv_path);
  if (!fin) throw std::runtime_error("open csv failed: " + csv_path);

  std::string header;
  if (!std::getline(fin, header)) throw std::runtime_error("empty csv: " + csv_path);
  if (!header.empty() && header.back()=='\r') header.pop_back();

  // split header
  std::vector<std::string> cols;
  {
    std::stringstream ss(header); std::string tk;
    while (std::getline(ss, tk, ',')) {
      tk.erase(std::remove_if(tk.begin(), tk.end(), ::isspace), tk.end());
      cols.push_back(tk);
    }
  }
  auto find_col=[&](std::string name)->int{
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    for (int i=0;i<(int)cols.size();++i){
      std::string t=cols[i]; std::transform(t.begin(), t.end(), t.begin(), ::tolower);
      if (t==name) return i;
    }
    return -1;
  };

  int c_start=-1, c_goal=-1;
  int g1 = find_col("goal_1"), g2 = find_col("goal_2");
  if (g1!=-1 && g2!=-1) { c_start=g1; c_goal=g2; }
  else {
    int cur = find_col("current_location");
    if (cur!=-1 && g1!=-1) { c_start=cur; c_goal=g1; }
  }
  if (c_start<0 || c_goal<0)
    throw std::runtime_error("need (goal_1,goal_2) or (current_location,goal_1)");

  std::ofstream fout(out_scen);
  if (!fout) throw std::runtime_error("write scen failed: " + out_scen);
  fout << "version 1\n";

  std::string line; size_t wrote=0; long long id=0;
  while (std::getline(fin, line)) {
    if (!line.empty() && line.back()=='\r') line.pop_back();
    if (line.empty()) continue;
    std::vector<std::string> v; std::stringstream ss(line); std::string tk;
    while (std::getline(ss, tk, ',')) v.push_back(tk);

    auto trim=[](std::string s){ s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end()); return s; };
    if (c_start >= (int)v.size() || c_goal >= (int)v.size()) continue;
    std::string s0=trim(v[c_start]), s1=trim(v[c_goal]);
    if (s0.empty() || s1.empty()) continue;

    long long is = std::stoll(s0), ig = std::stoll(s1);
    auto [sx,sy] = idx_to_xy(is, W);
    auto [gx,gy] = idx_to_xy(ig, W);

    std::string mapname = std::filesystem::path(map_path).filename().string();
    fout << id++ << " " << mapname << " " << W << " " << H
         << " " << sx << " " << sy << " " << gx << " " << gy << " 0\n";
    ++wrote;
    
  }
  return wrote;
}
