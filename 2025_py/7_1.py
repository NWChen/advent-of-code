from aoc_util import read

BEAM, SPLITTER, EMPTY = "|", "^", "."
grid = [list(line) for line in read(7).splitlines()]


def prettyprint(g: list[list[str]]):
    print("\n".join(["".join(row) for row in g]))


# performs a single-step (row) inplace update on the given grid `g`
# `last_r` = last updated row, i.e. the index of the row from which
#   we should begin this update (all rows above will be ignored)
# returns the most recently updated row, and the number of splits made in this update
def update(g: list[list[str]], last_r: int = 0):
    r = last_r + 1
    if last_r == 0:
        start_c = g[0].index("S")  # column where the beam entered the manifold
        g[r][start_c] = BEAM
        return r, 0

    dsplits = 0
    for c, cell in enumerate(g[r]):
        if g[last_r][c] == BEAM and cell == SPLITTER:  # beam hit splitter
            dsplits += 1
            if c >= 1:
                g[r][c - 1] = BEAM  # split beam to the left
            if c < len(grid[0]) - 1:
                g[r][c + 1] = BEAM  # split beam to the right
        elif g[last_r][c] == BEAM and cell == EMPTY:
            g[r][c] = BEAM
    return r, dsplits


row, splits = 0, 0
while row < len(grid) - 1:
    row, dsplits = update(grid, row)
    splits += dsplits
    prettyprint(grid)

print(splits)
