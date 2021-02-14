#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#define FILENAME "D:/Code/projects/Algorithms/src/CFP/data/data30-90.txt"
#define RESULTS "D:/Code/projects/Algorithms/src/CFP/data/data30-90.result"

#define SHIFT 1
#define SHAKING 5000
#define SEARCHING 500

class Factory {
  uint32_t ones_count = 0;
  uint32_t clasters_count = 1;
  std::vector<std::vector<bool>> matrix;
  std::vector<uint32_t> machines, parts;
  std::vector<uint32_t> mclasters, pclasters;

 public:
  Factory() {
    std::ifstream fin(FILENAME);
    if (!fin.is_open()) throw std::runtime_error("File is not opened");

    uint32_t M, P;
    fin >> M >> P;

    matrix.resize(M);
    for (uint32_t i = 0; i < M; i++) {
      matrix[i].resize(P);

      uint32_t machine;
      fin >> machine;

      std::string line;
      std::getline(fin, line);
      std::istringstream is(line);
      auto parts = std::vector<uint32_t>(std::istream_iterator<uint32_t>(is),
                                         std::istream_iterator<uint32_t>());

      for (uint32_t part : parts) {
        matrix[machine - SHIFT][part - SHIFT] = true;
        ones_count++;
      }

      machines.push_back(i);
      mclasters.push_back(1);
    }
    for (uint32_t i = 0; i < P; i++) {
      parts.push_back(i);
      pclasters.push_back(1);
    }

    fin.close();
  }

  void writeResult() {
    std::ifstream fin(RESULTS);
    if (!fin.is_open()) throw std::runtime_error("File is not opened");
    double best_efficacy;
    fin >> best_efficacy;
    fin.close();

    double current_efficacy = getEfficacy();
    if (best_efficacy < current_efficacy) {
      std::ofstream fout(RESULTS);
      fout << current_efficacy << std::endl << std::endl;

      for (uint32_t val : mclasters) fout << val << " ";
      fout << std::endl;
      for (uint32_t val : pclasters) fout << val << " ";
      fout << std::endl << std::endl;

      sortMatrix();
      for (auto mpos : machines) {
        for (auto ppos : parts) fout << matrix[mpos][ppos] << " ";
        fout << std::endl;
      }

      fout.close();
    }
  }

  void sortMatrix() {
    uint32_t pos = 0;
    for (uint32_t claster = 1; claster <= clasters_count; claster++)
      for (uint32_t mpos = 0; mpos < mclasters.size(); mpos++)
        if (mclasters[mpos] == claster) machines[pos++] = mpos;

    pos = 0;
    for (uint32_t claster = 1; claster <= clasters_count; claster++)
      for (uint32_t ppos = 0; ppos < pclasters.size(); ppos++)
        if (pclasters[ppos] == claster) parts[pos++] = ppos;
  }

  void printMatrix() {
    sortMatrix();
    for (auto mpos : machines) {
      for (auto ppos : parts) std::cout << matrix[mpos][ppos] << " ";
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  void printOriginalMatrix() {
    for (auto row : matrix) {
      for (auto el : row) std::cout << el << " ";
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  void printClasters() {
    for (uint32_t val : mclasters) std::cout << val << " ";
    std::cout << std::endl;
    for (uint32_t val : pclasters) std::cout << val << " ";
    std::cout << std::endl << std::endl;
  }

  void divideClaster() {
    if (clasters_count == std::min(machines.size(), parts.size())) return;

    // Choose random claster to divide
    std::vector<uint32_t> clasters;
    for (uint32_t i = 1; i <= clasters_count; i++) {
      uint32_t rows = 0, cols = 0;
      for (uint32_t val : mclasters)
        if (val == i) rows++;
      for (uint32_t val : pclasters)
        if (val == i) cols++;
      if (!(rows > 2 && cols > 2)) continue;
      clasters.push_back(i);
    }
    if (clasters.size() == 0) return;
    uint32_t claster = clasters[rand() % clasters.size()];

    // Choose random element to seperate from claster
    std::vector<uint32_t> machines_pos, parts_pos;
    for (uint32_t pos = 0; pos < mclasters.size(); pos++)
      if (mclasters[pos] == claster) machines_pos.push_back(pos);
    for (uint32_t pos = 0; pos < pclasters.size(); pos++)
      if (pclasters[pos] == claster) parts_pos.push_back(pos);
    uint32_t new_claster_row = machines_pos[rand() % machines_pos.size()];
    uint32_t new_claster_col = parts_pos[rand() % parts_pos.size()];

    // Set new claster
    ++clasters_count;
    mclasters[new_claster_row] = clasters_count;
    pclasters[new_claster_col] = clasters_count;
  }

  void mergeClaster() {
    if (clasters_count == 1) return;

    // Choose random claster
    std::vector<uint32_t> clasters;
    for (uint32_t i = 1; i <= clasters_count; i++) clasters.push_back(i);
    uint32_t claster1 = clasters[rand() % clasters.size()];

    // Choose other random claster
    clasters.clear();
    for (uint32_t i = 1; i <= clasters_count; i++) {
      if (claster1 == i) continue;
      clasters.push_back(i);
    }
    uint32_t claster2 = clasters[rand() % clasters.size()];

    // Unite clasters
    uint32_t master_claster = std::min(claster1, claster2);
    uint32_t adder_claster = std::max(claster1, claster2);

    for (uint32_t pos = 0; pos < mclasters.size(); pos++) {
      if (mclasters[pos] == adder_claster) mclasters[pos] = master_claster;
      if (mclasters[pos] > adder_claster) mclasters[pos] -= 1;
    }

    for (uint32_t pos = 0; pos < pclasters.size(); pos++) {
      if (pclasters[pos] == adder_claster) pclasters[pos] = master_claster;
      if (pclasters[pos] > adder_claster) pclasters[pos] -= 1;
    }

    --clasters_count;
  }

  void reassignRow() {
    if (clasters_count == 1) return;

    uint32_t source = 0;
    for (uint32_t claster = 1; claster <= clasters_count; claster++) {
      uint32_t rows = 0;
      for (uint32_t val : mclasters)
        if (val == claster) rows++;
      if (rows < 2) continue;
      source = claster;
      break;
    }
    if (source == 0) return;

    std::vector<uint32_t> source_poses;
    for (uint32_t pos = 0; pos < mclasters.size(); pos++)
      if (mclasters[pos] == source) source_poses.push_back(pos);

    uint32_t row = source_poses[rand() % source_poses.size()];

    mclasters[row] = 1 + rand() % clasters_count;
  }

  void reassignColumn() {
    if (clasters_count == 1) return;

    uint32_t source = 0;
    for (uint32_t claster = 1; claster <= clasters_count; claster++) {
      uint32_t cols = 0;
      for (uint32_t val : pclasters)
        if (val == claster) cols++;
      if (cols < 2) continue;
      source = claster;
      break;
    }
    if (source == 0) return;

    std::vector<uint32_t> source_poses;
    for (uint32_t pos = 0; pos < pclasters.size(); pos++)
      if (pclasters[pos] == source) source_poses.push_back(pos);

    uint32_t col = source_poses[rand() % source_poses.size()];

    pclasters[col] = 1 + rand() % clasters_count;
  }

  double getEfficacy() {
    uint32_t ones_in = 0, zeros_in = 0;

    for (uint32_t row = 0; row < machines.size(); row++) {
      for (uint32_t col = 0; col < parts.size(); col++) {
        if (mclasters[row] != pclasters[col]) continue;
        if (matrix[row][col])
          ++ones_in;
        else
          ++zeros_in;
      }
    }

    return static_cast<double>(ones_in) / static_cast<double>(ones_count + zeros_in);
  }

  void copy(std::vector<uint32_t>& source, std::vector<uint32_t>& destination) {
    std::copy(source.begin(), source.end(), destination.begin());
  }

  void VNS() {
    uint32_t best_clasters_count = 0, local_clasters_count = 0, temp_clasters_count = 0;
    double best_efficacy = 0, local_efficacy = 0, temp_efficacy = 0;
    std::vector<uint32_t> best_mclasters, local_mclasters, temp_mclasters;
    std::vector<uint32_t> best_pclasters, local_pclasters, temp_pclasters;
    best_mclasters.resize(mclasters.size());
    best_pclasters.resize(pclasters.size());
    local_mclasters.resize(mclasters.size());
    local_pclasters.resize(pclasters.size());
    temp_mclasters.resize(mclasters.size());
    temp_pclasters.resize(pclasters.size());

    int32_t shaking_id = 0;
    while (shaking_id < SHAKING) {
      // Shaking
      if (rand() % 2 == 0)
        mergeClaster();
      else
        divideClaster();

      copy(mclasters, local_mclasters);
      copy(pclasters, local_pclasters);
      local_efficacy = getEfficacy();
      local_clasters_count = clasters_count;

      // VND
      uint32_t search_id = 0;
      while (search_id < SEARCHING) {
        // Searching
        if (rand() % 2 == 0)
          reassignColumn();
        else
          reassignRow();

        copy(mclasters, temp_mclasters);
        copy(pclasters, temp_pclasters);
        temp_efficacy = getEfficacy();
        temp_clasters_count = clasters_count;

        // Move or not
        if (local_efficacy < temp_efficacy) {
          local_efficacy = temp_efficacy;
          local_clasters_count = temp_clasters_count;
          copy(temp_mclasters, local_mclasters);
          copy(temp_pclasters, local_pclasters);
          search_id = 0;
        } else {
          clasters_count = local_clasters_count;
          copy(local_mclasters, mclasters);
          copy(local_pclasters, pclasters);
          ++search_id;
        }
      }

      // Move or not
      if (best_efficacy < local_efficacy) {
        std::cout << best_efficacy << " -> " << local_efficacy << std::endl;
        best_efficacy = local_efficacy;
        best_clasters_count = local_clasters_count;
        copy(local_mclasters, best_mclasters);
        copy(local_pclasters, best_pclasters);
        shaking_id = 0;
      } else {
        clasters_count = best_clasters_count;
        copy(best_mclasters, mclasters);
        copy(best_pclasters, pclasters);
        ++shaking_id;
      }
    }

    // Save results
    copy(best_mclasters, mclasters);
    copy(best_pclasters, pclasters);
  }
};

int main() {
  srand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

  try {
    Factory F;
    F.VNS();

    std::cout << std::endl;
    std::cout << F.getEfficacy() << std::endl;
    F.printClasters();

    F.writeResult();

  } catch (std::exception& error) {
    std::cerr << error.what() << std::endl;
  }

  return 0;
}
