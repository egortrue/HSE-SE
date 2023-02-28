import os
import time
import matplotlib.pyplot as plt
import numpy as np

PYTHON_BIN = "python3"
C_BIN = "build/app"

steps = 100
particles = (50, 250, 500)
threads = (1, 2, 4, 8, 16, 32)

values = {i: [] for i in threads}
os.system("mkdir data")
for particles_count in particles:
    for threads_count in threads:
        print(f"Start {particles_count} particles for {threads_count} threads")
        os.system(
            f"{PYTHON_BIN} generate_data.py {particles_count} {steps} data/input_{particles_count}_{steps}")
        start_time = time.time()
        os.system(
            f"{C_BIN} data/input_{particles_count}_{steps} data/output_{particles_count}_{steps} {threads_count}")
        execute_time = time.time() - start_time
        values[threads_count].append(execute_time)
        print(f"Result = {execute_time}")
os.system("rm -rf data")

x = np.arange(len(particles))
fig, ax = plt.subplots(constrained_layout=True)
width = 0.15
multiplier = 0
for attribute, measurement in values.items():
    offset = width * multiplier
    rects = ax.bar(x + offset, measurement, width, label=attribute)
    multiplier += 1

ax.set_title(f'Скорость выполнения для {steps} шагов')
ax.set_xlabel('Кол-во частиц')
ax.set_ylabel('Время выполнения (сек.)')
ax.set_xticks(np.arange(len(particles)) + width *
              (len(threads) / 2 - 0.5), map(str, particles))
ax.legend(loc='upper left', ncols=len(
    threads) / 2, title="Кол-во потоков")

plt.savefig("figure.png")
plt.show()
