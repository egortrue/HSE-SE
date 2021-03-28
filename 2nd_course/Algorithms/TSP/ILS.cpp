#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

#define FILENAME "D:/Code/projects/Algorithms/src/TSP/data/mona_1000.txt"
#define RESULTS "D:/Code/projects/Algorithms/src/TSP/results.txt"

#define SHIFT 1
#define ITERATIONS 500
#define PERTURBATION 0.2

struct City {
  uint32_t id;
  int x, y;

  City() : id(0), x(0), y(0){};
  City(uint32_t id, int x, int y) : id(id), x(x), y(y){};
};

class Tour {
 public:
  uint32_t N;
  double** distances;        // N * N
  std::vector<City> cities;  // N

  double path_size;
  std::vector<uint32_t> path;  // N + 1

  Tour(int);

  void ILS(uint32_t);
  void computeDistances();
  void computeLength();
  void nearestNeighbourSearch();
  void twoOpt();
  void perturbation();

  double getDistance(uint32_t id_from, uint32_t id_to) {
    return distances[id_from - SHIFT][id_to - SHIFT];
  };
};

Tour::Tour(int N) : N(N), path_size(0) {
  distances = new double*[N];
  for (int i = 0; i < N; i++) distances[i] = new double[N];
}

void Tour::computeDistances() {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      distances[i][j] = distances[j][i] =
          sqrt(pow(cities[i].x - cities[j].x, 2) +
               pow(cities[i].y - cities[j].y, 2));
}

void Tour::computeLength() {
  path_size = 0;
  for (int i = 0; i < N; ++i) path_size += getDistance(path[i], path[i + 1]);
}

void Tour::nearestNeighbourSearch() {
  // New path
  std::vector<uint32_t> tmp_path;

  // Set start city
  int current = rand() % N, success = 0;
  tmp_path.push_back(path[current]);

  for (int i = 0; i < N - 1; i++) {
    for (int j = 0; j < N; j++) {
      bool flag = true;
      for (int k = 0; flag && k < i; k++)
        if (cities[j].id == tmp_path[k]) flag = false;

      if (flag && current == success) success = j;

      if (flag && getDistance(path[current], path[current]) >
                      getDistance(path[current], path[j]))
        success = j;
    }

    current = success;
    tmp_path.push_back(path[current]);
  }

  // Set end city equal to the start one
  tmp_path.push_back(tmp_path.front());

  // Update tour
  for (int i = 0; i < N + 1; i++) path[i] = tmp_path[i];
  computeLength();
}

void Tour::twoOpt() {
  double best_path_size = path_size;

  std::vector<uint32_t> new_path;
  new_path.resize(path.size());
  uint32_t new_path_counter = 0;
  double new_path_size = 0;

  for (uint32_t i = 1; i < N; i++) {
    for (uint32_t j = i + 1; j < N; j++) {
      if (getDistance(i, j) + getDistance(i + 1, j + 1) <
          getDistance(i, j + 1) + getDistance(i + 1, j))
        continue;

      new_path_counter = 0;
      new_path_size = 0;

      for (uint32_t c = 0; c < i; c++) {
        new_path[new_path_counter++] = path[c];
        if (c > 0)
          new_path_size += getDistance(new_path[new_path_counter - 2],
                                       new_path[new_path_counter - 1]);
      }

      for (uint32_t c = j; c >= i; c--) {
        new_path[new_path_counter++] = path[c];
        new_path_size += getDistance(new_path[new_path_counter - 2],
                                     new_path[new_path_counter - 1]);
      }

      for (uint32_t c = j + 1; c < N; c++) {
        new_path[new_path_counter++] = path[c];
        new_path_size += getDistance(new_path[new_path_counter - 2],
                                     new_path[new_path_counter - 1]);
      }

      // Set end city equal to the start one
      new_path[new_path_counter++] = path[0];
      new_path_size += getDistance(new_path[new_path_counter - 2],
                                   new_path[new_path_counter - 1]);

      if (new_path_size < best_path_size) {
        best_path_size = new_path_size;
        std::copy(new_path.begin(), new_path.end(), path.begin());
      }
    }
  }
  // Update path lenght
  path_size = best_path_size;
}

void Tour::perturbation() {
  std::random_device rd;
  std::mt19937 g(rd());

  uint32_t count = (int)(N * PERTURBATION);
  uint32_t pos = rand() % (N - count);

  shuffle(path.begin() + pos, path.begin() + pos + count, g);
  computeLength();
}

void Tour::ILS(uint32_t iterations) {
  // Initial solution
  nearestNeighbourSearch();
  std::vector<uint32_t> best_path;
  best_path.resize(path.size());
  double best_path_size = path_size;
  std::copy(path.begin(), path.end(), best_path.begin());

  // Main cycle
  for (int i = 1; i <= iterations + 1; ++i) {
    double previous_value = path_size;

    // Local optimization
    while (true) {
      twoOpt();
      double current_value = path_size;

      if (current_value != previous_value) {
        std::cout << previous_value << " -> " << current_value << std::endl;
        previous_value = current_value;
      } else {
        break;
      }
    }

    // Update the best case
    if (previous_value < best_path_size) {
      best_path_size = previous_value;
      std::copy(path.begin(), path.end(), best_path.begin());
      // Temporal output for interaptions in runtime
      for (auto id : path) std::cout << id << " ";
      std::cout << "\n";
    }
    // OR Return to the best case
    else {
      std::copy(best_path.begin(), best_path.end(), path.begin());
      path_size = best_path_size;
    }

    // Do not the last perturbation
    if (i == iterations + 1) break;

    // Ruin the best case
    std::cout << "perturbation #" << i << std::endl;
    perturbation();
  }

  // Save the best case to the path
  std::copy(best_path.begin(), best_path.end(), path.begin());
  path_size = best_path_size;
}

int main() {
  srand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

  std::ifstream fin(FILENAME);
  if (!fin.is_open()) std::runtime_error("File is not opened");

  // Read the data
  uint32_t N;
  fin >> N;
  Tour tour(N);
  for (uint32_t i = 0; i < N; i++) {
    uint32_t id;
    int x, y;
    fin >> id >> x >> y;
    tour.cities.push_back(City(id, x, y));
    tour.path.push_back(id);
  }
  fin.close();

  // Set end city equal to the start one
  tour.path.push_back(tour.path.front());
  tour.computeDistances();

  // Main
  tour.ILS(ITERATIONS);

  // Results (cmd)
  std::cout << FILENAME << "\nResult: " << tour.path_size << std::endl;
  for (auto id : tour.path) std::cout << id << " ";
  std::cout << "\n\n";

  // Results for drawing (file)
  std::ofstream fout(RESULTS);
  if (!fout.is_open()) std::runtime_error("File is not opened");
  fout << FILENAME << " " << N << " " << tour.path_size << "\n";
  for (auto id : tour.path) std::cout << id << " ";
  std::cout << "\n";
  for (auto id : tour.path) {
    fout << id << " " << tour.cities[id - SHIFT].x << " "
         << tour.cities[id - SHIFT].y << "\n";
  }
  fout << "\n\n";

  return 0;
}
