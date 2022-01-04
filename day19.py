import numpy as np
import re
import sys
#from day19_geometry import octahedral_groups, translate
from collections import defaultdict
from matplotlib import pyplot as plt
from scipy import spatial

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
    return np.linalg.norm(b1 - b2, ord=1)

# unused but cool
def connected_components(adjs):
    visited = set()
    
    def component(scanner):
        scanners = {scanner}
        while scanners:
            scanner = scanners.pop()
            visited.add(scanner)
            scanners |= (set(adjs[scanner]) - visited)
            yield scanner
    
    components = []        
    for adj in adjs:
        if adj not in visited:
            components.append(list(component(adj)))
    return components

# returns path from $scanner -> target_id for all scanners in adj
# no guarantee of shortest path
def paths(adjs, target_id):
    out = {}
    q = []
    for id in adjs.keys():
        q.append((id, {id}, [id])) 
    
    while q:
        id, visited, path = q.pop(0)
        if id == target_id:
            out[path[0]] = path
        else:
            for adj in set([adj for adj in adjs[id] if adj not in visited]):
                _path = path + [adj]
                #out[id][adj] = _path
                _visited = set(visited)
                _visited.add(adj)
                q.append((adj, _visited, _path))
    if target_id in out:
        del out[target_id]
    return out

def rotations():
    return spatial.transform.Rotation.create_group('O').as_matrix()

def compute_distances(mat_i, mat_j, dist_func=manhattan):
    distances = np.zeros((mat_i.shape[0], mat_j.shape[0]), dtype='int64')
    distances = np.zeros((mat_i.shape[0], mat_j.shape[0]), dtype='int64')
    for i in range(mat_i.shape[0]):
        for j in range(mat_j.shape[0]):
            distances[i][j] = dist_func(mat_i[i,], mat_j[j,])
    return distances

def translate(s_i, s_j, mat_i, mat_j):
    # assert(mat_i.dtype == 'int64') 
    # assert(mat_j.dtype == 'int64') 
    # unknown point correspondence
    # is there a translation mat_i -> mat_j that results in >=12 matches?
    distances = compute_distances(mat_i, mat_j)
    for i in range(mat_i.shape[0]):
        for j in range(mat_j.shape[0]):           
            dist = manhattan(mat_i[i,], mat_j[j,])
            matches = distances[distances == dist]
            if matches.shape[0] >= 12:
                T = mat_j[j,] - mat_i[i,]
                return (True, T)
    return (False, None) 

def compute_tf(s_i, s_j, scanners):
    for R in rotations():
        R = R.astype('int64')
        R_i = np.matmul(scanners[s_i], R)
        successful_transform, T = translate(s_i, s_j, R_i, scanners[s_j])
        if successful_transform:
            return (True, R, T)
    return (False, None, None) 
        
def solve(filename):
    scanners = fread(sys.argv[1]) 
    tfs = defaultdict(dict)
    adjs = defaultdict(set)
    
    for s_i in scanners:
        for s_j in [s for s in scanners if s != s_i]:
            (successful_tf, R, T) = compute_tf(s_i, s_j, scanners)
            if successful_tf:
                tfs[s_i][s_j] = (R, T)
                adjs[s_i].add(s_j)
                
    s0_basis_beacons = {}
    s0_basis_scanners = {}
    for scanner, path in paths(adjs, 0).items():
        mat = scanners[scanner]
        s_mat = np.zeros((3,), dtype='int64')
        for i in range(len(path) - 1):
            s_from, s_to = path[i], path[i + 1]
            (R, T) = tfs[s_from][s_to]
            assert R.dtype == 'int64'
            assert T.dtype == 'int64'
            mat = np.add(np.matmul(mat, R).astype('int64'), T)
            s_mat = np.add(np.matmul(s_mat, R).astype('int64'), T)
            
        s0_basis_beacons[scanner] = mat
        s0_basis_scanners[scanner] = s_mat

    s0_basis_beacons[0] = scanners[0]
    s0_basis_scanners[0] = np.zeros((3,), dtype='int64')
    
    # part 2
    max_scanner_distance = 0
    for s1 in s0_basis_scanners.values():
        for s2 in s0_basis_scanners.values():
            max_scanner_distance = max(manhattan(s1, s2), max_scanner_distance)
    import pdb; pdb.set_trace()
    
    s0_basis_beacons = np.concatenate(list(s0_basis_beacons.values()), axis=0)
    s0_basis_beacons = np.unique(s0_basis_beacons, axis=0)
    n_beacons = s0_basis_beacons.shape[0]
    return (n_beacons, max_scanner_distance)
        
    #ax = plt.axes(projection='3d')
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
  
if __name__ == '__main__':
    print(solve(sys.argv[1]))
    