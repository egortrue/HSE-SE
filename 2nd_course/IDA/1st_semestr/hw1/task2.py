def to_1_with_rule(n):
    count = 0
    while n != 1:
        if n % 2 == 0:
            n /= 2
        else:
            n = 3 * n + 1
        count += 1
    return count


dict_of_numbers = dict()
for number in range(1, 1000):
    dict_of_numbers[number] = to_1_with_rule(number)
number_with_max_count = 1
max_count = 0

for number, count in dict_of_numbers.items():
    if count > max_count:
        number_with_max_count = number
        max_count = count

print(f'Number: {number_with_max_count}, Count: {max_count}')  # >> 871 and 178
