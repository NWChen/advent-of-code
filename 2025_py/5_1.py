from aoc_util import read

# parse
input = read(5)
ranges_str, ids_str = input.split("\n\n")

ids = [int(x) for x in ids_str.split("\n")]
ranges = []
for range in ranges_str.split("\n"):
    ranges.append([int(x) for x in range.split("-")])


# solve
def is_fresh(id: int, ranges: list) -> int:
    for src, dst in ranges:
        if src <= id <= dst:
            return 1
    return 0


fresh = sum([is_fresh(id, ranges) for id in ids])
print(fresh)
