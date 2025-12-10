from aoc_util import read

xys = [tuple(map(int, line.split(","))) for line in read(9).splitlines()]


def max_area(xys: list[tuple]) -> int:
    m = 0
    for i, top_left in enumerate(xys):
        for bottom_right in xys[i + 1 :]:
            (y1, x1), (y2, x2) = top_left, bottom_right
            m = max(m, (abs(x2 - x1) + 1) * (abs(y2 - y1) + 1))
    return m


print(max_area(xys))
