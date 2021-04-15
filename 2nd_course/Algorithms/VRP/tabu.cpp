#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

#define FILENAME_INPUT "../../data/file1.txt"
#define FILENAME_OUTPUT "../../data/file1.output"

// Trajectory based algorithm
// (Поиск путей до локального минимума)
// ILS, VNS

//=============================

// Tabu
/**
 Скатываемся в первый локальный минимум
 Выбираем лучший из худших сосдених решений
 Запрещаем обратный ход в первый локальный минимум
 Храним все запреты в TabuList несколько ходов (short term memory) или навсегда
(long term memory)

 (Предотвращает быстрое возвращение в старое решение)
**/

//=============================

// VRPTW (Vehicle Routing Problem with Time Windows)
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

// ОБЯЗАТЕЛЬНО
// - Tabu Search
// - Исследование неверных решений со штрафом

// Страта Бычкова
// первое решение: к каждому кастомеру идёт одна машина
// это решение исследуется со штрафом и находиться правильное решение

struct coord_t {
  int32_t x, y;
};

class Customer {
 public:
  uint32_t id;
  coord_t position;

  uint32_t demand;

  // 0 - 1440
  uint16_t ready_time;
  uint16_t due_time;
  uint16_t service_time;
};

typedef struct {
  Customer* target;
  double start_time;
} action_t;

class Vehicle {
 public:
  uint32_t capacity;
  std::list<action_t> route;
};

class World {
 public:
  uint32_t customers_count;
  uint32_t max_vehicles_count;
  uint32_t max_capacity;

  Customer* depo;
  std::vector<Customer*> customers;
  std::vector<Vehicle*> vehicles;

  double** distances;

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

    distances = new double*[customers_count + 1];
    for (uint32_t i = 0; i <= customers_count; ++i) {
      distances[i] = new double[customers_count + 1];
    }

    for (uint32_t i = 0; i <= customers_count; ++i) {
      distances[i][i] = 0;
      for (uint32_t j = i + 1; j <= customers_count; ++j) {
        if (i == 0) {
          distances[i][j] = distances[j][i] =
              calculate_distance(depo, customers[j - 1]);
          continue;
        }

        distances[i][j] = distances[j][i] =
            calculate_distance(customers[i - 1], customers[j - 1]);
      }
    }
  };

  double calculate_distance(Customer* c1, Customer* c2) {
    return sqrt(pow(c1->position.x - c2->position.x, 2) +
                pow(c1->position.y - c2->position.y, 2));
  }

  void greed() {
    for (uint32_t id = 0; id < this->customers_count; ++id) {
      if (vehicles.size() < max_vehicles_count) {
        Vehicle* v = new Vehicle();
        v->capacity = customers[id]->demand;
        v->route.push_back({customers[id], distances[0][id + 1]});
        vehicles.push_back(v);
      }
    }
  };
};

int main() {
  try {
    World* mWorld = new World();

  } catch (std::exception& error) {
    std::cerr << error.what() << std::endl;
  }
  return 0;
}
