import numpy as np
import re
import sys
from collections import defaultdict
from matplotlib import pyplot

# for every scanner,
# generate all permutations of beacons in 3d space
# with respect to each permutation (of 24 total) of a scanner orientation
# over all these permutations (24 * |beacons|), take the 1 that matches across all scanners
# where 'matches' = there is no beacon that only 1 scanner can see (?)
def fread(filename: str) -> defaultdict:
    scanners = defaultdict(list)
    id = 0 # of a scanner
    
    f = open(filename, 'r')
    for line in f.readlines():
        if '---' in line:
            id = int(re.search(r'([0-9]+)', line).group()) # TODO this is unnecessary, since scanners are ordered in a file
        elif line and line != '\n':
            tokens = line.replace('\n', '').split(',')
            xyz = np.array(list(map(lambda n: int(n), tokens)), dtype=int)
            scanners[id].append(xyz)
 
    f.close()
    return { k: np.ndarray(v) for k, v in scanners.items() } # these values are deltas, not points!

# orient a beacon in 3d space, assuming origin at (0, 0, 0)
# using beacon_deltas from origin, in the plane specified by 'normal' vector
def orient_beacon(deltas: np.ndarray, up: np.ndarray, normal: np.ndarray) -> np.ndarray:
    beacon = np.zeros(3, dtype=int)
    

def permute_scanner() -> list:
    pass

if __name__ == '__main__':
    scanners = fread(sys.argv[1])