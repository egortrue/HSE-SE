def is_simple(a: int) -> bool:
    for i in range(2, a):
        if a % i == 0:
            return False
    return True


def found_simple_digits_to_n(n: int) -> list:
    result = []
    for i in range(2, n):
        if is_simple(i):
            result.append(i)
    return result


def found_all_divisors_of_number(a: int) -> list:
    divisors = []
    for i in range(2, a):
        if a % i == 0:
            divisors.append(i)
    return divisors


def nod(a, b):
    if a < 0 and b < 0:
        print("error values")
        return

    if a == 0 or b == 0:
        return max(a, b)

    return nod(b % a, a)


print(found_all_divisors_of_number(80), found_all_divisors_of_number(120))
print(nod(80, 120))
print(found_simple_digits_to_n(120))
