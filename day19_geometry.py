import numpy as np

# TODO this creates a new numpy array from nested list each invocation - inefficient
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
    #from scipy import spatial
    #out = spatial.transform.Rotation.create_group('O').as_matrix()
    return out

# find a translation T that overlays mat1 onto mat2
# since we don't know the point correspondence from mat1 -> mat2,
# try all point matchings
def translate(mat1, mat2):
    for row1 in mat1:
        for row2 in mat2:
            T = np.tile(np.subtract(row2, row1), (mat1.shape[0], 1)) # mat1.shape[0] should == 12 or 13
            #if np.array_equal(np.add(mat1, T), mat2): # TODO why doesn't this work?
            error = np.sum(np.subtract(mat2, np.add(mat1, T)))
            if error == 0:
                T_vec = T[0,:]
                return (True, T_vec)
            #elif 649 in mat1 and 640 in mat1 and 665 in mat1:
            #    print(error)
    return (False, None)

#def correspondence(mat1, mat2):
#    for row1 in mat2:
#        for row2 in mat2:
