import sys

# find twelve digits, inorder in b, such that they form a maximal integer
# def max_joltage(b: int) -> int:
#     sb = str(b)

#     def neighbors(idx: int):
#         return [j for j in range(idx + 1, len(sb))]

#     out = 0
#     visited = set()
#     q = [(int(sb[i]), i) for i in range(len(sb) - 12)]
#     max_joltage_so_far = 0
#     while q:
#         joltage_so_far, idx = q.pop(0)
#         print(f"{b}: {idx} / {len(sb)} ({joltage_so_far})")
#         if joltage_so_far in visited or joltage_so_far < max_joltage_so_far:
#             continue
#         visited.add(joltage_so_far)
#         sj = str(joltage_so_far)
#         max_joltage_so_far = max(joltage_so_far, max_joltage_so_far)
#         if len(sj) == 12:
#             out = max(out, joltage_so_far)
#         else:
#             for j in neighbors(idx):
#                 next_digit = sb[j]
#                 next_sj = sj + str(next_digit)
#                 q.append((int(next_sj), j))

#     return out


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
