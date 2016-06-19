from dpd2 import SimulationObject, BinBox, freeObjList
from sys import argv

def bead(line):
    
    data = line.split()
    _bead = SimulationObject(x=data[1], 
                             y=data[2],
                             z=data[3])
    
    setattr(_bead, "bead_id", data[0])    
    return _bead

def hydrophobe_tail(num_beads, in_file):
    _tail = {}
    
    _tail["block"] = polymer_block(num_beads, in_file)
    # TODO calc center of mass, store reference to other tail
    return _tail
    
def polymer_block(num_beads, in_file):
    _block = []
    
    for i in range(num_beads):
        _block.append(bead(in_file.readline()))
    
    return _block

def triblock(poly_length, tail_length, in_file):
    _triblock = {
       "tail1": hydrophobe_tail(tail_length, in_file),
       "block": polymer_block(poly_length, in_file),
       "tail2": hydrophobe_tail(tail_length, in_file)
    }
    
    # TODO this just be center of mass beads for tails
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

def process(**kwargs):
    
    num_beads_per_frame = kwargs["num_beads"]
    poly_length = kwargs["poly_length"]
    tail_length = kwargs["tail_length"]
    
    # TODO come up with a clever way to determine
    # the number of frames without have to reading 
    # the whole file
    num_frames = kwargs["num_frames"]
    chain_length = poly_length + 2 * tail_length
    num_chains = num_beads_per_frame // chain_length
    
    cluster_solver = BinBox(boxDimensions=kwargs["box_dimensions"])
    
    with open(kwargs["input_file"], "r") as in_file:
        for i in range(num_frames):            
            _frame = frame(num_chains, poly_length, tail_length, in_file)
            
            cluster_solver.deriveClusters(_frame["beads"])
            
            freeObjList(_frame["beads"])
            
            _frame["micelles"] = []
            for cluster in cluster_solver.clusterList:
                _frame["micelles"].append(micelle(cluster))
                
            cluster_solver.empty()


if __name__ == "__main__":
    pass

    