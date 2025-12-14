import numpy as np
import numpy.typing as npt
from aoc_util import read
from matplotlib import pyplot as plt


def prettyprint(xys) -> None:
    if not isinstance(xys, np.ndarray):
        xys = np.array(xys)
    if xys.shape[1] == 2:
        plt.plot(xys[:, 0], xys[:, 1], "-")
    else:
        plt.matshow(xys)
    plt.show()


def compress(xys: list[tuple[int, int]]):
    x_rank, y_rank = {}, {}
    x_idx, y_idx = 0, 0

    for x, _ in sorted(xys, key=lambda xy: xy[0]):
        if x not in x_rank:
            x_rank[x] = x_idx
            x_idx += 1

    for _, y in sorted(xys, key=lambda xy: xy[1]):
        if y not in y_rank:
            y_rank[y] = y_idx
            y_idx += 1

    return [(x_rank[x], y_rank[y]) for x, y in xys], x_rank, y_rank


def compressed_points_to_grid(rankspace_pts: tuple[int, int]) -> npt.NDArray:
    M, N = max([pt[0] for pt in rankspace_pts]), max([pt[1] for pt in rankspace_pts])
    grid = np.zeros((M + 1, N + 1))

    # fill in vertices, in rank-space
    for x, y in rankspace_pts:
        grid[x][y] = 1

    # fill in edges, in rank-space
    rankspace_pts = np.vstack((rankspace_pts, rankspace_pts[0]))
    for (x1, y1), (x2, y2) in zip(rankspace_pts[:-1], rankspace_pts[1:]):
        x_min, x_max = min(x1, x2), max(x1, x2)
        y_min, y_max = min(y1, y2), max(y1, y2)
        grid[x_min : x_max + 1, y_min : y_max + 1] = 1

    # flood-fill
    seed = (144, 144) if grid.shape[0] > 100 else (2, 2)
    q = [seed]
    while q:
        (x, y) = q.pop()  # popleft() matters here!
        grid[x][y] = 1
        for dx, dy in [(0, -1), (0, 1), (1, 0), (-1, 0)]:
            nx, ny = x + dx, y + dy
            if (
                0 <= nx < grid.shape[0]
                and 0 <= ny < grid.shape[1]
                and grid[nx][ny] == 0
            ):
                q.append((nx, ny))

    return grid


def get_max_area(
    xys: list[tuple[int, int]], grid: npt.NDArray, x_rank: dict, y_rank: dict
) -> int:
    max_area = 0
    for i, (x1, y1) in enumerate(xys):
        for x2, y2 in xys[i + 1 :]:
            x_min, x_max = min(x1, x2), max(x1, x2)
            y_min, y_max = min(y1, y2), max(y1, y2)
            x_min_compressed, x_max_compressed = x_rank[x_min], x_rank[x_max]
            y_min_compressed, y_max_compressed = y_rank[y_min], y_rank[y_max]

            # collision check
            if np.all(
                grid[
                    x_min_compressed : x_max_compressed + 1,
                    y_min_compressed : y_max_compressed + 1,
                ]
            ):
                area = (x_max - x_min + 1) * (y_max - y_min + 1)
                max_area = max(max_area, area)
    return max_area


xys = [tuple(map(int, line.split(","))) for line in read(9).splitlines()]
compressed_points, x_rank, y_rank = compress(xys)
filled_grid = compressed_points_to_grid(compressed_points)
prettyprint(filled_grid)
print(get_max_area(xys, filled_grid, x_rank, y_rank))
