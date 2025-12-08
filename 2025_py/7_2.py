from aoc_util import read

START, SPLITTER, EMPTY = "S", "^", "."
grid = [list(line) for line in read(7).splitlines()]


def prettyprint(g: list[list[str]]):
    print("\n".join(["".join(row) for row in g]))


def solve_dp():
    dp = [[0 for _ in grid[0]] for _ in grid]
    dp[0][grid[0].index(START)] = 1
    R, C = len(grid), len(grid[0])
    for r in range(R - 1):
        for c in range(C):
            cell = grid[r][c]
            if cell == EMPTY or cell == START:
                dp[r + 1][c] += dp[r][c]
            elif cell == SPLITTER:
                if c > 0:
                    dp[r + 1][c - 1] += dp[r][c]
                if c < C - 1:
                    dp[r + 1][c + 1] += dp[r][c]

    return sum(dp[-1])


def solve_rec():
    start_c = grid[0].index(START)
    m = {}

    def ways(r, c):
        if r == len(grid) - 1:
            return 1
        if (r, c) in m:
            return m[(r, c)]
        if grid[r][c] == EMPTY:
            return ways(r + 1, c)
        left = ways(r + 1, c - 1) if c > 0 else 0
        right = ways(r + 1, c + 1) if c < len(grid[0]) - 1 else 0
        m[(r, c)] = left + right
        return left + right

    return ways(0, start_c)


print(solve_rec())
print(solve_dp())
