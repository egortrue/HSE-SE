# https://pythonworld.ru/osnovy/peregruzka-operatorov.html
import math

class Fraction:

    def reducing(self):
        div = math.gcd(self.numerator, self.denominator)
        self.numerator = int(self.numerator / div)
        self.denominator = int(self.denominator / div)

    def toFraction(self, other):
        if type(self) != type(other):
            if type(other) == int:
                other = Fraction(other, 1)
            elif type(other) == float:
                after_dot = str(other).split('.')[1]
                other = Fraction(int(other * (10 ** len(after_dot))), 10 ** len(after_dot))
        return other

    def __init__(self, numerator: int, denominator: int):
        if denominator == 0:
            raise ZeroDivisionError
        self.numerator = numerator
        self.denominator = denominator
        self.reducing()

    def __str__(self):
        return f"{self.numerator}/{self.denominator}"

    def __float__(self):
        return self.numerator / self.denominator

    def __int__(self):
        return int(float(self))

    def __round__(self, n=None):
        return Fraction(1, 1) * round(float(self), n)

##################################################################

    def __add__(self, other):
        other = self.toFraction(other)
        return Fraction(self.numerator * other.denominator + other.numerator * self.denominator,
                        self.denominator * other.denominator)

    def __radd__(self, other):
        return self + other

    def __iadd__(self, other):
        other = self.toFraction(other)
        self.numerator = self.numerator * other.denominator + other.numerator * self.denominator
        self.denominator *= other.denominator
        return self

##################################################################

    def __sub__(self, other):
        other = self.toFraction(other)
        return Fraction(self.numerator * other.denominator - other.numerator * self.denominator,
                        self.denominator * other.denominator)

    def __rsub__(self, other):
        other = self.toFraction(other)
        return Fraction(other.numerator * self.denominator - self.numerator * other.denominator,
                        self.denominator * other.denominator)

    def __isub__(self, other):
        other = self.toFraction(other)
        self.numerator = self.numerator * other.denominator - other.numerator * self.denominator
        self.denominator *= other.denominator
        return self

##################################################################

    def __mul__(self, other):
        other = self.toFraction(other)
        return Fraction(self.numerator * other.numerator, self.denominator * other.denominator)

    def __rmul__(self, other):
        return self * other

    def __imul__(self, other):
        other = self.toFraction(other)
        self.numerator *= other.numerator
        self.denominator *= other.denominator
        return self

##################################################################

    def __truediv__(self, other):
        other = self.toFraction(other)
        return Fraction(self.numerator * other.denominator, self.denominator * other.numerator)

    def __rtruediv__(self, other):
        other = self.toFraction(other)
        return Fraction(other.numerator * self.denominator, other.denominator * self.numerator)

    def __itruediv__(self, other):
        other = self.toFraction(other)
        self.numerator *= other.denominator
        self.denominator *= other.numerator
        return self

##################################################################

    def __pow__(self, power):
        return Fraction(self.numerator ** power, self.denominator ** power)

    def __ipow__(self, power):
        self.numerator **= power
        self.denominator **= power
        return self

##################################################################

    def __lt__(self, other):
        if type(other) == float:
            return float(self) < other
        else:
            other = self.toFraction(other)
            return self.numerator * other.denominator < other.numerator * self.denominator

    def __le__(self, other):
        if type(other) == float:
            return float(self) <= other
        else:
            other = self.toFraction(other)
            return self.numerator * other.denominator <= other.numerator * self.denominator

    def __eq__(self, other):
        if type(other) == float:
            return float(self) == other
        else:
            other = self.toFraction(other)
            return self.numerator * other.denominator == other.numerator * self.denominator

    def __ne__(self, other):
        if type(other) == float:
            return float(self) != other
        else:
            other = self.toFraction(other)
            return self.numerator * other.denominator != other.numerator * self.denominator

    def __gt__(self, other):
        if type(other) == float:
            return float(self) > other
        else:
            other = self.toFraction(other)
            return self.numerator * other.denominator > other.numerator * self.denominator

    def __ge__(self, other):
        if type(other) == float:
            return float(self) >= other
        else:
            other = self.toFraction(other)
            return self.numerator * other.denominator >= other.numerator * self.denominator


a = Fraction(22, 7)
print(f" 0) {a} | {float(a)} | {int(a)} | {round(a, 2)} | {float(round(a, 2))}")

a = Fraction(9, 21)
print(f" 1) {a == 9/21} -> {a} == {9/21}\n")

a = Fraction(3, 2)
print(f" 2) {a + 2.5} == {2.5 + a}")
print(f" 3) {a - 2.5} != {2.5 - a}")
print(f" 4) {a * 2.5} == {2.5 * a}")
print(f" 5) {a / 2.5} != {2.5 / a}\n")

a = Fraction(4, 2)
print(f" 7) {a < 2} -> {a} < 2")
print(f" 8) {a <= 2} -> {a} <= 2")
print(f" 9) {a == 2} -> {a} == 2")
print(f"10) {a != 2} -> {a} != 2")
print(f"11) {a > 2} -> {a} > 2")
print(f"12) {a >= 2} -> {a} >= 2")

