from dataclasses import dataclass
from itertools import product

import numpy as np
from aoc_util import read
from scipy.optimize import linprog


@dataclass
class Machine:
    buttons: list[tuple[int]]
    joltages: list[int]


def parse(line: str):
    tokens = line.split(" ")
    _, buttons_list, joltages_str = tokens[0], tokens[1:-1], tokens[-1]
    return Machine(
        buttons=[
            button if isinstance(button, tuple) else (button,)
            for button in list(map(eval, buttons_list))
        ],
        joltages=list(map(int, joltages_str[1:-1].split(","))),
    )


def fewest_button_presses(machine: Machine) -> int:
    # 0. define system of lin eq
    M, N = len(machine.joltages), len(machine.buttons)
    A = np.zeros((M, N), dtype=int)
    B = np.zeros((M,), dtype=int)
    for i, joltage in enumerate(machine.joltages):
        B[i] = joltage
        for j, button in enumerate(machine.buttons):
            A[i][j] = i in button

    # 1. find possible button press combinations by solving system of lin eq
    out = sum(linprog(np.ones((N,)), A_eq=A, b_eq=B, integrality=1).x)
    return int(out)
    # m = max(np.max(A), np.max(B)) + 1
    # min_presses = np.inf
    # for coefs in product(*[range(0, m) for _ in range(N)]):
    #     X = np.array(coefs).T
    #     if np.allclose(A @ X, B):
    #         min_presses = min(min_presses, sum(coefs))
    # return min_presses


# expressing problem as a system of linear equations:
# [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
#   x4 + x5 = 3
#   x1 + x5 = 5
#   x2 + x3 + x4 = 4
#   x0 + x1 + x3 = 7
#
# [...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
#   x0 + x2 + x3 = 7
#   x3 + x4 = 5
#   x0 + x1 + x3 + x4 = 12
#   x0 + x1 + x4 = 7
#   x0 + x2 + x4 = 2
#
# [.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}
#   x0 + x1 + x2 = 10
#   x0 + x2 + x3 = 11
#   x0 + x2 + x3 = 11
#   x0 + x1 = 5
#   x0 + x1 + x2 = 10
#   x2 = 5

inputs = [parse(line) for line in read(10).splitlines()]
out = 0
for i, machine in enumerate(inputs):
    res = fewest_button_presses(machine)
    print(f"Machine {i + 1}/{len(inputs)}: {res} presses")
    out += res

print(out)
