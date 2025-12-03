import sys

seq = []
input_file = sys.argv[1] if len(sys.argv) > 1 else "1.demo"
with open(input_file, "r") as f:
    lines = f.readlines()
    for line in lines:
        dir, turns = line[0], int(line[1:])
        seq.append((dir, turns))

n = 50
out = 0
for dir, turns in seq:
    dturn = -1 if dir == "L" else 1
    for i in range(turns):  # ):
        n = (n + dturn) % 100
        if n == 0:
            out += 1

print(out)
