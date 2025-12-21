import copy

from aoc_util import read

adj = {
    k: v.split(" ")
    for [k, v] in map(lambda line: line.split(": "), read(11).splitlines())
}


def get_num_paths(src: str, dst: str, adj: dict):
    if src not in adj:
        return 0
    m = {}  # map node : # paths from node to dst

    def dfs(u):
        if u == dst:
            return 1
        if u in m:
            return m[u]
        if u in adj:
            num_paths = sum(dfs(v) for v in adj[u])
            m[u] = num_paths
            return num_paths
        return 0

    return dfs(src)


routes = [
    ["svr", "dac", "fft", "out"],
    ["svr", "fft", "dac", "out"],
]

out = 0
for route in routes:
    num_paths: int = 1
    for i in range(len(route) - 1):
        src, dst = route[i], route[i + 1]
        num_paths *= get_num_paths(src, dst, adj)
    out += num_paths

print(out)
