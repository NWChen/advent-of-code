import sys

seq = []
input_file = sys.argv[1] if len(sys.argv) > 1 else "2.demo"
with open(input_file, "r") as f:
    ranges = f.read().split(",")
    for r in ranges:
        s = r.split("-")
        src, dst = int(s[0]), int(s[1])
        seq.append((src, dst))

out = 0
for src, dst in seq:
    for n in range(src, dst + 1):
        sn = str(n)
        half = len(sn) // 2
        if sn[:half] == sn[half:]:
            out += n

print(out)
