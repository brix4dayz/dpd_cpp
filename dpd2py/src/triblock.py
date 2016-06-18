from dpd2 import SimulationObject, BinBox, freeObjList
from sys import argv



def bead(line):
    pass

def hydrophobe_tail(num_beads, in_file):
    _tail = {}
    
    _tail["block"] = polymer_block(num_beads, in_file)
    # TODO calc center of mass, store reference to other tail
    return _tail
    

def polymer_block(num_beads, in_file):
    _block = []
    
    return _block

def triblock(poly_length, tail_length, in_file):
    _triblock = {
       "tail1": hydrophobe_tail(tail_length, in_file),
       "block": polymer_block(poly_length, in_file),
       "tail2": hydrophobe_tail(tail_length, in_file)
    }
    
    def get_beads():
        _beads = []
        
        for bead in _triblock["tail1"]["block"]:
            _beads.append(bead)
        for bead in _triblock["block"]:
            _beads.append(bead)
        for bead in _triblock["tail"]["block"]:
            _beads.append(bead)

    _triblock["get_beads"] = get_beads
    
    return _triblock

def frame(num_chains, poly_length, tail_length, in_file):
    # TODO
    _frame = {
       "chains": [],
       "beads": [],
       "micelles": None
    }
    
    for i in xrange(num_chains):
        chain = triblock(poly_length, tail_length, in_file)
        _frame["chains"].append(chain)
        for bead in chain["get_beads"]():
            _frame["beads"].append(bead)
    
    return _frame

def micelle(cluster):
    pass

if __name__ == "__main__":
    
    pass
    