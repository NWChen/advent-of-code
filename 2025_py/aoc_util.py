import sys


def read(day: int, strip: bool = True):
    assert 0 < day <= 12
    input_filepath = sys.argv[1] if len(sys.argv) > 1 else f"{day}.demo"
    with open(input_filepath, "r") as f:
        out = f.read()
        return out.strip() if strip else out
    return []
