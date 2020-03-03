import time


def calculate(e1, e2, e3, x):
    y = -1
    e4 = e3 + (e2 - e1)
    if e1 <= x < e2:
        y = (x - e1) / (e2 - e1)
    elif e2 <= x <= e3:
        y = 1
    elif e3 < x <= e4:
        y = (e4 - x) / (e4 - e3)
    return y


def fuzzication(epsilon: 'array', x):
    e1 = -1 * epsilon[0]
    e2 = 0
    e3 = 0
    y_zero = calculate(e1, e2, e3, x)

    e1 = 0
    e2 = epsilon[1]
    e3 = epsilon[2]
    y_small_pos = calculate(e1, e2, e3, x)

    e1 = -1 * (epsilon[1] + epsilon[2])
    e2 = -1 * epsilon[2]
    e3 = -1 * (epsilon[1])
    y_small_neg = calculate(e1, e2, e3, x)

    return y_small_neg, y_zero, y_small_pos


def rules(theta, omega, e_theta: 'array', e_omega: 'array'):
    y_theta = fuzzication(e_theta, theta)
    y_omega = fuzzication(e_omega, omega)

    dictionary = {'00': 2, '01': 1, '02': 0, '10': 1,
                  '11': 0, '12': -1, '20': 0, '21': -1, '22': -2}
    y_curr = []
    for id1, val1 in enumerate(y_theta):
        for id2, val2 in enumerate(y_omega):
            if val1 == -1 or val2 == -1:
                continue
            else:
                curr_belongingness = min(val1, val2)
                curr_id = dictionary[str(id1) + str(id2)]
                y_curr.append([curr_belongingness, curr_id])
    return y_curr


def defuzzify(epsilon: 'array of epsilon for curr', y):
    e1 = epsilon[0]
    e2 = epsilon[1]
    e3 = epsilon[2]
    e4 = e3 + (e2 - e1)
    x_centroid = (e1 + e4) / 2
    base1 = e4 - e1
    base2 = base1 - 2*(e2 - e1)
    area = 0.5 * (base1 + base2) * y
    return x_centroid, area


def compute(theta, omega, e_theta, e_omega, e_curr):
    curr_list = rules(theta, omega, e_theta, e_omega)

    dictionary = {0: [-1 * e_curr[0], 0, 0],
                  1: [0, e_curr[1], e_curr[2]],
                  2: [e_curr[3], e_curr[4], e_curr[5]],
                  -1: [-1 * (e_curr[2] + e_curr[1]), -1 * e_curr[2], -1 * e_curr[1]],
                  - 2: [-1 * (e_curr[5] + e_curr[4] - e_curr[3]), -1 * e_curr[5], -1 * e_curr[4]]
                  }

    total_area = 0
    weighted_area = 0
    for tup in curr_list:
        centroid, area = defuzzify(dictionary[tup[1]], tup[0])
        weighted_area += (centroid * area)
        total_area += area
    return weighted_area / total_area
