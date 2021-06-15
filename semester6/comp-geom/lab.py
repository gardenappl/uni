import math
import matplotlib.pyplot as plt
import numpy as np


def get_angle(p1, p2):
    return math.atan2(p2[1] - p1[1], p2[0] - p1[0])


def star_shaped_poly(points):
    center = [sum(coord) / len(coord) for coord in zip(*points)]
    print("Center:", center)

    poly = sorted(points, key=lambda p: get_angle(center, p))

    return np.array(poly)


_quadratic_curve_m = np.array([
    [0.5, -1, 0.5],
    [-1, 1, 0.5],
    [0.5, 0, 0]
])



def uniform_quadratic_b_spline(poly):
    def get_quadratic_curve(p_triple):
        # t is [0, 1]
        return lambda t: p_triple.T @ _quadratic_curve_m @ np.array([t * t, t, 1])

    print(poly.shape[0])
    return [ get_quadratic_curve(poly[[i - 2, i - 1, i]]) for i in range(poly.shape[0]) ]
        

points = []

with open('points') as points_file:
    for line in points_file:
        points.append(tuple([int(num) for num in line.split()]))

points_x, points_y = zip(*points)

plt.plot(points_x, points_y, 'ro')
plt.grid()

poly = star_shaped_poly(points)

print("Points:", points)
print("Polygon: ", poly)


# Draw polygon
prev_point = None
for point in poly:
    if prev_point is not None:
        plt.plot([prev_point[0], point[0]], [prev_point[1], point[1]], 'r')
    else:
        # connect last point to first
        plt.plot([poly[-1][0], poly[0][0]], [poly[-1][1], poly[0][1]], 'r')
    prev_point = point


spline = uniform_quadratic_b_spline(poly)
n = 50
for curve in spline:
    points = np.array([curve(t) for t in np.linspace(0, 1, n)])
    plt.plot(points[:,0], points[:,1])


plt.show()
