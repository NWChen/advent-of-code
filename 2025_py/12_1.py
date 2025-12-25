import copy
from dataclasses import dataclass

import numpy as np
import numpy.typing as npt
from aoc_util import read


@dataclass
class Region:
    width: int
    height: int
    qtys: list[int]


# parse
shapes, regions = {}, []
for block in read(12).split("\n\n"):
    lines = block.splitlines()
    if "x" in lines[0]:  # region
        for line in lines:
            dims_str, qtys_str = line.split(": ")
            w, h = map(int, dims_str.split("x"))
            regions.append(
                Region(
                    w,
                    h,
                    {
                        idx: qty
                        for idx, qty in enumerate(map(int, qtys_str.split()))
                        if qty > 0
                    },
                )
            )
    else:
        idx, *grid = lines
        shape = (np.array([list(row) for row in grid]) == "#").astype(int)
        shapes[int(idx.rstrip(":"))] = shape


def can_fit_naive(idxs_to_shapes: dict, region: Region) -> bool:
    shapes = []
    for idx, qty in region.qtys.items():
        shapes.extend([idxs_to_shapes[idx] for _ in range(qty)])

    total_shape_area = sum(np.sum(shape) for shape in shapes)
    # return (9 * sum(region.qtys.values())) > (region.height * region.width)
    return total_shape_area <= region.height * region.width


# def can_fit_just_boxes(box_dim: int, region: Region) -> bool:
#     return 9 * sum(region.qtys.values()) > region.height * region.width
# n_vert = region.height / box_dim
# n_horiz = region.width / box_dim
# return sum(region.qtys.values()) < n_horiz * n_vert


def can_fit_everything_rec(idxs_to_shapes: dict, region: Region) -> bool:
    memo = {}
    shapes = []
    for idx, qty in region.qtys.items():
        shapes.extend([idxs_to_shapes[idx] for _ in range(qty)])

    def fits(shapes: list[npt.NDArray], grid: npt.NDArray, i: int) -> bool:
        if not shapes:  # packed all the shapes we needed to
            return True

        if sum(np.sum(shape) for shape in shapes) > np.sum(grid == 0):  # early stopping
            return False

        shape = shapes[0]
        # state = (shape.tobytes(), grid.tobytes())
        # if state in memo:
        #     return memo[state]

        (M, N), (m, n) = grid.shape, shape.shape
        for turns in range(4):
            rotated_shape = np.rot90(shape, k=turns)
            for r in range(M - m + 1):
                for c in range(N - n + 1):
                    if not np.any(grid[r : r + m, c : c + n] & rotated_shape):
                        next_grid = grid.copy()
                        next_grid[r : r + m, c : c + n] |= rotated_shape
                        if fits(shapes[1:], next_grid):
                            return True

        # memo[state] = False
        return False

    grid = np.zeros((region.height, region.width), dtype=int)
    return fits(shapes, grid)


def can_fit_everything(idxs_to_shapes: dict, region: Region) -> bool:
    # shapes = [
    #     idxs_to_shapes[idx] for idx, qty in region.qtys.items() for _ in range(qty)
    # ]
    shapes = []
    for idx, qty in region.qtys.items():
        shapes.extend([idx] * qty)
    M, N = region.height, region.width
    grid = np.zeros((M, N), dtype=int)
    q = [(grid, shapes)]

    while q:
        grid, shapes = q.pop()
        if shapes == []:
            return True

        shape = idxs_to_shapes[shapes[0]]
        BRANCHING_LIMIT = 10
        r, c = 0, 0
        for turns in range(4):
            rot = np.rot90(shape, k=turns)
            m, n = rot.shape
            empty_idxs = np.argwhere(grid == 0)
            random_idxs = np.random.choice(
                len(empty_idxs),
                size=min(BRANCHING_LIMIT, len(empty_idxs)),
                replace=False,
            )
            for r, c in [empty_idxs[i] for i in random_idxs]:
                if grid[r, c] or r > M - m or c > N - n:
                    continue
                if np.any(grid[r : r + m, c : c + n] & rot):
                    continue
                new_grid = grid.copy()
                new_grid[r : r + m, c : c + n] |= rot
                q.append((new_grid, shapes[1:]))

    return False


# for region in regions:
# print(can_fit_everything_rec(shapes, region))
# print(can_fit_everything(shapes, region))
# print(sum(can_fit_just_boxes(3, region) for region in regions))
print(sum(can_fit_naive(shapes, region) for region in regions))

# s = np.array(
#     [
#         [1, 1, 1],
#         [1, 0, 0],
#         [1, 1, 1],
#     ]
# )
# assert (
#     fit_one_shape(
#         s,
#         np.array(
#             [
#                 [0, 0, 0, 0],
#                 [0, 0, 0, 0],
#                 [0, 0, 0, 0],
#                 [0, 0, 0, 0],
#             ]
#         ),
#     )
#     != []
# ), "empty grid"
# assert (
#     fit_one_shape(
#         s,
#         np.array(
#             [
#                 [0, 0, 0, 0],
#                 [1, 1, 1, 0],
#                 [1, 0, 0, 0],
#                 [1, 1, 1, 0],
#             ]
#         ),
#     )
#     != []
# ), "one 4 shape placed"
