import numpy as np
import re
import sys
from collections import defaultdict
from matplotlib import pyplot as plt

# for every scanner,
# generate all permutations of beacons in 3d space
# with respect to each permutation (of 24 total) of a scanner orientation
# over all these permutations (24 * |beacons|), take the 1 that matches
# where 'matches' = >=1 pair of scanners each observe >=12 common beacons
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
    return { k: np.array(v) for k, v in scanners.items() } # these values are deltas, not points!

def manhattan(b1: np.ndarray, b2: np.ndarray):
    return np.sum(np.absolute(b2 - b1))

def distances(beacons: np.ndarray):
    mat = np.zeros((beacons.shape[0], beacons.shape[0]), dtype=int) 
    for i, b1 in enumerate(beacons): # TODO this is inefficent use of numpy
        for j, b2 in enumerate(beacons):
            if not np.array_equal(b1, b2):
                mat[i][j] = manhattan(b1, b2)
    return mat

# bs1: a |beacons| x |beacons| distance mat
# bs2: a |beacons'| x |beacons'| distance mat
# returns [([row] index of beacon in bs1, [row] index of matching beacon in bs2), ... ]
def share_geq12_beacons(bs1: np.ndarray, bs2: np.ndarray):
    # for every row1 of bs1,
    # is there a row2 of bs2
    # that shares at least 12 elements[distances] (excluding 0) with row1?
    for row1 in bs1:
        for row2 in bs2:
            intersection = np.intersect1d(row1[row1 != 0], row2[row2 != 0])
            if intersection.shape[0] >= 12:
                return intersection.shape[0]
    return 0

# TODO this creates a new numpy array from nested list each invocation
# which is very inefficient
def R_x(v, deg=90):
    rad = np.radians(deg)
    cos = np.cos(rad)
    sin = np.sin(rad)
    mat = np.array([[1, 0, 0],
           [0, cos, -sin],
           [0, sin, cos]], dtype=int)
    return np.dot(v, mat)

def R_y(v, deg=90):
    rad = np.radians(deg)
    cos = np.cos(rad)
    sin = np.sin(rad)
    mat = np.array([[cos, 0, sin],
           [0, 1, 0],
           [-sin, 0, cos]], dtype=int)
    return np.dot(v, mat)

def R_z(v, deg=90):
    rad = np.radians(deg)
    cos = np.cos(rad)
    sin = np.sin(rad)
    mat = np.array([[cos, -sin, 0],
           [sin, cos, 0],
           [0, 0, 1]], dtype=int)
    return np.dot(v, mat)

# orient a beacon in 3d space, assuming origin at (0, 0, 0)
# using [beacon_]deltas from origin, in the plane specified by 'normal' vector
# up = degrees? one of 0, 90, 180, 270
# beacons: an (m, 3) array
# where m=# beacons
def orient_beacons(beacons: np.ndarray, up: int, normal: np.ndarray):
    if normal[0]:
        return R_x(beacons, up)
    elif normal[1]:
        return R_y(beacons, up)
    elif normal[2]:
        return R_z(beacons, up)

# beacons: an m x 3 ndarray
# translation: a (3,) ndarray
def translate_beacons(beacons: np.ndarray, translation: np.ndarray):
    T = np.tile(translation, (beacons.shape[0], 1)) 
    return beacons + T

# for any 2 scanners,
# is there a permutation & translation of each scanner, such that both scanners see the same >=12 beacons?
# i.e.
# for all permutations p1 of scanner1,
#   for all permutations p2 of scanner2,
#       is there a >=12-element subset of (p2 - p1) s.t. each element is the same? (translation is the same?)
# OR
# compute distance matrix for a given scanner, for each beacon's dist to another beacon
# if any 2 scanners share a 12-element subset of that distance matrix in common,
# they are (probably) the same 12 beacons
# so compute a relative rotation/transformation between those two scanners
def eq(beacons1, beacons2):
    return np.array_equal(np.sort(beacons1, axis=0), np.sort(beacons2, axis=0))

#def compute_RT(mat1, mat2):

def partOne(filename):
    scanners = fread(sys.argv[1])
    out = 0
    
    #RT_mat = np.zeros((len(scanners), len(scanners))) # R = rot, T = translation
    #RT = defaultdict({})
    
    for s1, bs1 in scanners.items(): # scanner = an int (id of a scanner)
        d1 = distances(bs1)
        for s2, bs2 in scanners.items():
            d2 = distances(bs2)
            if s1 != s2:
                matches = share_geq12_beacons(d1, d2)
                out += matches
   
    print(out) 
    return out

#def permute_scanner(m: dict, scanner: int) -> list:
#    out = []
#    for up in range(0, 360, 90):
#        for normal in [np.array([1, 0, 0]), np.array([0, 1, 0]), np.array([0, 0, 1])]:
#            out.append(orient_beacons(m[scanner], up, normal)) 
#            out.append(orient_beacons(m[scanner], up, -normal)) 
#    out = np.array(out) # a 24 x |beacons| x 3 ndarray
#    return out
#
#def partOne(filename):
#    fig = plt.figure()
#    ax = plt.axes(projection='3d')
#    colors = ['c', 'r', 'g', 'y', 'b']
#    
#    scanners = fread(sys.argv[1])
#    for scanner in scanners:
#        for perm in permute_scanner(scanners, scanner):
#            ax.scatter(perm[:,0], perm[:,1], perm[:,2], c=colors[scanner], alpha=0.5)
#    plt.show()
#    pass            

if __name__ == '__main__':
    partOne(sys.argv[1])
    