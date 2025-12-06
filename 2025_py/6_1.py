import math

from aoc_util import read

input = [line.split() for line in read(6).split("\n")]
input = zip(*input)

out = 0
for problem in input:
    operands, operator = problem[:-1], problem[-1]
    operands = list(map(int, operands))
    if operator == "*":
        out += math.prod(operands)
    elif operator == "+":
        out += sum(operands)
print(out)
