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
 Храним все запреты в TabuList несколько ходов (short term memory) или навсегда (long term memory)

 (Предотвращает быстрое возвращение в старое решение)
**/

//=============================

// VRPTW (Vehicle Routing Problem with Time Windows)
/**
1. первый кастомер не депо
2. Время пути до кастомера больше суммы путей до него
3. Соблюдение окон
4. Перегруз по объему
5. Раутов <= кол-во машин
6. Все кастомеры должны быть обслужены
7. Система штрафов (penalty = 2 -> 2^2 -> 2^2^2)
  При не соблюдении временных рамок - исследуем это решение но со штрафом (растёт экспоненциально)

**/

// ОБЯЗАТЕЛЬНО
// - Tabu Search
// - Исследование неверных решений со штрафом

// Страта бычкова
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

class Truck {
 public:
  uint32_t capacity;
  uint32_t id;

  std::list<Customer*> route;
};

class World {
 public:
  uint32_t max_trucks_count;
  uint32_t max_capacity;

  Customer* depo;
  std::vector<Customer*> customers;
  std::vector<Truck*> trucks;

  World() {
    std::ifstream fin(FILENAME_INPUT);
    if (!fin.is_open()) throw std::runtime_error("File is not opened");

    // Чтение данных
    bool first_is_depo = true;
    fin >> max_trucks_count >> max_capacity;
    while (!fin.eof()) {
      Customer* c = new Customer();
      fin >> c->id;
      fin >> c->position.x >> c->position.y;
      fin >> c->demand;
      fin >> c->ready_time >> c->due_time >> c->service_time;
      if (first_is_depo) {
        depo = c;
        first_is_depo = false;
        continue;
      }
      
      customers.push_back(c);
    }

    fin.close();

  };

  void printCustomers() {
    for (auto c : this->customers) {
      std::cout << c->id << " " << c->position.x << " " << c->position.y
                << std::endl;
    }
  }
};

int main() {
  try {
    World* mWorld = new World();
    mWorld->printCustomers();

  } catch (std::exception& error) {
    std::cerr << error.what() << std::endl;
  }
  return 0;
}
