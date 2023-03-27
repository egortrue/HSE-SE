import os
import matplotlib.pyplot as plt

SOURCE = "src/main.cpp"
BINARY = "build/app"
TYPE = "block"

# Данные
processes = [1, 2, 4, 6, 8]
sizes = [10, 50, 100, 500, 1000, 1500, 2000]
data = []

os.system(f"mpic++ {SOURCE} -o {BINARY}")
for process in processes:
    values = []
    for size in sizes:
        print(f"Start {size} size for {process} threads")
        os.system(f"mpiexec -n {process} {BINARY} {size} {TYPE}")
        with open("time.txt") as file:
            time = float(file.readline()) / 1000
            values.append(time)
            print(time)
    data.append(values)

# Создание графика
fig, ax = plt.subplots()
for i, row in enumerate(data):
    ax.plot(sizes, row, marker='o',
            label=f'{processes[i]} поток/ов')

# Настройка графика
title = "???"
if TYPE == "row":
    title = "'Разбиение по строкам'"
if TYPE == "column":
    title = "'Разбиение по столбцам'"
if TYPE == "block":
    title = "'Разбиение по блокам'"
ax.set_title(f'Время выполнения перемножения матриц с помощью\n{title}')
ax.set_xlabel('Размер матриц (N)')
ax.set_ylabel('Время выполнения (сек)')
ax.legend()
ax.grid()

# Отображение графика
plt.savefig(f'{TYPE}.png', dpi=400)
plt.show()
