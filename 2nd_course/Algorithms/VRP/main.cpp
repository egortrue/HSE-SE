#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

#define FILENAME_INPUT "../../data/file1.txt"
#define FILENAME_OUTPUT "../../data/file1.output"

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
  uint32_t trucks_max_count;
  uint32_t capacity;

  std::vector<Customer*> customers;
  std::vector<Truck*> trucks;

  World() {
    std::ifstream fin(FILENAME_INPUT);
    if (!fin.is_open()) throw std::runtime_error("File is not opened");

    // Чтение данных
    fin >> trucks_max_count >> capacity;
    while (!fin.eof()) {
      Customer* c = new Customer();
      fin >> c->id;
      fin >> c->position.x >> c->position.y;
      fin >> c->demand;
      fin >> c->ready_time >> c->due_time >> c->service_time;
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
