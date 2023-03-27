

#include "common.hpp"
#include "matrix.hpp"
#include "mpi.hpp"

enum ALGORITHM {
    ROW,
    COLUMNS,
    BLOCK
} algorithm;

// Общие данные
int N;

// Данные главного потока
Matrix matrix1;
Matrix matrix2;
Matrix result;
int rows_counter;
int cols_counter;

// Инициализация - только на главном потоке
void init() {
    if (MPI::processID == MPI::MAIN_PROCESS) {
        matrix1 = Matrix(N, N, Matrix::RANDOM);
        matrix2 = Matrix(N, N, Matrix::RANDOM);
        result = Matrix(N, N, Matrix::ZERO);
        rows_counter = N;
        cols_counter = N;

#ifdef DEBUG
        std::cout << matrix1;
        std::cout << matrix2;
#endif
    }
}

// Управляющий поток
void mainRoutine() {
    if (algorithm == ROW) {
        for (int row = 0; row < N; ++row) {
            for (int col = 0; col < N; ++col) {
                MPI::sendVector(row, matrix1, MPI::TAG_ROW);
                MPI::sendVector(col, matrix2, MPI::TAG_COL);
                MPI::getValue(result);
            }
        }

        // Останавливаем все потоки, когда данные закончились
        MPI::stop();
    } else if (algorithm == COLUMNS) {
    }
}

// Вычисляющие потоки
void coRoutine() {
    if (algorithm == ROW) {
        int rowIndex = -1, colIndex = -1;
        std::vector<double> row = std::vector<double>(N);
        std::vector<double> col = std::vector<double>(N);
        while (true) {
            rowIndex = -1, colIndex = -1;
            if (MPI::isStopped()) break;
            MPI::getVector(row, rowIndex, MPI::TAG_ROW);  // Блокирует, пока не придет
            MPI::getVector(col, colIndex, MPI::TAG_COL);  // Блокирует, пока не придет

            // Результат
            if (MPI::isStopped()) break;
            double value = row * col;
            MPI::sendValue(value, rowIndex, colIndex);
        }
    } else if (algorithm == COLUMNS) {
    }
}

double workflow() {
    if (MPI::processID == MPI::MAIN_PROCESS) {
        init();
    }
    MPI_Barrier(MPI_COMM_WORLD);  // Все потоки дождутся инициализации на главном потоке

    auto start = std::chrono::high_resolution_clock::now();
    if (MPI::processID == MPI::MAIN_PROCESS) {
        mainRoutine();
    } else {
        coRoutine();
    }
    MPI_Barrier(MPI_COMM_WORLD);  // Все потоки дождутся выполнения

    auto stop = std::chrono::high_resolution_clock::now();
#ifdef DEBUG
    if (MPI::processID == MPI::MAIN_PROCESS) {
        std::cout << result;
    }
#endif

    if (MPI::processID == MPI::MAIN_PROCESS) {
        std::chrono::duration<double, std::milli> ms = stop - start;
        return ms.count();
    }
    return -1;
}

int main(int argc, char** argv) {
    // Настройки
    srand(time(0));
    std::cout.precision(2);
    N = atoi(argv[1]);
    algorithm = (ALGORITHM)atoi(argv[2]);

    // Запуск
    double execution_time = MPI::run(workflow, argc, argv);

    if (execution_time != -1) {
        std::ofstream file("time.txt");
        file << execution_time;
        file.close();
    }

    return 0;
}
