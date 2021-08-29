
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

#define FILENAME "../../data/lu980.txt"
#define RESULTS "../../data/lu980.result"

#define ID_SHIFT 1               // Relative to zero
#define GENERATION_SIZE 1000     // Count of solutions on current stage
#define GENERATION_COUNT 100000  // Max count of generations

typedef std::vector<uint32_t> path_t;

struct City {
  uint32_t id;
  double x, y;

  City(uint32_t id, double x, double y) : id(id), x(x), y(y){};
};

class GeneticSolver {
 public:
  // Input data
  uint32_t N;
  std::vector<City> cities;

  std::mt19937 generator;
  double** distances;  // Distances between each pair of cities

  uint32_t generation_id;
  path_t* generation;  // Generated solutions on current stage
  std::vector<double> generation_fitness;  // Fitness values for generation

  std::vector<uint32_t> wheel;  // selection pool for current generation

  //==================================================================

  GeneticSolver() {
    // Create generator
    std::random_device rd;
    generator = std::mt19937(rd());

    // Read the data
    std::ifstream fin(FILENAME);
    if (!fin.is_open()) std::runtime_error("File is not opened");
    fin >> N;
    for (uint32_t i = 0; i < N; ++i) {
      uint32_t id;
      double x, y;
      fin >> id >> x >> y;
      cities.push_back(City(id, x, y));
    }
    fin.close();

    // Init distances
    distances = new double*[N];
    for (uint32_t i = 0; i < N; ++i) distances[i] = new double[N];
    for (uint32_t i = 0; i < N; ++i) {
      for (uint32_t j = 0; j < N; ++j)
        distances[i][j] = distances[j][i] =
            sqrt(pow(cities[i].x - cities[j].x, 2) +
                 pow(cities[i].y - cities[j].y, 2));
    }

    generation = new path_t[GENERATION_SIZE];
    generation_fitness.resize(GENERATION_SIZE);
  }

  ~GeneticSolver() {
    for (uint32_t i = 0; i < N; ++i) delete[] distances[i];
    delete[] distances;
    delete[] generation;
  }

  //==================================================================

  void generateRandomSolution(path_t& solution) {
    solution.clear();
    solution.resize(N);
    for (uint32_t i = 0; i < N; ++i) solution[i] = i;
    shuffle(solution.begin(), solution.end(), generator);
    solution.shrink_to_fit();
  }

  void initFirstGeneration() {
    for (uint32_t i = 0; i < GENERATION_SIZE; ++i)
      generateRandomSolution(generation[i]);
  }

  //==================================================================

  double calculateFitness(path_t& solution) {
    double result = 0;
    // Highly optimized cycle
    uint32_t* end = &solution[N - 1];
    for (uint32_t* i = &solution[0]; i != end; ++i)
      result += distances[*i][*(i + 1)];
    return result + distances[solution[N - 1]][solution[0]];
  }

  void estimateGeneration() {
    for (uint32_t i = 0; i < GENERATION_SIZE; ++i)
      generation_fitness[i] = calculateFitness(generation[i]);
  }

  //==================================================================

  void createWheel() {
    wheel.clear();
    double sum = 0;
    for (uint32_t i = 0; i < GENERATION_SIZE; ++i) sum += generation_fitness[i];

    for (uint32_t id = 0; id < GENERATION_SIZE; ++id) {
      uint32_t percentage = std::floor(1.0 / (generation_fitness[id] / sum));
      for (uint32_t j = 0; j < percentage; ++j) wheel.push_back(id);
    }
    wheel.shrink_to_fit();
  }

  uint32_t wheelSelection() {
    if (wheel.size() == 0) createWheel();
    std::uniform_int_distribution<> range(0, wheel.size() - 1);
    return wheel[range(generator)];
  }

  uint32_t tournamentSelection(uint32_t count) {
    if (count > GENERATION_SIZE)
      throw std::runtime_error("incorrect tournament size");
    std::uniform_int_distribution<> range(0, GENERATION_SIZE - 1);

    uint32_t winner;
    double best_value = UINT_MAX;
    for (uint32_t id = 0; id < count; ++id) {
      double value = generation_fitness[range(generator)];
      if (value < best_value) {
        best_value = value;
        winner = id;
      }
    }
    return winner;
  }

  //==================================================================

  void swapMutation(path_t& solution) {
    std::uniform_int_distribution<> range(0, N - 1);
    std::swap(solution[range(generator)], solution[range(generator)]);
  }

  void scrumbleMutation(path_t& solution) {
    std::uniform_int_distribution<> pos_range(0, N - 2);
    uint32_t pos = pos_range(generator);
    std::uniform_int_distribution<> count_range(1, N - pos);
    uint32_t count = count_range(generator);
    std::cout << count << " " << pos << std::endl;
    shuffle(solution.begin() + pos, solution.begin() + pos + count, generator);
  }

  //==================================================================

  int32_t find(std::vector<uint32_t>& vec, uint32_t value) {
    uint32_t size = vec.size();
    for (uint32_t i = 0; i < size; ++i)
      if (vec[i] == value) return i;
    return -1;
  }

  // https://cw.fel.cvut.cz/b201/_media/courses/a0m33eoa/du/puljic2013crossoversforvrp.pdf
  void partiallyMappedCrossover(path_t& p1, path_t& p2, path_t& offspring) {
    std::vector<bool> mask;
    mask.resize(N);
    for (uint32_t i = 0; i < N; ++i) mask[i] = generator() % 2;

    std::vector<uint32_t> p1_gens, p2_gens;
    for (uint32_t i = 0; i < N; ++i) {
      if (mask[i]) {
        p1_gens.push_back(p1[i]);
        p2_gens.push_back(p2[i]);
      }
    }

    path_t c1, c2;
    c1.resize(N);
    c2.resize(N);
    for (uint32_t i = 0; i < N; ++i) {
      if (mask[i]) {
        c1[i] = p1[i];
        c2[i] = p2[i];
      } else {
        int32_t gen_pos = find(p1_gens, p2[i]);
        c1[i] = (gen_pos >= 0) ? p2_gens[gen_pos] : p2[i];
        gen_pos = find(p2_gens, p1[i]);
        c2[i] = (gen_pos >= 0) ? p1_gens[gen_pos] : p1[i];
      }
    }

    double c1_fit = calculateFitness(c1);
    double c2_fit = calculateFitness(c2);
    if (c1_fit < c2_fit)
      std::copy(c1.begin(), c1.end(), offspring.begin());
    else
      std::copy(c2.begin(), c2.end(), offspring.begin());
    offspring.shrink_to_fit();
  }

  //==================================================================

  void runEvolution() {
    initFirstGeneration();
    estimateGeneration();

    while (++generation_id < GENERATION_COUNT) {
      path_t* old_generation = generation;
      path_t* new_generation = new path_t[GENERATION_SIZE];

      for (uint32_t i = 0; i < GENERATION_SIZE; ++i) {
        // Select parents
        path_t parent1 = generation[wheelSelection()];
        path_t parent2 = generation[wheelSelection()];

        // Reproduction
        partiallyMappedCrossover(parent1, parent2, new_generation[i]);

        // Mutate
        scrumbleMutation(new_generation[i]);
      }

      generation = new_generation;
      estimateGeneration();
      delete[] old_generation;
      wheel.clear();
    }

    double best_fitness = UINT_MAX;
    uint32_t generation_id;
    for (uint32_t i = 0; i < GENERATION_SIZE; ++i) {
      if (generation_fitness[i] < best_fitness) {
        best_fitness = generation_fitness[i];
        generation_id = i;
      }
    }
    std::cout << best_fitness << std::endl;
  }

  //==================================================================
};

int main() {
  try {
    auto solver = new GeneticSolver();
    solver->runEvolution();
    delete solver;
  } catch (std::exception& error) {
    std::cerr << error.what() << std::endl;
  }

  return 0;
}
