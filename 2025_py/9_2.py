from aoc_util import read

EMPTY, RED, GREEN = 0, 1, 2


def prettyprint(grid: list[list[int]]) -> None:
    for row in grid:
        s = ""
        for cell in row:
            match cell:
                case 0:
                    s += "."
                case 1:
                    s += "R"
                case 2:
                    s += "G"
        print(s)


def max_area(xys: list[tuple]) -> int:
    m = 0
    for i, top_left in enumerate(xys):
        for bottom_right in xys[i + 1 :]:
            (y1, x1), (y2, x2) = top_left, bottom_right
            m = max(m, (abs(x2 - x1) + 1) * (abs(y2 - y1) + 1))
    return m


# inplace update grid
def scanlines(grid: list[list[int]], xys: list[tuple[int, int]]) -> None:
    R, C = len(grid), len(grid[0])
    for r, row in enumerate(grid):
        intersection_cols = []  # vertical edge crossings
        for i in range(len(xys) - 1):
            (r1, c1), (r2, c2) = xys[i], xys[i + 1]
            if r1 > r2:  # let (r1, c1) be the top point
                r1, r2 = r2, r1
                c1, c2 = c2, c1

            if c1 == c2 and r1 <= r <= r2:
                intersection_cols.append(c1)

        intersection_cols.sort()
        for i in range(0, len(intersection_cols), 2):
            left, right = intersection_cols[i], intersection_cols[i + 1]
            for j in range(left, right + 1):
                grid[r][j] = GREEN

    # overlay polygon
    for x, y in xys:
        grid[x][y] = RED


xys = [tuple(map(int, line[::-1].split(","))) for line in read(9).splitlines()]
xys.append(xys[0])
M, N = max([x for (x, _) in xys]) + 1, max([y for (_, y) in xys]) + 1
grid = [[EMPTY for _ in range(N)] for _ in range(M)]
scanlines(grid, xys)
prettyprint(grid)
# print(max_area(xys))
