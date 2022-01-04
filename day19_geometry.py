import numpy as np
from scipy import spatial

# TODO this creates a new numpy array from nested list each invocation - inefficient
def octahedral_groups():
    out = []
    #for rot in range(0, 360, 90):
    #    rad = np.radians(rot)
    #    cos, sin = np.cos(rad), np.sin(rad)
    #    out.append(np.array([[1, 0, 0],
    #       [0, cos, -sin],
    #       [0, sin, cos]], dtype=int))
    #    out.append(np.array([[cos, 0, sin],
    #       [0, 1, 0],
    #       [-sin, 0, cos]], dtype=int))
    #    out.append(np.array([[cos, 0, sin],
    #       [sin, cos, 0],
    #       [0, 0, 1]], dtype=int))
    
    # todo write something that does this
    out = spatial.transform.Rotation.create_group('O').as_matrix()
    return out

# find a translation T that overlays mat1 onto mat2
# since we don't know the point correspondence from mat1 -> mat2,
# try all point matchings
def translate(mat1, mat2):
    assert mat1.dtype == 'int64'
    assert mat2.dtype == 'int64'
    for row1 in mat1:
        for row2 in mat2:
            T = row2 - row1
            error = np.count_nonzero(mat2 - (mat1 + T))
            #if np.array_equal(np.add(mat1, T), mat2): # TODO why doesn't this work?
            if error == 0:
                return (True, T)
            elif -690 in mat1 and -336 in mat1 and 620 in mat1 and -789 in mat1:
                print(error, (mat2 - (mat1 + T)))
    return (False, None)
