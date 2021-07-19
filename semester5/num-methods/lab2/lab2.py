import numpy as np


a = np.array([
    (1, 3, 5, 7),
    (3, 5, 7, 1),
    (5, 7, 1, 3),
    (7, 1, 1, 5)
], dtype=np.float64)

b = np.array([12, 0, 4, 16], dtype=np.float64)


def solve_gauss_m(a, b):
    a = a.copy()
    b = b.copy()

    for i in range(0, a.shape[0]):
        main_row_index = i + np.argmax(a[range(i, a.shape[1]), i])
        main_row_el = a[main_row_index, i]

        # Swap main row with ith row
        if main_row_index != i:
            a[[main_row_index, i], :] = a[[i, main_row_index], :]
            b[[main_row_index, i]] = b[[i, main_row_index]]
        # print(a)
        # print(b)

        a[i, :] /= main_row_el
        b[i] /= main_row_el

        for row_index in range(i + 1, a.shape[1]):
            multiplier = a[row_index, i]
            a[row_index, :] -= a[i, :] * multiplier
            b[row_index] -= b[i] * multiplier

        # print(a)
        # print(b)

    x = np.empty_like(b)

    for i in range(0, a.shape[0]):
        row_index = x.size - 1 - i
        x[row_index] = b[row_index]
        for j in range(0, i):
            x[row_index] -= a[row_index, a.shape[1]-1 - j] * x[x.size-1 - j]

    return x


MAX_STEPS = 100


def solve_seidel(a, b, max_steps=MAX_STEPS, epsilon=1e-04):
    x = np.zeros_like(b)

    for step in range(max_steps):
        x_new = np.zeros_like(x)

        for i in range(x.size):
            for j in range(0, i):
                x_new[i] -= a[i, j] / a[i, i] * x_new[j]
            for j in range(i + 1, x.size):
                x_new[i] -= a[i, j] / a[i, i] * x[j]
            x_new[i] += b[i] / a[i, i]

        print("Step", step, ":", x_new)

        if np.allclose(x, x_new, atol=epsilon, rtol=0):
            return x_new

        x = x_new

    return x_new


print("Gauss (with selection of main element):", solve_gauss_m(a, b))
print("numpy.linalg.solve:", np.linalg.solve(a, b))

a = np.array([
    [3, -1, 1],
    [-1, 2, 0.5],
    [1, 0.5, 3]
], dtype=np.float64)
b = np.array([1, 1.75, 2.5], dtype=np.float64)

print("Seidel:", solve_seidel(a, b, epsilon=0.0001))
print("numpy.linalg.solve:", np.linalg.solve(a, b))
