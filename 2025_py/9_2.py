import numpy as np
import numpy.typing as npt
from aoc_util import read
from matplotlib import pyplot as plt


def prettyprint(xys: npt.NDArray, points=False) -> None:
    if points:
        plt.matshow(xys)
    else:
        plt.plot(xys[:, 0], xys[:, 1], "-")
    plt.show()


# def is_collinear(p1, p2, p3):
#     (x1, y1), (x2, y2), (x3, y3) = p1, p2, p3
#     horizontal = y1 == y2 == y3
#     vertical = x1 == x2 == x3
#     return horizontal or vertical


# def compress(xys: list[tuple[int, int]]):
#     n, out = len(xys), []
#     for i in range(n):
#         p1 = xys[(i - 1 + n) % n]
#         p2 = xys[i]
#         p3 = xys[(i + 1) % n]
#         if not is_collinear(p1, p2, p3):
#             out.append(p2)
#     return out


def is_collinear(pts):
    n = len(pts)
    p1, p2, p3 = pts[(np.arange(n) - 1) % n], pts, pts[(np.arange(n) + 1) % n]
    x1, x2, x3 = p1[:, 0], p2[:, 0], p3[:, 0]
    y1, y2, y3 = p1[:, 1], p2[:, 1], p3[:, 1]
    horizontal = (y1 == y2) & (y2 == y3)
    vertical = (x1 == x2) & (x2 == x3)
    return horizontal | vertical


def compress(xys: npt.NDArray):
    print("Compressing...")
    return xys[~is_collinear(xys)]


def fill(xys: npt.NDArray):
    grid = np.zeros((np.max(xys[:, 0]) + 1, np.max(xys[:, 1]) + 1), dtype=bool)
    for (x1, y1), (x2, y2) in zip(xys[:-1], xys[1:]):
        x1, x2 = min(x1, x2), max(x1, x2)
        y1, y2 = min(y1, y2), max(y1, y2)
        grid[x1 : x2 + 1, y1 : y2 + 1] = True

    seed = (57200, 57200) if 57200 < grid.shape[0] else (9, 5)
    q, visited = [seed], set([seed])
    i = 0
    while q:
        (x, y) = q.pop()
        i += 1
        # print(f"Filled {i}/{grid.shape[0] * grid.shape[1]}")
        grid[x][y] = True
        for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
            nx, ny = x + dx, y + dy
            if (
                (x, y) != (nx, ny)
                and 0 <= nx < grid.shape[0]
                and 0 <= ny < grid.shape[1]
                and grid[nx][ny] == 0
                and (nx, ny) not in visited
            ):
                visited.add((nx, ny))
                q.append((nx, ny))

    return grid


def get_max_area(xys: list[tuple[int, int]], grid: npt.NDArray) -> int:
    max_area = 0
    for i, (x1, y1) in enumerate(xys):
        for x2, y2 in xys[i + 1 :]:
            x_min, x_max = min(x1, x2), max(x1, x2)
            y_min, y_max = min(y1, y2), max(y1, y2)
            if np.all(grid[x_min : x_max + 1, y_min : y_max + 1]):
                area = (x_max - x_min + 1) * (y_max - y_min + 1)
                max_area = max(max_area, area)
    return max_area


xys = np.array([tuple(map(int, line.split(","))) for line in read(9).splitlines()])
xys = np.vstack((compress(xys), np.array(xys[0])))  # hacky closure
grid = fill(xys)
# prettyprint(grid, True)
print(get_max_area(xys, grid))
