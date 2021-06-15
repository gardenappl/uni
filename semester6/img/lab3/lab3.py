from PIL import Image
import sys
import math
import numpy as np

from sympy import *
init_printing(use_unicode=True)


if len(sys.argv) <= 2:
    print("Usage: lab2.py [input] [output]")
    exit(1)


image = Image.open(sys.argv[1]).convert("L")
inputArr = np.asarray(image)
outputArr = np.zeros_like(inputArr)

print("Starting...")

x, y, a, b, c, alpha, beta, gamma = symbols('x y a b c \u03b1 \u03b2 \u03b3')
f = Function('f')

# Squared residual
f_a = a*x**2 + b*y**2 + c*x*y + alpha*x + beta*y + gamma
err_sq = 0
for i in [0, 1, 2]:
    for j in [0, 1, 2]:
        err_sq += (f_a.subs([(x, x + i), (y, y + j)]) - f(x + i, x + j)) ** 2


# Replace f(x, y) with f(0, 0)
err_minimization = []
for var in [a, b, c, alpha, beta, gamma]:
    err_minimization.append(diff(err_sq, var).subs([(x, 0), (y, 0)]))
    print("0 =", err_minimization[-1])
    print()

# Solve system of linear equations
solution = solve(err_minimization, [a, b, c, alpha, beta, gamma])

# Turn SymPy expressions into fast NumPy functions

offset_x = 0
offset_y = 0
def get_pixel_with_offset(x, y):
    return inputArr[offset_x + x, offset_y + y]

X = {}
for var in solution:
    X[var] = lambdify([], solution[var], [{'f': get_pixel_with_offset}, 'numpy'])
    print(var, '=', solution[var])
    print()


for x in range(inputArr.shape[0] - 2):
    for y in range(inputArr.shape[1] - 2):
        offset_x = x
        offset_y = y
        a_val = X[a]()
        b_val = X[b]()
        c_val = X[c]()
        alpha_val = X[alpha]()
        beta_val = X[beta]()
        outputArr[x, y] = math.sqrt((2*a_val + c_val + alpha_val) ** 2 + (2*b_val + c_val + beta_val) ** 2)

Image.fromarray(outputArr, 'L').save(sys.argv[2])
