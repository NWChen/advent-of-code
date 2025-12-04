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

    return out


input_file = sys.argv[1] if len(sys.argv) > 1 else "3.demo"
with open(input_file, "r") as f:
    batteries = [int(line) for line in f.readlines()]

out = 0
for battery in batteries:
    out += max_joltage(battery)

print(out)
