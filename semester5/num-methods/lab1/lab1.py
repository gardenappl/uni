import math
from sympy import *

init_printing()

x = symbols("x")
y = E**-x + x**2 - 2

print("Function: ")
pprint(y)
print("1st derivative: ")
pprint(diff(y))
print("2nd derivative: ")
pprint(diff(y, x, 2))
plot(y, (x, -2, 2))


x_0 = float(input("Enter x_0: "))
epsilon = float(input("Choose precision: "))


f = lambdify("x", y)
f_deriv = lambdify("x", diff(y))


print("\nNewton's method")

x_n = x_0
steps = 0
while abs(f(x_n)) > epsilon: 
    x_n = x_n - f(x_n) / f_deriv(x_n)
    print("f(%f) = %f" % (x_n, f(x_n)))
    steps += 1

print("Answer: %f, steps: %d" % (x_n, steps))
input("Press Enter to continue.")


print("\nModified Newton's method")

x_n = x_0
f_deriv0 = f_deriv(x_0)
steps = 0
while abs(f(x_n)) > epsilon: 
    x_n = x_n - f(x_n) / f_deriv0
    print("f(%f) = %f" % (x_n, f(x_n)))
    steps += 1

print("Answer: %f, steps: %d" % (x_n, steps))
