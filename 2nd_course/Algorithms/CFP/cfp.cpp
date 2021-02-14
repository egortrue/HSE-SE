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
#define SHAKING 500

struct Cluster {
  uint32_t id;
  std::vector<uint32_t> machines;
  std::vector<uint32_t> parts;
};

class Factory {
  uint32_t M, P;
  uint32_t ones_count = 0;
  std::vector<std::vector<bool>> matrix;

  uint32_t clusters_count = 0;
  std::vector<uint32_t> mclusters, pclusters;
  std::vector<Cluster> clusters;

 public:
  Factory() {
    std::ifstream fin(FILENAME);
    if (!fin.is_open()) throw std::runtime_error("File is not opened");

    // Init matrix
    fin >> M >> P;
    matrix.resize(M);
    for (uint32_t i = 0; i < M; i++) {
      matrix[i].resize(P);

      // Read the data
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
    }

    mclusters.resize(M);
    pclusters.resize(P);
    clusters_count = 1;
    for (uint32_t i = 0; i < M; i++) mclusters[i] = 1;
    for (uint32_t i = 0; i < P; i++) pclusters[i] = 1;
    updateClusters();

    fin.close();
  }

  void updateClusters() {
    clusters.clear();
    clusters.resize(clusters_count);

    for (uint32_t i = 0; i < M; i++) {
      uint32_t cluster = mclusters[i];
      clusters[cluster - 1].id = cluster;
      clusters[cluster - 1].machines.push_back(i);
    }

    for (uint32_t i = 0; i < P; i++) clusters[pclusters[i] - 1].parts.push_back(i);
  }

  void printClusters() {
    for (auto id : mclusters) std::cout << id << " ";
    std::cout << std::endl;
    for (auto id : pclusters) std::cout << id << " ";
    std::cout << std::endl;
  }

  void printMatrix() {
    for (auto row : matrix) {
      for (auto el : row) std::cout << el << " ";
      std::cout << std::endl;
    }
  }

  void writeResult() {
    std::ifstream fin_temp(RESULTS);
    if (!fin_temp.is_open()) {
      std::ofstream temp(RESULTS);
      temp.close();
    } else
      fin_temp.close();

    std::ifstream fin(RESULTS);
    double best_efficacy;
    fin >> best_efficacy;
    fin.close();

    double current_efficacy = getEfficacy();
    if (best_efficacy < current_efficacy) {
      std::ofstream fout(RESULTS);
      fout << current_efficacy << std::endl << std::endl;

      for (uint32_t val : mclusters) fout << val << " ";
      fout << std::endl;
      for (uint32_t val : pclusters) fout << val << " ";
      fout << std::endl << std::endl;

      std::vector<uint32_t> machines;
      std::vector<uint32_t> parts;
      machines.resize(M);
      parts.resize(P);

      uint32_t pos = 0;
      for (uint32_t cluster = 1; cluster <= clusters_count; cluster++)
        for (uint32_t mpos = 0; mpos < M; mpos++)
          if (mclusters[mpos] == cluster) machines[pos++] = mpos;

      pos = 0;
      for (uint32_t cluster = 1; cluster <= clusters_count; cluster++)
        for (uint32_t ppos = 0; ppos < P; ppos++)
          if (pclusters[ppos] == cluster) parts[pos++] = ppos;

      for (auto mpos : machines) {
        for (auto ppos : parts) fout << matrix[mpos][ppos] << " ";
        fout << std::endl;
      }

      fout.close();
    }
  }

  double getEfficacy() {
    uint32_t ones_in = 0, zeros_in = 0;

    for (uint32_t row = 0; row < M; row++) {
      for (uint32_t col = 0; col < P; col++) {
        if (mclusters[row] != pclusters[col]) continue;
        if (matrix[row][col])
          ++ones_in;
        else
          ++zeros_in;
      }
    }

    return static_cast<double>(ones_in) / static_cast<double>(ones_count + zeros_in);
  }

  void divideClaster() {
    if (clusters_count == std::min(M, P)) return;

    // Choose random claster to divide
    std::vector<uint32_t> pool;
    for (uint32_t i = 1; i <= clusters_count; i++) {
      if (clusters[i - 1].machines.size() >= 2 && clusters[i - 1].parts.size() >= 2)
        pool.push_back(i);
    }
    if (pool.size() == 0) return;
    uint32_t master = pool[rand() % pool.size()];

    // Choose random element to seperate from claster
    uint32_t new_cluster_row =
        clusters[master - 1].machines[rand() % clusters[master - 1].machines.size()];
    uint32_t new_cluster_col =
        clusters[master - 1].parts[rand() % clusters[master - 1].parts.size()];

    // Set new claster
    ++clusters_count;
    mclusters[new_cluster_row] = clusters_count;
    pclusters[new_cluster_col] = clusters_count;
    updateClusters();
  }

  void mergeClaster() {
    if (clusters_count == 1) return;

    // Choose random claster
    std::vector<uint32_t> pool;
    for (uint32_t i = 1; i <= clusters_count; i++) pool.push_back(i);
    uint32_t cluster1 = pool[rand() % pool.size()];

    // Choose other random claster
    pool.clear();
    for (uint32_t i = 1; i <= clusters_count; i++) {
      if (cluster1 != i) pool.push_back(i);
    }
    uint32_t cluster2 = pool[rand() % pool.size()];

    // Unite clasters
    uint32_t master_cluster = std::min(cluster1, cluster2);
    uint32_t adder_cluster = std::max(cluster1, cluster2);

    for (uint32_t pos = 0; pos < M; pos++) {
      if (mclusters[pos] == adder_cluster) mclusters[pos] = master_cluster;
      if (mclusters[pos] > adder_cluster) mclusters[pos] -= 1;
    }

    for (uint32_t pos = 0; pos < P; pos++) {
      if (pclusters[pos] == adder_cluster) pclusters[pos] = master_cluster;
      if (pclusters[pos] > adder_cluster) pclusters[pos] -= 1;
    }

    --clusters_count;
    updateClusters();
  }

  void copy(std::vector<uint32_t> &source, std::vector<uint32_t> &destination) {
    std::copy(source.begin(), source.end(), destination.begin());
  }

  void VNS() {
    double best_efficacy = 0;
    uint32_t best_clusters_count = 0;
    std::vector<uint32_t> best_mclusters;
    std::vector<uint32_t> best_pclusters;
    best_mclusters.resize(M);
    best_pclusters.resize(P);

    uint32_t shaking_id = 0;
    while (shaking_id < SHAKING) {
      if (shaking_id < SHAKING / 2)
        mergeClaster();
      else
        divideClaster();

      double local_efficacy = getEfficacy();
      uint32_t local_clusters_count = clusters_count;

    //===================================================================
    // VND
    label:
      for (uint32_t row = 0; row < M; row++) {
        for (uint32_t cluster = 1; cluster <= clusters_count; cluster++) {
          if (mclusters[row] == cluster) continue;
          if (clusters[mclusters[row] - 1].machines.size() == 1) continue;

          uint32_t temp = mclusters[row];
          mclusters[row] = cluster;
          double temp_efficacy = getEfficacy();
          if (local_efficacy < temp_efficacy) {
            local_efficacy = temp_efficacy;
            updateClusters();
            goto label;
          } else {
            mclusters[row] = temp;
          }
        }
      }

      for (uint32_t col = 0; col < P; col++) {
        for (uint32_t cluster = 1; cluster <= clusters_count; cluster++) {
          if (pclusters[col] == cluster) continue;
          if (clusters[pclusters[col] - 1].parts.size() == 1) continue;

          uint32_t temp = pclusters[col];
          pclusters[col] = cluster;
          double temp_efficacy = getEfficacy();
          if (local_efficacy < temp_efficacy) {
            local_efficacy = temp_efficacy;
            updateClusters();
            goto label;
          } else {
            pclusters[col] = temp;
          }
        }
      }
      //===================================================================

      // Move or not
      if (best_efficacy < local_efficacy) {
        std::cout << best_efficacy << " -> " << local_efficacy << std::endl;
        best_efficacy = local_efficacy;
        best_clusters_count = local_clusters_count;
        copy(mclusters, best_mclusters);
        copy(pclusters, best_pclusters);
        shaking_id = 0;
      } else {
        clusters_count = best_clusters_count;
        copy(best_mclusters, mclusters);
        copy(best_pclusters, pclusters);
        ++shaking_id;
        updateClusters();
      }
    }

    // Save results
    clusters_count = best_clusters_count;
    copy(best_mclusters, mclusters);
    copy(best_pclusters, pclusters);
  }
};

int main() {
  srand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

  try {
    Factory F;
    F.VNS();

    std::cout << F.getEfficacy() << std::endl;
    F.printClusters();
    std::cout << std::endl;
    F.writeResult();
  } catch (std::exception &error) {
    std::cerr << error.what() << std::endl;
  }

  return 0;
}
