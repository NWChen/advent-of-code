import math

from aoc_util import read


def process(input: list[str]) -> list[list[str]]:
    # 1. scan for columns that are all whitespace
    separator_col_idxs = [-1]
    maxlen = max([len(line) for line in input])
    for i in range(maxlen):
        if all([line[i] == " " for line in input]):
            separator_col_idxs.append(i)
    separator_col_idxs.append(maxlen)

    # 2. split input on those columns
    out = []
    for line in input:
        row = []
        for i in range(len(separator_col_idxs) - 1):
            start, end = separator_col_idxs[i], separator_col_idxs[i + 1]
            row.append(line[start + 1 : end])
        out.append(row)
    return out


# '123 328  51 64 ...' -> [[['123', '328', ' 51', '64 '], ...]
lines = process(read(6, strip=False).splitlines())

# [['123', '328', ' 51', '64 '], ...] -> [('123', ' 45', '  6', '*  '), ...]
lines = list(zip(*lines))

problems = []
for column in lines:
    problem = list(zip(*[list(token) for token in column[:-1]]))
    problem, operator = ["".join(tup).strip() for tup in problem], column[-1].strip()
    problems.append(problem + [operator])

out = 0
for problem in problems:
    operands, operator = problem[:-1], problem[-1]
    operands = list(map(int, operands))
    out += math.prod(operands) if operator == "*" else sum(operands)
print(out)
