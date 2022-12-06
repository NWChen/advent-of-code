adjs = {0: [23, 23], 5: [10, 13, 10, 13], 6: [10, 10], 23: [0, 0], 1: [3, 22, 3, 22], 3: [1, 1, 19, 29, 19, 29], 22: [1, 15, 19, 1, 15, 19], 2: [7, 7], 7: [2, 2, 20, 24, 20, 24], 19: [3, 8, 3, 8, 22, 24, 28, 22, 24, 28], 29: [3, 24, 3, 24], 4: [8, 20, 24, 8, 20, 24], 8: [4, 4, 15, 19, 15, 19], 20: [4, 7, 12, 4, 7, 12], 24: [4, 7, 14, 19, 4, 7, 14, 19, 29, 30, 29, 30], 10: [5, 6, 5, 6, 18, 26, 18, 26], 13: [5, 5, 16, 18, 16, 18], 15: [8, 8, 22, 22], 9: [17, 27, 17, 27], 17: [9, 16, 9, 16], 27: [9, 9], 12: [20, 20], 18: [10, 13, 10, 13, 21, 21], 26: [10, 11, 21, 10, 11, 21], 11: [26, 26], 16: [13, 13, 17, 17], 14: [24, 24], 28: [19, 19], 21: [18, 18, 26, 26], 30: [24, 24], 25: []}
scanner_ids = list(adjs.keys())
from collections import defaultdict

def all_paths(adjs, scanner_ids):
    out = defaultdict(dict)
    q = [(id, {id}, [id]) for id in scanner_ids]
    
    while q:
        id, visited, path = q.pop(0)
        for adj in set([adj for adj in adjs[id] if adj not in visited]):
            _path = path + [adj]
            out[id][adj] = _path
            _visited = set(visited)
            _visited.add(adj)
            q.append((adj, _visited, _path))
    
    reachable_scanners = set(scanner_ids)
    for to_scanners in out.values():
        reachable_scanners &= set(to_scanners.keys())
    import pdb; pdb.set_trace()
    common_scanner = list(reachable_scanners)[0] # TODO possible value/keyerror?
    return (out, common_scanner)

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
    
print(list(connected_components(adjs)))