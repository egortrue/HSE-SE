# https://pythonworld.ru/osnovy/peregruzka-operatorov.html
import math

class Fraction:

    def reducing(self):
        div = math.gcd(self.numerator, self.denominator)
        self.numerator = int(self.numerator / div)
        self.denominator = int(self.denominator / div)

    def toFraction(self, other):
        if type(self) != type(other):
            other = Fraction(other, 1)
        return other

    def __init__(self, numerator, denominator):
        if denominator == 0:
            raise ZeroDivisionError
        self.numerator = numerator
        self.denominator = denominator
        self.reducing()

    def __str__(self):
        return f"{self.numerator}/{self.denominator}"

##################################################################

    def __add__(self, other):
        other = self.toFraction(other)
        return Fraction(self.numerator * other.denominator + other.numerator * self.denominator,
                        self.denominator * other.denominator)

    def __radd__(self, other):
        return self + other

##################################################################

    def __sub__(self, other):
        other = self.toFraction(other)
        return Fraction(self.numerator * other.denominator - other.numerator * self.denominator,
                        self.denominator * other.denominator)

    def __rsub__(self, other):
        other = self.toFraction(other)
        return Fraction(other.numerator * self.denominator - self.numerator * other.denominator,
                        self.denominator * other.denominator)

##################################################################

    def __mul__(self, other):
        other = self.toFraction(other)
        return Fraction(self.numerator * other.numerator, self.denominator * other.denominator)

    def __rmul__(self, other):
        return self * other

##################################################################

    def __truediv__(self, other):
        other = self.toFraction(other)
        return Fraction(self.numerator * other.denominator, self.denominator * other.numerator)

    def __rtruediv__(self, other):
        other = self.toFraction(other)
        return Fraction(other.numerator * self.denominator, other.denominator * self.numerator)

##################################################################

    def __pow__(self, power, modulo=None):
        return Fraction(self.numerator ** power, self.denominator ** power)

##################################################################

    def __lt__(self, other):
        other = self.toFraction(other)
        return True if (self.numerator * other.denominator < other.numerator * self.denominator) else False

    def __le__(self, other):
        other = self.toFraction(other)
        return True if (self.numerator * other.denominator <= other.numerator * self.denominator) else False

    def __eq__(self, other):
        other = self.toFraction(other)
        return True if (self.numerator * other.denominator == other.numerator * self.denominator) else False

    def __ne__(self, other):
        other = self.toFraction(other)
        return True if (self.numerator * other.denominator != other.numerator * self.denominator) else False

    def __gt__(self, other):
        other = self.toFraction(other)
        return True if (self.numerator * other.denominator > other.numerator * self.denominator) else False

    def __ge__(self, other):
        other = self.toFraction(other)
        return True if (self.numerator * other.denominator >= other.numerator * self.denominator) else False


print(" 1)", Fraction(9, 21), '\n')

a = Fraction(3, 2)
print(" 2)", a + 2, " == ", 2 + a)
print(" 3)", a - 2, " != ", 2 - a)
print(" 4)", a * 2, " == ", 2 * a)
print(" 5)", a / 2, " != ", 2 / a)
print(" 6)", a ** 3, '\n')

a = Fraction(4, 2)
print(f" 7) {a < 2} -> {a} < 2")
print(f" 8) {a <= 2} -> {a} <= 2")
print(f" 9) {a == 2} -> {a} == 2")
print(f"10) {a != 2} -> {a} != 2")
print(f"11) {a > 2} -> {a} > 2")
print(f"12) {a >= 2} -> {a} >= 2")

