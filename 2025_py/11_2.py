import copy
from collections import deque

from aoc_util import read

adj = {
    k: v.split(" ")
    for [k, v] in map(lambda line: line.split(": "), read(11).splitlines())
}


def get_num_paths(src: str, dst: str, adj: dict):
    q = deque([src])
    visited = set()
    out = 0
    while q:
        u = q.popleft()
        visited.add(u)
        if u == dst:
            out += 1
            continue
        if u in adj:
            q.extend([v for v in adj[u] if v not in visited])
    return out


routes = [
    ["svr", "dac", "fft", "out"],
    ["svr", "fft", "dac", "out"],
]

out = 0
for route in routes:
    num_paths: int = 1
    for i in range(len(route) - 1):
        src, dst = route[i], route[i + 1]
        print(f"Finding routes from {src} to {dst} for route {'->'.join(route)}...")
        num_paths *= get_num_paths(src, dst, adj)
    out += num_paths

print(out)
