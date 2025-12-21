from aoc_util import read

adj = {
    k: v.split(" ")
    for [k, v] in map(lambda line: line.split(": "), read(11).splitlines())
}


def solve_iterative():
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
    return npaths


def solve_recursive():
    def dfs(src: str, dst: str):
        if src == dst:
            return 1
        paths = sum(dfs(v, dst) for v in adj[src])
        return paths

    return dfs("you", "out")


print(solve_recursive())
