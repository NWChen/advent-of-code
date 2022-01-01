import numpy as np
import re
import sys
from day19_geometry import octahedral_groups, rotate, translate
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
    scanner_to_beacons = { k: np.array(v) for k, v in scanners.items() } # these values are deltas, not points!
    return scanner_to_beacons

def manhattan(b1: np.ndarray, b2: np.ndarray):
    return np.sum(np.absolute(b2 - b1))

#def euclidean(b1, b2):
#    return np.linalg.norm(b2 - b1)
#
def distances(beacons: np.ndarray):
    mat = np.zeros((beacons.shape[0], beacons.shape[0]), dtype=int) 
    for i, b1 in enumerate(beacons): # TODO this is inefficent use of numpy
        for j, b2 in enumerate(beacons):
            if not np.array_equal(b1, b2):
                mat[i][j] = manhattan(b1, b2)
    return mat

# bs1, bs2: |beacons| x |beacons| distance mats, possibly different shapes
def shared_beacons(bs1: np.ndarray, bs2: np.ndarray):
    for row1 in bs1:
        for row2 in bs2:
            intersection, bs1_idxs, bs2_idxs = np.intersect1d(row1, row2, return_indices=True)
            if intersection.shape[0] >= 12:
                return (intersection.shape[0], bs1_idxs, bs2_idxs)
    return (0, None, None)

# computes the transformation (Rx)T,
# returns (successful?, R, T) if possible
# note rotation+translation doesn't commute
def compute_tf(mat1, mat2):
    #from scipy import spatial
    #for R in spatial.transform.Rotation.create_group("O").as_matrix():
    for R in octahedral_groups():
        Rx = np.dot(mat1, R)
        succ, T = translate(Rx, mat2)
        if succ:
            return (True, R, T) 
    return (False, None, None)            
    #for up in range(0, 360, 90):
    #    for normal in np.concatenate((np.eye(3), -np.eye(3))):
    #        (R, Rx) = rotate(mat1, up, normal) 
    #        print(R)
    #        (successful, T) = translate(Rx, mat2)
    #        if successful:
    #            return (True, R, T)
    #return (False, None, None)
    
def partOne(filename):
    scanners = fread(sys.argv[1])
    
    # a map from scanner ID i in [0...n] -> {... scanner IDs with beacons that intersect with scanner i ...}
    intersections = defaultdict(list)
    dists = {}
    for scanner, beacons in scanners.items():
        dists[scanner] = distances(beacons)
    
    # compute scanner-scanner intersections
    for s1, d1 in dists.items():
        for s2, d2 in [tup for tup in dists.items() if tup[0] != s1]:
            n_matched, bs1_idxs, bs2_idxs = shared_beacons(d1, d2)
            if n_matched >= 12:
                bs1 = scanners[s1][bs1_idxs]
                bs2 = scanners[s2][bs2_idxs]
                intersections[s1].append((s2, bs1, bs2))
     
    # compute scanner-scanner transformations
    tf = defaultdict(dict)
    for s1 in intersections:
        for (s2, bs1, bs2) in intersections[s1]:
                valid_tf, R, T = compute_tf(bs1, bs2)
                if valid_tf:
                    tf[s1][s2] = (R, T)

    # fill in remaining transformations
    print(tf)
    
    # at the end we should be able to take np.unique(all known beacons, i.e. all transformations wrt e.g. scanner 0)                
    # to get # of beacons
    # to do that, we need all transformations wrt scanner 0
    # traverse DAG of transformations, for all scanners other than 0?
    return 0

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

#def permute_scanner(m: dict, scanner: int) -> list:
#    out = []
#    for up in range(0, 360, 90):
#        for normal in [np.array([1, 0, 0]), np.array([0, 1, 0]), np.array([0, 0, 1])]:
#            out.append(orient_beacons(m[scanner], up, normal)) 
#            out.append(orient_beacons(m[scanner], up, -normal)) 
#    out = np.array(out) # a 24 x |beacons| x 3 ndarray
#    return out

def badpartOne(filename):
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    colors = ['c', 'r', 'g', 'y', 'b']
    
    out = 0 
    scanners = fread(sys.argv[1])
    for s1 in scanners:
        for s2 in [id for id in scanners if id != s1]:
            
            # TODO this won't work because of unique possible orderings
            
            for tf1 in permute_scanner(scanners, s1): # tf = transformation: 1 of 24
                for tf2 in permute_scanner(scanners, s2):
                    tf1 = np.sort(tf1, axis=0) # TODO not this
                    tf2 = np.sort(tf2, axis=0)
                    (bigger_tf, smaller_tf) = (tf1, tf2) if tf1.shape[0] > tf2.shape[0] else (tf2, tf1)
                    #print(bigger_tf.shape[0], smaller_tf.shape[0])
                    for i in range(bigger_tf.shape[0] - smaller_tf.shape[0] + 1):
                        translations = bigger_tf[i:i + smaller_tf.shape[0],] - smaller_tf
                        print(np.unique(translations, axis=0))
                        n_matched = translations.shape[0] - np.unique(translations, axis=0).shape[0]
                        if n_matched >= 12:
                            out += n_matched
            
            #ax.scatter(perm[:,0], perm[:,1], perm[:,2], c=colors[scanner], alpha=0.5)
    #plt.show()
    print(out)
    return out

if __name__ == '__main__':
    partOne(sys.argv[1])
    