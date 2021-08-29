from random import random

def flip(chance: float, count=1):
    if count == 1:
        return 1 if random() <= chance else 0
    return [flip(chance) for x in range(count)]

print(flip(0.5, 10))

def exp(chance, exp_count, count=1):
    return [sum(flip(chance, count)) for x in range(exp_count)]

print(exp(0.7, 5, 10))
