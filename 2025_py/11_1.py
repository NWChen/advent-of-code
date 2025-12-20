from aoc_util import read

adj = {
    k: v.split(" ")
    for [k, v] in map(lambda line: line.split(": "), read(11).splitlines())
}

q = [("you", {"you"})]
npaths = 0
while q:
    cur, visited = q.pop()
    if cur == "out":
        npaths += 1
        continue
    for neighbor in adj[cur]:
        if neighbor not in visited:
            q.append((neighbor, set(visited) | {neighbor}))

print(npaths)
