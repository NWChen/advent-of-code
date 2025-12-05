import os
import sys
import time

import numpy as np

input_file = sys.argv[1] if len(sys.argv) > 1 else "4.demo"
grid = []


with open(input_file, "r") as f:
    for line in f.readlines():
        grid.append([0 if c == "." else 1 for c in line.strip()])


def prettyprint(g):
    a = np.full(g.shape, ".")
    a[g == 1] = "@"
    a[g == 0] = "."
    print("\n".join(["".join(row) for row in a[1:-1, 1:-1]]))


def remove(g):
    removed = 0
    out = g.copy()
    for r in range(1, len(grid) + 1):
        for c in range(1, len(grid[0]) + 1):
            window = g[r - 1 : r + 2, c - 1 : c + 2]
            if g[r][c] == 1 and (np.sum(window) - 1) < 4:
                out[r][c] = 0
                removed += 1
    # print(f"Remove {removed} rolls of paper:")
    os.system("clear")
    prettyprint(out)
    time.sleep(0.01)
    return removed, out


pg = np.pad(np.array(grid), pad_width=1)
last_g = np.array([])
removed = 0

while not np.array_equal(last_g, pg):
    last_g = pg.copy()
    dr, pg = remove(pg)
    removed += dr

print(removed)
