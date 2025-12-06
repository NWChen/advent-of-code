import sys


# find twelve digits, inorder in b, such that they form a maximal integer
def max_joltage(b: int) -> int:
    sb = str(b)
    idxs = [(len(sb) - i) for i in range(1, 13)][::-1]
    min_idx = -1
    for i, idx in enumerate(idxs):
        max_idx, max_digit = idx, 0
        while idx > min_idx:
            if int(sb[idx]) >= max_digit:
                max_digit = int(sb[idx])
                max_idx = idx
            idx -= 1
        idxs[i] = max_idx
        min_idx = max(min_idx, max_idx)

    out = int("".join(sb[idx] for idx in idxs))
    return out


input_file = sys.argv[1] if len(sys.argv) > 1 else "3.demo"
with open(input_file, "r") as f:
    batteries = [int(line) for line in f.readlines()]

out = 0
for battery in batteries:
    out += max_joltage(battery)

print(out)
