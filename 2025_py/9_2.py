import numpy as np
from aoc_util import read
from matplotlib import pyplot as plt


def prettyprint(xys: list[tuple[int, int]]) -> None:
    xys = np.array(xys)
    plt.plot(xys[:, 0], xys[:, 1], "-")
    plt.show()


# check if rectangle bounded by tl (topleft) and br (bottomright) intersects with any edge
# (assumes all edges are axis-aligned)
def intersects(tl: tuple[int, int], br: tuple[int, int], edges) -> bool:
    (r_min, c_min), (r_max, c_max) = tl, br
    # (c_min, r_min), (c_max, r_max) = tl, br
    assert r_min < r_max and c_min < c_max
    for (E_r1, E_c1), (E_r2, E_c2) in edges:
        r_overlap, c_overlap = False, False
        if E_r1 == E_r2:
            E_c_min, E_c_max = min(E_c1, E_c2), max(E_c1, E_c2)
            r_overlap = r_min < E_r1 < r_max
            c_overlap = max(c_min, E_c_min) < min(c_max, E_c_max)
        elif E_c1 == E_c2:
            E_r_min, E_r_max = min(E_r1, E_r2), max(E_r1, E_r2)
            r_overlap = max(r_min, E_r_min) < min(r_max, E_r_max)
            c_overlap = c_min < E_c1 < c_max
        if r_overlap and c_overlap:
            return True
    return False


def lerp_col(src: tuple[int, int], dst: tuple[int, int], pt: int) -> int:
    (r1, c1), (r2, c2) = src, dst
    r, c = pt
    t = (r - r1) / (r2 - r1)
    c_intersection = c1 + t * (c2 - c2)
    return c_intersection


# raycast horizontally
def contains(pt: tuple[int, int], edges: list[tuple[int, int]]) -> bool:
    r, c = pt
    crossings = 0
    for src, dst in edges:
        (E_r1, E_c1), (E_r2, E_c2) = src, dst
        crosses = (E_r1 > r) != (E_r2 > r)  # vertical edge
        if crosses and c < lerp_col(src, dst, pt):
            crossings += 1
    return crossings % 2 == 1


def max_area(xys: list[tuple], edges: list[tuple[int, int]]) -> int:
    m = 0
    for i, p1 in enumerate(xys):
        for p2 in xys[i + 1 :]:
            (r1, c1), (r2, c2) = p1, p2
            r_min, r_max = min(r1, r2), max(r1, r2)
            c_min, c_max = min(c1, c2), max(c1, c2)
            if not ((r_min < r_max) and (c_min < c_max)):
                continue
            tl = (r_min, c_min)
            br = (r_max, c_max)
            valid = not intersects(tl, br, edges)
            contained = contains(tl, edges) and contains(br, edges)
            if valid and contained:
                m = max(m, (abs(r_max - r_min) + 1) * (abs(c_max - c_min) + 1))
    return m


xys = [tuple(map(int, line[::-1].split(","))) for line in read(9).splitlines()]
xys.append(xys[0])
edges = [(xys[i], xys[i + 1]) for i in range(len(xys) - 1)]
print(max_area(xys, edges))

# try every rectangle; if it intersects the polygon, it's not a candidate
# assert intersects((1, 7), (7, 11), [((5, 9), (9, 9))])  # interior of polygon
# assert intersects((1, 7), (7, 11), [((4, 9), (4, 12))])  # right edge of polygon
# assert intersects((1, 7), (7, 11), [((4, 5), (4, 9))])  # left edge of polygon
# assert intersects((1, 7), (7, 11), [((0, 9), (2, 9))])  # top edge of polygon
# assert intersects((1, 7), (7, 11), [((6, 9), (8, 9))])  # bottom edge of polygon
# assert not intersects((1, 7), (7, 11), [((4, 5), (4, 6))])
