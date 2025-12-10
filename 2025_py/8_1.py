import math

from aoc_util import read


def distance(p1: tuple, p2: tuple) -> float:
    return math.sqrt(sum([(a - b) ** 2 for a, b in zip(p1, p2)]))


points = [tuple(map(int, row.split(","))) for row in read(8).splitlines()]
distances = []
for i, p1 in enumerate(points):
    for p2 in points[i + 1 :]:
        distances.append((distance(p1, p2), p1, p2))
distances = sorted(distances, key=lambda tup: tup[0])
circuits = [{point} for point in points]


def find(needle: tuple, haystack: list[set[tuple]]):
    for i, hay in enumerate(haystack):
        if needle in hay:
            return i
    return -1


def largest_n(components: list[set], n: int = 3):
    return sorted(components, key=lambda component: -len(component))[:n]


circuits = [{point} for point in points]
connections = 0
LIMIT = 1000
for distance, p1, p2 in distances:
    i, j = find(p1, circuits), find(p2, circuits)
    connections += 1

    if i >= 0 and j >= 0 and i != j:
        circuits[i] |= circuits[j]
        del circuits[j]

    if connections == LIMIT:
        break

print(math.prod([len(circuit) for circuit in largest_n(circuits)]))
