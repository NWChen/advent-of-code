import numpy as np

# TODO this creates a new numpy array from nested list each invocation
# which is very inefficient
# orient a beacon in 3d space, assuming origin at (0, 0, 0)
# using [beacon_]deltas from origin, in the plane specified by 'normal' vector
# up = degrees: one of 0, 90, 180, 270
# beacons: an (|beacons|, 3) array
def rotate(beacons: np.ndarray, up: int, normal: np.ndarray):
    R = None
    rad = np.radians(up)
    cos, sin = np.cos(rad), np.sin(rad)
    if normal[0] != 0:
        R = np.array([[1, 0, 0],
           [0, cos, -sin],
           [0, sin, cos]], dtype=int)
    elif normal[1] != 0:
        R = np.array([[cos, 0, sin],
           [0, 1, 0],
           [-sin, 0, cos]], dtype=int)
    elif normal[2] != 0:
        R = np.array([[cos, -sin, 0],
           [sin, cos, 0],
           [0, 0, 1]], dtype=int)
    print(R.shape)
    return (R, np.dot(beacons, R))

def octahedral_groups():
    out = []
    for rot in range(0, 360, 90):
        rad = np.radians(rot)
        cos, sin = np.cos(rad), np.sin(rad)
        out.append(np.array([[1, 0, 0],
           [0, cos, -sin],
           [0, sin, cos]], dtype=int))
        out.append(np.array([[cos, 0, sin],
           [0, 1, 0],
           [-sin, 0, cos]], dtype=int))
        out.append(np.array([[cos, 0, sin],
           [sin, cos, 0],
           [0, 0, 1]], dtype=int))
    return out

# find a translation T that overlays mat1 onto mat2
# since we don't know the point correspondence from mat1 -> mat2,
# try all point matchings
def translate(mat1, mat2):
    for row1 in mat1:
        for row2 in mat2:
            T = np.tile(np.subtract(row2, row1), (mat1.shape[0], 1)) # mat1.shape[0] should == 12 or 13
            #if np.array_equal(np.add(mat1, T), mat2): # TODO why doesn't this work?
            if np.sum(mat2 - (mat1 + T)) == 0:
                T_vec = T[0,:]
                return (True, T_vec)
    return (False, None)
