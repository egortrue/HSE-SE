// VRPTW (Vehicle Routing Problem with Time Windows)

//======================
// Trajectory based algorithm (Поиск путей до локального минимума)
// ILS, VNS

//======================
// Tabu search
/**
 Скатываемся в первый локальный минимум
 Выбираем лучший из худших сосдених решений
 Запрещаем обратный ход в первый локальный минимум
 Храним все запреты в TabuList несколько ходов (short term memory) или навсегда
(long term memory)

 (Предотвращает быстрое возвращение в старое решение)
**/

//======================
// VRPTW

/**
Чекеры
1. первый кастомер не депо
2. Время пути до кастомера больше суммы путей до него
3. Соблюдение окон
4. Перегруз по объему
5. Раутов <= кол-во машин
6. Все кастомеры должны быть обслужены
7. Система штрафов (penalty = 2 -> 2^2 -> 2^2^2)
  При не соблюдении временных рамок - исследуем это решение но со штрафом
(растёт экспоненциально)

Ответ:
id1 startTime1 id2 startTime2 id3 ..... id1 startTimeN
id1 startTime1 id2 startTime2 id3 ..... id1 startTimeN
id1 startTime1 id2 startTime2 id3 ..... id1 startTimeN
**/

//======================
// Обязательно
// - Tabu Search
// - Исследование неверных решений со штрафом

//======================
// Совет
// первое решение: к каждому кастомеру идёт одна машина
// это решение исследуется со штрафом и находиться правильное решение

//==================================================================================

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <vector>

#define FILENAME_INPUT "../../data/I1.txt"
#define FILENAME_OUTPUT "../../data/file1.output"

struct coord_t {
  int32_t x, y;
};

class Customer {
 public:
  // Входные значения
  uint32_t id;
  coord_t position;
  uint32_t demand;
  uint16_t ready_time;    // 0 - 1440
  uint16_t due_time;      // 0 - 1440
  uint16_t service_time;  // 0 - 1440

  bool visited = false;
};

struct action_t {
  Customer* target;
  uint32_t begin_time;
};

uint32_t max_capacity;
Customer* depo;
uint32_t** distances;

class Vehicle {
 public:
  uint32_t capacity;
  uint32_t penalty;
  std::list<action_t> route;

  Vehicle() {
    this->capacity = 0;
    this->penalty = 0;
    this->route.clear();
  }

  Vehicle(const Vehicle& orig) {
    this->capacity = orig.capacity;
    this->penalty = orig.penalty;
    this->route.clear();
    if (orig.route.size() > 1) {
      for (auto iterator = orig.route.begin(); iterator != orig.route.end();
           ++iterator) {
        this->route.push_back(*iterator);
      }
    } else if (orig.route.size() == 1) {
      this->route.push_back(orig.route.front());
    }
  }

  void addCustomer(Customer* c) {
    capacity += c->demand;

    if (route.size() != 0) {
      // Покупатели вставляются по увелечению времени закрытия
      for (auto iterator = route.begin(); iterator != route.end(); ++iterator) {
        if (c->due_time < iterator->target->due_time) {
          route.insert(iterator, {c, 0});
          calculateTimeline();
          return;
        }
      }
    }

    route.push_back({c, 0});
    calculateTimeline();
  }

  Customer* popCustomer() {
    // У валидного маршрута вытаскиваем первого покупателя
    if (penalty == 0) {
      action_t obj = route.front();
      route.pop_front();
      capacity -= obj.target->demand;
      calculateTimeline();
      return obj.target;
    }

    // У не валидного маршрута ищем покупателя, к которому не успеваем
    for (auto iterator = route.begin(); iterator != route.end(); ++iterator) {
      if (iterator->begin_time > iterator->target->due_time) {
        action_t obj = *iterator;
        route.erase(iterator);
        capacity -= obj.target->demand;
        calculateTimeline();
        return obj.target;
      }
    }

    // Вытащим хотя бы последнего покупателя
    action_t obj = route.back();
    route.pop_back();
    capacity -= obj.target->demand;
    calculateTimeline();
    return obj.target;
  }

  uint32_t calculatePenalty() {
    penalty = 0;

    // Перегруз
    if (capacity > max_capacity) penalty += capacity - max_capacity;

    // Опоздание
    for (auto obj : route) {
      if (obj.begin_time > obj.target->due_time)
        penalty += obj.begin_time - obj.target->due_time;
    }

    return penalty;
  }

  // Пересчёт времени прибытия для каждого покупателя в маршруте
  void calculateTimeline() {
    auto prev_iterator = route.begin();
    auto cur_iterator = route.begin();
    if (cur_iterator == route.end()) return;

    cur_iterator->begin_time = distances[0][cur_iterator->target->id];
    if (cur_iterator->begin_time < cur_iterator->target->ready_time)
      cur_iterator->begin_time = cur_iterator->target->ready_time;
    ++cur_iterator;

    while (cur_iterator != route.end()) {
      cur_iterator->begin_time =
          prev_iterator->begin_time + prev_iterator->target->service_time +
          distances[prev_iterator->target->id][cur_iterator->target->id];

      if (cur_iterator->begin_time < cur_iterator->target->ready_time)
        cur_iterator->begin_time = cur_iterator->target->ready_time;

      ++prev_iterator;
      ++cur_iterator;
    }
  }
};

struct solution_t {
  double quality;
  uint32_t penalty;              // Штраф решения
  std::list<Vehicle*> vehicles;  // Маршруты
};

// Short-term memory
typedef std::list<std::pair<Customer*, uint32_t>> tabuList_t;

class World {
 public:
  uint32_t customers_count;
  uint32_t max_vehicles_count;
  std::vector<Customer*> customers;

  // Short-term memory
  tabuList_t tabuList;

  World() {
    std::ifstream fin(FILENAME_INPUT);
    if (!fin.is_open()) throw std::runtime_error("File is not opened");

    // Чтение данных
    fin >> customers_count >> max_vehicles_count >> max_capacity;
    for (uint32_t i = 0; i <= customers_count; ++i) {
      Customer* c = new Customer();
      fin >> c->id;
      fin >> c->position.x >> c->position.y;
      fin >> c->demand;
      fin >> c->ready_time >> c->due_time >> c->service_time;
      if (i == 0)
        depo = c;
      else
        customers.push_back(c);
    }

    fin.close();

    // Выделим память под матрицу расстояний
    distances = new uint32_t*[customers_count + 1];
    for (uint32_t i = 0; i <= customers_count; ++i) {
      distances[i] = new uint32_t[customers_count + 1];
    }

    // Расстояния между покупателями
    for (uint32_t i = 0; i <= customers_count; ++i) {
      distances[i][i] = 0;
      for (uint32_t j = i + 1; j <= customers_count; ++j) {
        if (i == 0) {
          distances[0][j] = distances[j][0] =
              calculateDistance(depo, customers[j - 1]);
          continue;
        }

        distances[i][j] = distances[j][i] =
            calculateDistance(customers[i - 1], customers[j - 1]);
      }
    }
  };

  uint32_t calculateDistance(Customer* c1, Customer* c2) {
    return ceil(sqrt(pow(c1->position.x - c2->position.x, 2) +
                     pow(c1->position.y - c2->position.y, 2)));
  }

  // До каждого покупателя едет своя машина
  void initialSolution(solution_t& solution) {
    for (uint32_t id = 0; id < customers_count; ++id) {
      Vehicle* v = new Vehicle();
      v->addCustomer(customers[id]);
      solution.vehicles.push_back(v);
    }
  }

  void calculateQuality(solution_t& solution) {
    calculatePenalty(solution);
    if (solution.penalty != 0)
      solution.quality = 0.0;
    else {
      solution.quality = 1.0 - solution.vehicles.size() / max_vehicles_count;
    }
  }

  void calculatePenalty(solution_t& solution) {
    solution.penalty = 0;

    // Лишние машины
    if (solution.vehicles.size() > max_vehicles_count)
      solution.penalty += (solution.vehicles.size() - max_vehicles_count) * 100;

    // Сумма штрафов каждого маршрута
    for (auto vehicle : solution.vehicles)
      solution.penalty += vehicle->calculatePenalty();
  }

  void checkEmptyRoutes(solution_t& solution) {
    std::list<std::list<Vehicle*>::iterator> iterators;
    for (auto iterator = solution.vehicles.begin();
         iterator != solution.vehicles.end(); ++iterator)
      if ((*iterator)->route.size() == 0) iterators.push_back(iterator);

    for (auto iterator : iterators) {
      delete (*iterator);
      solution.vehicles.erase(iterator);
    }
  }

  tabuList_t::iterator tabuListFind(tabuList_t::iterator first,
                                    tabuList_t::iterator last, Customer* c) {
    while (first != last) {
      if ((*first).first == c) return first;
      ++first;
    }
    return last;
  }

  // Расперделим покупателей из случайных маршрутов
  void reassignRandomVehicles(solution_t& solution) {
    if (solution.vehicles.size() == 1) {
      Vehicle* new_vehicle = new Vehicle();
      Vehicle* old_vehicle = solution.vehicles.front();

      auto customer = old_vehicle->popCustomer();

      auto iterator = tabuListFind(tabuList.begin(), tabuList.end(), customer);
      if (iterator != tabuList.end()) {
        old_vehicle->addCustomer(customer);  // Вернём покупателя на место
        iterator->second -= 1;  // Уменьшим счётчик неприкосновенности
        checkTabuList();
        return;
      }

      new_vehicle->addCustomer(customer);
      solution.vehicles.push_back(new_vehicle);
    }

    do {
      auto random_vehicle1 = solution.vehicles.begin();
      std::advance(random_vehicle1, std::rand() % solution.vehicles.size());

      auto random_vehicle2 = solution.vehicles.begin();
      std::advance(random_vehicle2, std::rand() % solution.vehicles.size());

      auto customer = (*random_vehicle1)->popCustomer();

      auto iterator = tabuListFind(tabuList.begin(), tabuList.end(), customer);
      if (iterator != tabuList.end()) {
        (*random_vehicle1)
            ->addCustomer(customer);  // Вернём покупателя на место
        iterator->second -= 1;  // Уменьшим счётчик неприкосновенности
        checkTabuList();
        continue;
      }

      (*random_vehicle2)->addCustomer(customer);
      checkEmptyRoutes(solution);
    } while (solution.vehicles.size() > max_vehicles_count);
  }

  // Расперделим покупателей из худшего маршрута
  void reassignWorstVehicle(solution_t& solution) {
    uint32_t max_penalty = 0;
    Vehicle* worst_vehicle = nullptr;
    for (auto vehicle : solution.vehicles) {
      if (vehicle->penalty > max_penalty) {
        worst_vehicle = vehicle;
        max_penalty = vehicle->penalty;
      }
    }

    if (worst_vehicle != nullptr) {
      while (worst_vehicle->calculatePenalty() != 0) {
        auto customer = worst_vehicle->popCustomer();

        auto iterator =
            tabuListFind(tabuList.begin(), tabuList.end(), customer);
        if (iterator != tabuList.end()) {
          worst_vehicle->addCustomer(customer);  // Вернём покупателя на место
          iterator->second -= 1;  // Уменьшим счётчик неприкосновенности
          checkTabuList();
          continue;
        }

        auto random_vehicle = solution.vehicles.begin();
        std::advance(random_vehicle, std::rand() % solution.vehicles.size());
        (*random_vehicle)->addCustomer(customer);
      }

      checkEmptyRoutes(solution);
    }
  }

  void checkTabuList() {
    std::list<tabuList_t::iterator> iterators;
    for (auto iterator = tabuList.begin(); iterator != tabuList.end();
         ++iterator)
      if (iterator->second <= 0) iterators.push_back(iterator);

    for (auto iterator : iterators) {
      tabuList.erase(iterator);
    }
  }

  // Случайный ход для выхода из локального минимума
  void tabuMove(solution_t& solution) {
    auto random_vehicle1 = solution.vehicles.begin();
    std::advance(random_vehicle1, std::rand() % solution.vehicles.size());

    auto random_vehicle2 = solution.vehicles.begin();
    std::advance(random_vehicle2, std::rand() % solution.vehicles.size());

    auto customer = (*random_vehicle1)->popCustomer();
    tabuList.push_back({customer, 10});
    (*random_vehicle2)->addCustomer(customer);
    checkEmptyRoutes(solution);
  }

  void printSolution(const solution_t& solution) {
    std::cout << "Solution (penalty=" << solution.penalty
              << ",quality=" << solution.quality << ")" << std::endl;
    for (auto vehicle : solution.vehicles) {
      for (auto action : vehicle->route)
        std::cout << action.target->id << " " << action.begin_time << " ";
      std::cout << std::endl;
    }
  }

  void printDistances() {
    for (uint32_t i = 0; i <= customers_count; ++i) {
      for (uint32_t j = 0; j <= customers_count; ++j)
        std::cout << distances[i][j] << " ";
      std::cout << std::endl;
    }
  }

  void copySolution(const solution_t& orig_solution, solution_t& new_solution) {
    new_solution.penalty = orig_solution.penalty;
    new_solution.quality = orig_solution.quality;
    for (auto vehicle : orig_solution.vehicles)
      new_solution.vehicles.push_back(new Vehicle(*vehicle));
  }

  void destroySolution(solution_t& solution) {
    for (auto vehicle : solution.vehicles) delete vehicle;
    solution.vehicles.clear();
  }

  void run() {
    solution_t current;
    initialSolution(current);
    calculateQuality(current);

    uint32_t tabu_actions = 0;
    while (tabu_actions++ < 10) {
      // Поиск локального минимума
      uint32_t local_minimum = 0;
      while (local_minimum++ < 50) {
        solution_t best_candidate, cur_candidate;
        copySolution(current, best_candidate);
        for (uint32_t i = 0; i < customers_count * 2; ++i) {
          copySolution(current, cur_candidate);

          if (current.vehicles.size() > max_vehicles_count)
            reassignRandomVehicles(cur_candidate);
          else
            reassignWorstVehicle(cur_candidate);

          calculateQuality(cur_candidate);
          if (cur_candidate.penalty < best_candidate.penalty ||
              cur_candidate.quality > best_candidate.quality) {
            std::cout << "(" << current.penalty << ", " << current.quality
                      << ") -> (" << best_candidate.penalty << ", "
                      << best_candidate.quality << ")" << std::endl;
            destroySolution(best_candidate);
            copySolution(cur_candidate, best_candidate);
          }

          destroySolution(cur_candidate);
        }

        if (best_candidate.penalty < current.penalty ||
            best_candidate.quality > current.quality) {
          destroySolution(current);
          copySolution(best_candidate, current);
        }

        destroySolution(best_candidate);
      }

      tabuMove(current);
      calculateQuality(current);
    }
    printSolution(current);
  }
};

int main() {
  std::srand(std::chrono::system_clock::now().time_since_epoch().count());

  try {
    World w;
    w.run();

  } catch (std::exception& error) {
    std::cerr << error.what() << std::endl;
  }
  return 0;
}
