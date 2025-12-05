from aoc_util import read

# parse
input = read(5)
ranges_str = input.split("\n\n")[0]
ranges = [[int(x) for x in range.split("-")] for range in ranges_str.split("\n")]


# merge ranges
ranges = sorted(ranges, key=lambda range: range[0])
merged_ranges = []
for start, end in ranges:
    if not merged_ranges or start > merged_ranges[-1][1]:
        merged_ranges.append((start, end))
    else:  # overlap
        mr_start, mr_end = merged_ranges[-1]
        merged_ranges[-1] = (mr_start, max(mr_end, end))

out = sum([end - start + 1 for start, end in merged_ranges])
print(out)
