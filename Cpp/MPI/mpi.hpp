#pragma once

#include "common.hpp"

namespace MPI {

const int MAIN_PROCESS = 0;

const int TAG_STOP = 0;

const int TAG_ROW = 1;
const int TAG_ROW_REQUEST = 2;
const int TAG_ROW_NUMBER = 3;
const int TAG_COL = 4;
const int TAG_COL_REQUEST = 5;
const int TAG_COL_NUMBER = 6;
const int TAG_VALUE = 7;

int processID;
int processCount;
int requesterID = 0;
int stopFlag = false;

MPI_Request NULL_REQUEST;

double run(double (*workflow)(), int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    double execution_time = workflow();
    MPI_Finalize();
    return execution_time;
}

// Отправить всем процессам флаг остановки
void stop() {
    MPI::stopFlag = true;
    for (int process = 0; process < MPI::processCount; ++process)
        if (process != MPI::MAIN_PROCESS)
            MPI_Isend(&MPI::stopFlag, 1, MPI_INT, process, MPI::TAG_STOP, MPI_COMM_WORLD, &MPI::NULL_REQUEST);
}

// Попробовать получить сигнал стоп от главного процесса
void probeStop() {
    MPI_Request recieveStop;
    int isStopMessageCreated = 0;
    MPI_Iprobe(MPI::MAIN_PROCESS, MPI::TAG_STOP, MPI_COMM_WORLD, &isStopMessageCreated, MPI_STATUS_IGNORE);
    if (isStopMessageCreated)
        MPI::stopFlag = true;
}

bool isStopped() {
    if (MPI::stopFlag == false) probeStop();
    return MPI::stopFlag;
}

// Отправить данные, как только они запрошены
void sendVector(int index, Matrix& matrix, int tag) {
    double* data = tag == MPI::TAG_ROW ? matrix.data[index] : matrix.data_transposed[index];
    int request_tag = tag == MPI::TAG_ROW ? MPI::TAG_ROW_REQUEST : MPI::TAG_COL_REQUEST;
    int data_tag = tag == MPI::TAG_ROW ? MPI::TAG_ROW : MPI::TAG_COL;
    int index_tag = tag == MPI::TAG_ROW ? MPI::TAG_ROW_NUMBER : MPI::TAG_COL_NUMBER;

    MPI_Recv(&MPI::requesterID, 1, MPI_INT, MPI_ANY_SOURCE, request_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Rsend(data, matrix.cols, MPI_DOUBLE, MPI::requesterID, data_tag, MPI_COMM_WORLD);
    MPI_Rsend(&index, 1, MPI_INT, MPI::requesterID, index_tag, MPI_COMM_WORLD);
}

void getValue(Matrix& matrix) {
    int row, col;
    MPI_Recv(&row, 1, MPI_INT, MPI_ANY_SOURCE, MPI::TAG_VALUE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&col, 1, MPI_INT, MPI_ANY_SOURCE, MPI::TAG_VALUE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&matrix.data[row][col], 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI::TAG_VALUE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void sendValue(double value, int row, int col) {
    MPI_Ssend(&row, 1, MPI_INT, MPI::MAIN_PROCESS, MPI::TAG_VALUE, MPI_COMM_WORLD);
    MPI_Ssend(&col, 1, MPI_INT, MPI::MAIN_PROCESS, MPI::TAG_VALUE, MPI_COMM_WORLD);
    MPI_Ssend(&value, 1, MPI_DOUBLE, MPI::MAIN_PROCESS, MPI::TAG_VALUE, MPI_COMM_WORLD);
}

// Получить вектор и его номер в матрице
void getVector(std::vector<double>& vector, int& index, int tag) {
    if (isStopped()) return;

    int data_tag = tag == MPI::TAG_ROW ? MPI::TAG_ROW : MPI::TAG_COL;
    int request_tag = tag == MPI::TAG_ROW ? MPI::TAG_ROW_REQUEST : MPI::TAG_COL_REQUEST;
    int index_tag = tag == MPI::TAG_ROW ? MPI::TAG_ROW_NUMBER : MPI::TAG_COL_NUMBER;
    MPI_Request recieveVector;
    MPI_Request recieveVectorIndex;

    // Инициируем прием данных
    MPI_Irecv(vector.data(), vector.size(), MPI_DOUBLE, MPI::MAIN_PROCESS, data_tag, MPI_COMM_WORLD, &recieveVector);
    MPI_Irecv(&index, 1, MPI_INT, MPI::MAIN_PROCESS, index_tag, MPI_COMM_WORLD, &recieveVectorIndex);

    // Скажем главному потоку, что готовы принимать данные
    MPI_Send(&MPI::processID, 1, MPI_INT, MPI::MAIN_PROCESS, request_tag, MPI_COMM_WORLD);

    // Ждем данные
    int recieveRowComplete = 0;
    int recieveRowIndexComplete = 0;

    while (!(recieveRowComplete && recieveRowIndexComplete) && !isStopped()) {
        MPI_Test(&recieveVector, &recieveRowComplete, MPI_STATUS_IGNORE);
        MPI_Test(&recieveVectorIndex, &recieveRowIndexComplete, MPI_STATUS_IGNORE);
    }

    // Завершение приема
    if (MPI::stopFlag) {
        if (!recieveRowComplete) MPI_Cancel(&recieveVector);
        if (!recieveRowIndexComplete) MPI_Cancel(&recieveVectorIndex);
    }
}

}  // namespace MPI