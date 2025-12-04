import sys

import numpy as np

input_file = sys.argv[1] if len(sys.argv) > 1 else "4.demo"
grid = []
with open(input_file, "r") as f:
    for line in f.readlines():
        grid.append([0 if c == "." else 1 for c in line.strip()])

out = 0
padded_grid = np.pad(np.array(grid), pad_width=1)
for row_start in range(1, len(grid) + 1):
    for col_start in range(1, len(grid[0]) + 1):
        window = padded_grid[
            row_start - 1 : row_start + 2, col_start - 1 : col_start + 2
        ]
        if padded_grid[row_start][col_start] == 1 and (np.sum(window) - 1) < 4:
            out += 1

print(out)
