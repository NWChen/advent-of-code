import os
import sys

DEMO_DIR = "demo"
INPUT_DIR = "input"


def read(day: int, strip: bool = True) -> str:
    assert 0 < day <= 12
    base_filepath = sys.argv[1] if len(sys.argv) > 1 else f"{day}.demo"
    filename = os.path.basename(base_filepath)

    # if we can't find the given input file (whether demo or input file),
    # look in some possible other places
    possible_dirs = [
        base_filepath,
        os.path.join(DEMO_DIR, filename),
        os.path.join(INPUT_DIR, filename),
    ]
    for filepath in possible_dirs:
        try:
            with open(filepath, "r") as f:
                print(f"Using input file at {filepath}")
                out = f.read()
                return out.strip() if strip else out
        except FileNotFoundError:
            print(f"Input file not found at {filepath}; searching...")
    return ""
