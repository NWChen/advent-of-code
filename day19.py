import numpy as np
import re
import sys
from day19_geometry import octahedral_groups, translate
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

def euclidean(b1: np.ndarray, b2: np.ndarray):
    return np.linalg.norm(b1 - b2)

def manhattan(b1: np.ndarray, b2: np.ndarray):
    return np.sum(np.absolute(b2 - b1))

def distances(beacons: np.ndarray):
    mat = np.zeros((beacons.shape[0], beacons.shape[0]), dtype=int) 
    for i, b1 in enumerate(beacons): # TODO this is inefficent use of numpy
        for j, b2 in enumerate(beacons):
            if i != j:
                mat[i][j] = euclidean(b1, b2)
    return mat

# bs1, bs2: |beacons| x |beacons| distance mats, possibly different shapes
def get_shared_beacons(bs1: np.ndarray, bs2: np.ndarray):
    for row1 in bs1:
        for row2 in bs2:
            intersection, bs1_idxs, bs2_idxs = np.intersect1d(row1, row2, return_indices=True)
            # TODO do we have to worry about 0 values here?
            if intersection.shape[0] >= 11: # 11, since 1 of the beacons will have dist 0 (gives us 12)
                return (intersection.shape[0], bs1_idxs, bs2_idxs)
    return (0, None, None)

# computes the transformation (Rx)T,
# returns (successful?, R, T) if possible
# note rotation+translation doesn't commute
def compute_tf(mat1, mat2):
    for R in octahedral_groups():
        R = R.astype('int') # <<< this is very important, otherwise matmul defaults to float & floating point check may error out in line 61
        Rx = np.matmul(mat1, R)
        succ, T_vec = translate(Rx, mat2)
        if succ:
            return (True, R, T_vec) 
    return (False, None, None)

# returns path from $scanner -> 0 for all scanners in adj
# no guarantee of shortest path
def paths(adjs, scanner_ids):
    out = {}
    q = []
    for id in [id for id in scanner_ids if id != 0]:
        q.append((id, {id}, [id])) 
    
    while q:
        id, visited, path = q.pop(0)
        if id == 0:
            out[path[0]] = path
        elif 0 in adjs[id]:
            q.append((0, visited, path + [0]))
        else:
            for adj in [adj for adj in adjs[id] if adj not in visited]:
                _visited = set(visited)
                _visited.add(adj)
                q.append((adj, _visited, path + [adj])) 
    return out
       
def partOne(filename):
    scanners = fread(sys.argv[1])
    
    # a map from scanner ID i in [0...n] -> {... scanner IDs with beacons that intersect with scanner i ...}
    intersections = defaultdict(list)
    dists = {}
    for scanner, beacons in scanners.items():
        dists[scanner] = distances(beacons)
    
    # compute scanner-scanner intersections
    shared_beacons = defaultdict(dict)
    for s1, d1 in dists.items():
        #for s2, d2 in [tup for tup in dists.items() if tup[0] != s1]:
        for s2, d2 in dists.items():
            if s1 == s2:
                continue
            n_matched, bs1_idxs, bs2_idxs = get_shared_beacons(d1, d2)
            if n_matched >= 11:
                bs1 = scanners[s1][bs1_idxs]
                bs2 = scanners[s2][bs2_idxs]
                shared_beacons[s1][s2] = (bs1, bs2)
                shared_beacons[s2][s1] = (bs2, bs1)
                intersections[s1].append(s2)
                intersections[s2].append(s1)
     
    # compute scanner-scanner transformations
    tf = defaultdict(dict)
    adjs = defaultdict(list)
    for s1 in intersections:
        #for (s2, bs1, bs2) in intersections[s1]:
        for s2 in intersections[s1]:
            (bs1, bs2) = shared_beacons[s1][s2]
           
            valid_tf, R, T = compute_tf(bs1, bs2)
            if valid_tf:
                tf[s1][s2] = (R, T)
                adjs[s1].append(s2)
    
    #print(intersections)        
    #print([(k, v.keys()) for k, v in tf.items()])
    #print(adjs)
    #print(tf[1])
    #print(np.matmul(scanners[1], tf[1][0][0]) + tf[1][0][1])

    # fill in remaining transformations: from $scanner to 0
    # tf[s1][s2] = (rotation, translation) from s1 to s2
    s0_basis_beacons = {}
    _paths = paths(adjs, intersections.keys()).items()
    import pdb; pdb.set_trace()
    for scanner, path in _paths:
        mat = scanners[scanner]
        
        # apply sequence of rotations to get $scanner in scanner0's basis
        for i in range(len(path) - 1):
            from_scanner, to_scanner = path[i], path[i + 1]
            (R, T) = tf[from_scanner][to_scanner]
            mat = np.add(np.matmul(mat, R), T)
        s0_basis_beacons[scanner] = mat
    
    # add scanner 0 as well
    s0_basis_beacons[0] = scanners[0]
    
    ax = plt.axes(projection='3d')
    #colors = ['b', 'g', 'r', 'black', 'sienna', 'pink']
    #s4_to_s1 = np.matmul(scanners[4], tf[4][1][0]) + tf[4][1][1]
    #print(s0_basis_beacons[4])
    #print(s0_basis_beacons[2])
    #ax.scatter(s4_to_s1[:,0], s4_to_s1[:,1], s4_to_s1[:,2], c='r', s=10)
    #ax.scatter(scanners[1][:,0], scanners[1][:,1], scanners[1][:,2], c='b', s=20)
    
    #s1_to_s0 = np.matmul(scanners[1], tf[1][0][0]) + tf[1][0][1]
    #ax.scatter(s1_to_s0[:,0], s1_to_s0[:,1], s1_to_s0[:,2], c='r', s=10)
    #ax.scatter(scanners[0][:,0], scanners[0][:,1], scanners[0][:,2], c='b', s=30)
    #crange = np.arange(len(s0_basis_beacons))
    #for i in s0_basis_beacons:
    #    xs = s0_basis_beacons[i][:,0]
    #    ys = s0_basis_beacons[i][:,1]
    #    zs = s0_basis_beacons[i][:,2]
    #    ax.scatter(xs, ys, zs, alpha=0.5, s=10.0)
    #    for j in range(s0_basis_beacons[i].shape[0]):
    #        ax.text(xs[j], ys[j], zs[j], i)
    ## TEMP FOR DEBUGGING 
    #plt.show()
   
    #print(scanners[1].shape) 
    #print(scanners[4].shape) 
    #print(np.unique(np.concatenate((s4_to_s1, scanners[1]), axis=0), axis=0).shape)
    s0_basis_beacons = np.concatenate(list(s0_basis_beacons.values()), axis=0)
    s0_basis_beacons = np.unique(s0_basis_beacons, axis=0)
    n_beacons = s0_basis_beacons.shape[0]
    return n_beacons
   
if __name__ == '__main__':
    print(partOne(sys.argv[1]))
    