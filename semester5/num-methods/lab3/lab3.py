import numpy as np
import math


aa = 1.5
a = np.array([
    [5.18+aa, 1.12,    0.95,    1.32,    0.83],
    [1.12,    4.28-aa, 2.12,    0.57,    0.91],
    [0.95,    2.12,    6.13+aa, 1.29,    1.57],
    [1.32,    0.57,    1.29,    4.57-aa, 1.25],
    [0.83,    0.91,    1.57,    1.25,    5.21+aa]
])

epsilon = 10e-4


print("------------")
print("   Task 1   ")
print("------------")


# Find max abs eigenvalue and corresponding eigenvector
def power_iteration(a, epsilon):
    x_next = np.ones(a.shape[0])
    mu = np.nan
    mu_next = np.nan

    step = 0

    while not abs(mu_next - mu) < epsilon:
        step += 1
        x = x_next
        mu = mu_next

        x /= np.linalg.norm(x)
        x_next = a @ x
        mu_next = x_next @ x
        #print(f'x = {x}, x_next = {x_next}')

    print(f"(Steps: {step})")

    return (mu_next, x_next / np.linalg.norm(x_next))


# Alias
get_eigen_max = power_iteration


# Find max abs eigenvalue, knowing min abs eigenvalue
# a must be symmetrical
def get_eigenvalue_min(a, epsilon, eigenvalue_max=None):
    if eigenvalue_max:
        b = np.eye(a.shape[0]) * eigenvalue_max - a
        return eigenvalue_max - power_iteration(b, epsilon)[0]
    else:
        a_max = np.max(a)
        b = a_max * np.eye(a.shape[0]) - a
        return a_max - power_iteration(b, epsilon)[0]


# Find max abs eigenvalue and corresponding eigenvector
def get_eigen_min(a, epsilon):
    a_inv = np.linalg.inv(a)
    eigenvalue_min = 1 / get_eigen_max(a_inv, epsilon)[0]

    x = np.ones(a.shape[0])
    
    while True:
        x_next = a_inv @ x * eigenvalue_min

        if (np.allclose(x, x_next, atol=epsilon)):
            return (eigenvalue_min, x_next / np.linalg.norm(x_next))

        x = x_next 


print("Max. eigen. (power iteration): ")
eigenvalue_max, eigenvector_max = get_eigen_max(a, epsilon)
print(eigenvalue_max, eigenvector_max)

print("Min. eigenvalue (power iteration, from max. eigenvalue): ")
print(get_eigenvalue_min(a, epsilon, eigenvalue_max))

print("Min. eigenvalue (power iteration): ")
print(get_eigenvalue_min(a, epsilon))

print("Min. eigen. (power iteration on inverse): ")
eigenvalue_min, eigenvector_min = get_eigen_min(a, epsilon)
print(eigenvalue_min, eigenvector_min)


print("------------")
print("   Task 2   ")
print("------------")


def jacobi_rotation_method(a, epsilon):
    u_prod = np.eye(a.shape[0])

    # Quadratic sum of non-diagonal elements
    t = 0
    for i in range(a.shape[0]):
        for j in range(i, a.shape[1]):
            if i != j:
                t += 2 * (a[i, j] ** 2)

    step = 0

    while t > epsilon:
        step += 1
        # Find max. abs. non-diagonal element
        max_nd = 0

        for i in range(a.shape[0]):
            for j in range(a.shape[1]):
                if i != j and abs(a[i, j]) > abs(max_nd):
                    max_nd = a[i, j]
                    i_max_nd = i
                    j_max_nd = j

        
        if abs(a[i_max_nd, i_max_nd] - a[j_max_nd, j_max_nd]) < epsilon:
            phi = math.pi / 4
        else:
            phi = math.atan(2 * max_nd / 
                    (a[i_max_nd, i_max_nd] - a[j_max_nd, j_max_nd])) / 2

        u = np.eye(a.shape[0])
        u[i_max_nd, i_max_nd] = math.cos(phi)
        u[i_max_nd, j_max_nd] = math.sin(phi)
        u[j_max_nd, i_max_nd] = -math.sin(phi)
        u[j_max_nd, j_max_nd] = math.cos(phi)
        t -= 2 * (max_nd ** 2)
        #print(t)

        #a = u.transpose() @ a @ u
        a = u @ a @ u.transpose()
        #u_prod = u_prod @ u
        u_prod = u_prod @ u.transpose()

    print(f"(Steps: {step})")


    return (np.diag(a), u_prod)

get_eigen = jacobi_rotation_method

print("Eigenvectors (Jacobi rotations):")
eigenvalues, eigenvectors = get_eigen(a, epsilon)
print(eigenvalues)
print(eigenvectors)

print("------------")
print("    NumPy   ")
print("------------")

numpy_eigenvalues, numpy_eigenvectors = np.linalg.eig(a)
print("np.linalg.eig: ")
print(numpy_eigenvalues)
print(numpy_eigenvectors)
