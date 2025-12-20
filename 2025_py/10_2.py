from collections import deque
from dataclasses import dataclass

from aoc_util import read


@dataclass
class Machine:
    lights: list[bool]
    buttons: list[tuple[int]]
    joltages: list[int]


def parse(line: str):
    tokens = line.split(" ")
    lights_str, buttons_list, joltages_str = tokens[0], tokens[1:-1], tokens[-1]
    return Machine(
        lights=[True if c == "#" else False for c in list(lights_str[1:-1])],
        buttons=[
            button if isinstance(button, tuple) else (button,)
            for button in list(map(eval, buttons_list))
        ],
        joltages=list(map(int, joltages_str[1:-1].split(","))),
    )


def fewest_button_presses(machine: Machine) -> int:
    q = deque(
        [
            (
                [False for _ in machine.lights],  # indicator light sequence
                [0 for _ in machine.joltages],  # joltage counters
                0,  # button presses
            )
        ]
    )
    target_lights, target_joltages = machine.lights, machine.joltages
    visited = set(str(q[0][0]))
    while q:
        lights, joltages, presses = q.popleft()
        print(q)
        if lights == target_lights and joltages == target_joltages:
            return presses
        for button in machine.buttons:
            new_lights, new_joltages = lights[:], joltages[:]
            for toggle in button:
                new_lights[toggle] = not new_lights[toggle]
                new_joltages[toggle] += 1
            s = str(new_joltages)
            if s not in visited:
                visited.add(s)
                q.append((new_lights, new_joltages, presses + 1))
    return -1


inputs = [parse(line) for line in read(10).splitlines()]
presses = 0
for i, machine in enumerate(inputs):
    res = fewest_button_presses(machine)
    print(f"Found result for machine {i + 1}/{len(inputs)}: {res}")
    presses += res
print(presses)
