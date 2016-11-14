from dpd2 import SimulationObject, BinBox, free_obj_list
from multiprocessing import Process, Queue, cpu_count
from time import time
from sys import argv, exit

def bead(line):
    data = line.split()
    _bead = {
        "x": float(data[1]), 
        "y": float(data[2]),
        "z": float(data[3]),
        "bead_id": int(data[0])
    }
        
    return _bead

def hydrophobe_tail(chain_idx, idx, num_beads, in_file):
    _tail = {}
    
    _tail["block"] = polymer_block(num_beads, in_file)
    _tail["chain_idx"] = chain_idx
    _tail["tail_idx"] = idx

    return _tail

def tail_calc_com(_tail):
    com = {
        "x": 0.0,
        "y": 0.0,
        "z": 0.0
    }
     
    for i in range(len(_tail["block"])):
        for key in com.keys():
            com[key] += _tail["block"][i][key]
            
    for key in com.keys():
        com[key] /= len(_tail["block"])
        
    _tail["com"] = SimulationObject(**com)
    
    setattr(_tail["com"], "chain_idx", _tail["chain_idx"])
    setattr(_tail["com"], "tail_idx", _tail["tail_idx"])
    
def polymer_block(num_beads, in_file):
    _block = []
    
    for i in xrange(num_beads):
        _block.append(bead(in_file.next().rstrip('\n')))
    
    return _block

def triblock(idx, poly_length, tail_length, in_file):
    _triblock = {
        "idx": idx,
       "tail1": hydrophobe_tail(idx, "tail1", tail_length, in_file),
       "block": polymer_block(poly_length, in_file),
       "tail2": hydrophobe_tail(idx, "tail2", tail_length, in_file)
    }
    
    return _triblock

def triblock_get_beads(_triblock):
    tail_calc_com(_triblock["tail1"])
    tail_calc_com(_triblock["tail2"])
    _beads = []
    _beads.append(_triblock["tail1"]["com"])
    _beads.append(_triblock["tail2"]["com"])
    return _beads

def frame(idx, num_chains, poly_length, tail_length, in_file):
    _frame = {
        "idx": idx,
       "chains": [],
       "beads": [],
       "micelles": None
    }
    
    for i in xrange(num_chains):
        chain = triblock(i, poly_length, tail_length, in_file)
        _frame["chains"].append(chain)
    
    return _frame

def calc_com(_frame):
    for i in xrange(len(_frame["chains"])):
        for bead in triblock_get_beads(_frame["chains"][i]):
            _frame["beads"].append(bead)

def micelle(cluster):
    # TODO 
    return cluster

class Worker(Process):
    
    def __init__(self, idx, inQ, outQ, box_dimensions):
        super(Worker, self).__init__()
        self.box_dimensions = box_dimensions
        self.idx = idx
        self.name = "Worker " + str(idx)
        self.inQ = inQ
        self.outQ = outQ
        
    def run(self):
        
        while True:
           
#             print(self.name + " is waiting to receive frame...")
            _frame = self.inQ.get()
            
            if _frame is None:
#                 print(self.name + " is exiting...")
                break
    
#             print(self.name + " is processing frame " + str(_frame["idx"]))
    
            out = process_frame(_frame, self.box_dimensions)
            
#             print(self.name + " is trying to send...")
            self.outQ.put(out) 
            
        exit(0)

def process_frame(_frame, box_dimensions):
    calc_com(_frame)
    # Do stuff with the frame
    solver = BinBox(box_dimensions)
    solver.derive_clusters(_frame["beads"])
    free_obj_list(_frame["beads"])
    
    _frame["micelles"] = []
    for cluster in solver.clusterList:
        _frame["micelles"].append(micelle(cluster))
            
    solver.destroy()
    del solver

    # Print to output
    out = "\t Frame " + str(_frame["idx"])
    out += ",\tNumber of micelles: " + str(len(_frame["micelles"]))
    
    #TODO do more stuff
    
    del _frame
    
    return out

def main(**kwargs):
    
    num_beads_per_frame = kwargs["num_beads"]
    poly_length = kwargs["poly_length"]
    tail_length = kwargs["tail_length"]
    
    # TODO come up with a clever way to determine
    # the number of frames without have to reading 
    # the whole file
    num_frames = kwargs["num_frames"]
    chain_length = poly_length + 2 * tail_length
    num_chains = num_beads_per_frame // chain_length

    num_workers = 4
    print("Creating " + str(num_workers) + " workers...")
    workers = []
    queues = []
             
    for i in range(num_workers):        
        qs = {
           "input": Queue(),
           "output": Queue()
        }
        w = Worker(i, qs["input"], qs["output"], kwargs["box_dimensions"])
        workers.append(w)
        queues.append(qs)
        w.start()
#     
    print("Opening file... beginning processing...")
    frame_count = 0
    worker_idx = 0
    with open(kwargs["input_file"], "r") as in_file:
        for i in xrange(num_frames): 
            in_file.next() # skip num atoms
            in_file.next() # skip comment
            if i % 10 == 0:           
                _frame = frame(i, num_chains, poly_length, tail_length, in_file)
#                 process_frame(_frame, kwargs["box_dimensions"])
                queues[worker_idx]["input"].put(_frame)
                worker_idx = (worker_idx + 1) % num_workers
                frame_count += 1
#                 print("Main is sending frame " + str(i))
            else:
                # skip frame
                for i in xrange(num_beads_per_frame):
                    in_file.next()
                
    out_count = 0
    worker_idx = 0
    while out_count < frame_count:
#         print("Main is receiving output " + str(out_count))
        out = queues[worker_idx]["output"].get()
        print(out)
        worker_idx = (worker_idx + 1) % num_workers
        out_count += 1          
         
    for i in range(num_workers):
        queues[i]["input"].put(None)
        workers[i].join()
        
    print("Processing complete.")

if __name__ == "__main__":
    start = time()
    main(**{
        "input_file": argv[1],
        "num_beads": int(argv[2]),
        "poly_length": int(argv[3]),
        "tail_length": int(argv[4]),
        "num_frames": int(argv[5]),
        "box_dimensions": {
            "x": float(argv[6]),
            "y": float(argv[6]),
            "z": float(argv[6])
        }    
    })
    ttl_time = time() - start
    print("Ttl time: " + str(ttl_time))

    