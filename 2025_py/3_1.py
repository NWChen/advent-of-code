import sys


def max_joltage(b: int) -> int:
    out = 0
    sb = str(b)
    for i in range(len(sb)):
        for j in range(i + 1, len(sb)):
            out = max(int(sb[i] + sb[j]), out)
    return out


input_file = sys.argv[1] if len(sys.argv) > 1 else "3.demo"
with open(input_file, "r") as f:
    batteries = [int(line) for line in f.readlines()]

out = 0
for battery in batteries:
    out += max_joltage(battery)

print(out)
