seq = []
with open("1.input", "r") as f:
    lines = f.readlines()
    for line in lines:
        dir, turns = line[0], int(line[1:])
        seq.append((dir, turns))

n = 50
out = 0
for dir, turns in seq:
    if dir == "L":
        n -= turns
    if dir == "R":
        n += turns
    n %= 100
    if n == 0:
        out += 1

print(out)
