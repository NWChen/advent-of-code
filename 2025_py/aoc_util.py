import sys


def read(day: int):
    assert 0 < day <= 12
    input_file = sys.argv[1] if len(sys.argv) > 1 else f"{day}.demo"
    with open(input_file, "r") as f:
        return f.read().strip()
    return []
