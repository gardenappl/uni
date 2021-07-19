import numpy as np

h = 0.2

c = np.zeros(4)
c[0] = 0.1  # estimate
c[1] = 0.3
c[2] = 0.1  # estimate
c[3] = 0.12
m = np.zeros(3)
m[0] = 9.   # estimate
m[1] = 28.
m[2] = 18.

real_y = np.loadtxt('y3.txt')


def get_A(m, c):
    A = np.zeros((6, 6))
    A[0, 1] = 1.
    A[1, 0] = -(c[1]+c[0])/m[0]
    A[1, 2] = c[1]/m[0]
    A[2, 3] = 1.
    A[3, 0] = c[1]/m[1]
    A[3, 2] = -(c[1]+c[2])/m[1]
    A[3, 4] = c[2]/m[1]
    A[4, 5] = 1.
    A[5, 2] = c[2]/m[2]
    A[5, 4] = -(c[3]+c[2])/m[2]
    return A


def get_U_deriv(A, U, y, m, c):
    dAy_db = np.zeros((6, 3))
    dAy_db[1, 0] = -y[0]/m[0]
    dAy_db[1, 2] = (y[0] * (c[1]+c[0]) - y[2]*c[1])/(m[0]**2)
    dAy_db[3, 1] = (-y[2] + y[4])/m[1]
    dAy_db[5, 1] = (y[2] - y[4])/m[2]
    return A @ U + dAy_db


def runge_kutta_step(y, h, f):
    k_1 = h * f(y)
    k_2 = h * f(y + k_1/2.)
    k_3 = h * f(y + k_2/2.)
    k_4 = h * f(y + k_3)
    return y + (k_1 + 2.*k_2 + 2.*k_3 + k_4)/6.


while True:
    A = get_A(m, c)
    y = real_y[:, [0]]    
    U = np.zeros((6, 3))

    delta_y = np.zeros((6, 1))

    # Integrals for delta_beta
    UU_integral = np.zeros((3, 3))
    Uy_integral = np.zeros((3, 1))

    # Identification quality
    Ib = 0

    for i in range(1, real_y.shape[1]):
        U_new = runge_kutta_step(U, h, lambda U: get_U_deriv(A, U, y, m, c))
        y_new = runge_kutta_step(y, h, lambda y: A @ y)
        delta_y_new = real_y[:, [i]] - y_new

        # Add to Riemann integrals for delta_b
        UU_integral = UU_integral + h * (np.transpose(U) @ U + np.transpose(U_new) @ U_new) / 2.
        Uy_integral = Uy_integral + h * (np.transpose(U) @ delta_y + np.transpose(U_new) @ delta_y_new) / 2.

        # Add to Riemann integral for dentification quality
        Ib = Ib + h * (np.transpose(delta_y) @ delta_y + np.transpose(delta_y_new) @ delta_y) / 2.

        # Next step
        U = U_new
        y = y_new
        delta_y = delta_y_new

    # set new parameters
    delta_b = np.linalg.inv(UU_integral) @ Uy_integral
    c[0] += delta_b[0]
    c[2] += delta_b[1]
    m[0] += delta_b[2]

    print("I(beta)", Ib)

    if Ib < 1e-8:
        print("c", c)
        print("m", m)
        break
