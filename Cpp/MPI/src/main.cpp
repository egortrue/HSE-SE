

#include "common.hpp"
#include "matrix.hpp"
#include "mpi.hpp"

enum ALGORITHM {
    ROW,
    COLUMN,
    BLOCK
} algorithm;

// Общие данные
int N;

// Данные главного потока
Matrix matrixA;
Matrix matrixB;
Matrix result;

// Инициализация - только на главном потоке
void init() {
    if (MPI::processID == MPI::MAIN_PROCESS) {
        matrixA = Matrix(N, N, Matrix::RANDOM);
        matrixB = Matrix(N, N, Matrix::RANDOM);
        result = Matrix(N, N, Matrix::ZERO);

#ifdef DEBUG
        std::cout << "Matrix A " << matrixA;
        std::cout << "Matrix B " << matrixB;
#endif
    }
}

void row_alogrithm() {
    //=====================================================================
    // Разделить матрицу A по строкам

    int base_rows = N / MPI::processCount;
    int extra_rows = N % MPI::processCount;
    int rows = base_rows;
    if (MPI::processID < extra_rows)
        rows++;
    Matrix local_submatrixA = Matrix(rows, N);

    // Количество и смещения для MPI_Scatterv
    std::vector<int> counts_row(MPI::processCount, base_rows * N);
    std::vector<int> positions_row(MPI::processCount, 0);
    for (int process = 0; process < MPI::processCount; ++process) {
        if (process < extra_rows)
            counts_row[process] += N;
        if (process > 0)
            positions_row[process] = positions_row[process - 1] + counts_row[process - 1];
    }
    MPI_Scatterv(matrixA.data, counts_row.data(), positions_row.data(), MPI_DOUBLE, local_submatrixA.data, rows * N, MPI_DOUBLE, MPI::MAIN_PROCESS, MPI_COMM_WORLD);
    local_submatrixA.writeDataToTransposed();

    //=====================================================================
    // Получение матрицы B

    Matrix local_matrixB = Matrix(N, N);
    if (MPI::processID == MPI::MAIN_PROCESS)
        local_matrixB = matrixB;
    MPI_Bcast(local_matrixB.data, N * N, MPI_DOUBLE, MPI::MAIN_PROCESS, MPI_COMM_WORLD);

    //=====================================================================
    // Локальное перемножение матриц

    Matrix local_result = local_submatrixA * local_matrixB;

#ifdef DEBUG
    std::cout << "Local Result " << local_result;
#endif

    //=====================================================================
    // Сложение результирующей матрицы

    // Собрать все локальные блоки матрицы C на главном процессе
    std::vector<int> counts_result = counts_row;
    std::vector<int> positions_result = positions_row;

    MPI_Gatherv(local_result.data, rows * N, MPI_DOUBLE, result.data, counts_result.data(), positions_result.data(), MPI_DOUBLE, MPI::MAIN_PROCESS, MPI_COMM_WORLD);
}

void column_algorithm() {
    //=====================================================================
    // Разделить матрицу A по столбцам

    int base_cols = N / MPI::processCount;
    int extra_cols = N % MPI::processCount;
    int cols = base_cols;
    if (MPI::processID < extra_cols)
        cols++;
    Matrix local_submatrixA = Matrix(N, cols);

    // Количество и смещения для MPI_Scatterv
    std::vector<int> counts_col(MPI::processCount, base_cols * N);
    std::vector<int> positions_col(MPI::processCount, 0);
    for (int process = 0; process < MPI::processCount; ++process) {
        if (process < extra_cols)
            counts_col[process] += N;
        if (process > 0)
            positions_col[process] = positions_col[process - 1] + counts_col[process - 1];
    }
    MPI_Scatterv(matrixA.data_transposed, counts_col.data(), positions_col.data(), MPI_DOUBLE, local_submatrixA.data_transposed, cols * N, MPI_DOUBLE, MPI::MAIN_PROCESS, MPI_COMM_WORLD);
    local_submatrixA.writeTransposedToData();

    //=====================================================================
    // Получение матрицы B

    Matrix local_matrixB = Matrix(N, N);
    if (MPI::processID == MPI::MAIN_PROCESS)
        local_matrixB = matrixB;
    MPI_Bcast(local_matrixB.data, N * N, MPI_DOUBLE, MPI::MAIN_PROCESS, MPI_COMM_WORLD);

    //=====================================================================
    // Локальное вычисление

    Matrix local_result = Matrix(N, N);
    int col_start;
    if (extra_cols == 0 || MPI::processID < extra_cols)
        col_start = MPI::processID * cols;
    else
        col_start = (base_cols + 1) * extra_cols + ((MPI::processID - extra_cols) * base_cols);

    for (int row = 0; row < N; ++row)
        for (int col = col_start; col < col_start + cols; ++col)
            for (int k = 0; k < N; ++k)
                local_result(row, col) += local_submatrixA(k, col - col_start) * local_matrixB(row, k);

#ifdef DEBUG
    std::cout << "Local Result " << local_result;
#endif

    //=====================================================================
    // Сложение результирующей матрицы

    MPI_Reduce(local_result.data, result.data, N * N, MPI_DOUBLE, MPI_SUM, MPI::MAIN_PROCESS, MPI_COMM_WORLD);
}

double workflow() {
    if (MPI::processID == MPI::MAIN_PROCESS) {
        init();
    }
    MPI_Barrier(MPI_COMM_WORLD);  // Все потоки дождутся инициализации на главном потоке

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    if (algorithm == ROW)
        row_alogrithm();
    else if (algorithm == COLUMN)
        column_algorithm();
    else if (algorithm == BLOCK) {
    }
#ifdef DEBUG
    if (MPI::processID == MPI::MAIN_PROCESS) {
        std::cout << "Global Result " << result;
    }
#endif
    MPI_Barrier(MPI_COMM_WORLD);  // Все потоки дождутся выполнения
    std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

    if (MPI::processID == MPI::MAIN_PROCESS) {
        std::chrono::duration<double, std::milli> ms = stop - start;
        return ms.count();
    }
    return -1;
}

int main(int argc, char** argv) {
    // Параметры
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <matrix_size> "
                  << "<algo_name>" << std::endl;
        return 1;
    }
    N = std::stoi(argv[1]);
    std::string algo = std::string(argv[2]);
    if (algo == "row")
        algorithm = ROW;
    else if (algo == "column")
        algorithm = COLUMN;
    else if (algo == "block")
        algorithm = BLOCK;
    else {
        std::cerr << "Algo name could be either row, column or block." << std::endl;
        return 1;
    }

    // Настройки
    srand(time(0));
    std::cout.precision(2);

    // Запуск
    double execution_time = MPI::run(workflow, argc, argv);

    if (execution_time != -1) {
        std::ofstream file("time.txt");
        file << execution_time;
        file.close();
    }

    return 0;
}
