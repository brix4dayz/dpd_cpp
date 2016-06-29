from dpd2 import SimulationObject, BinBox, free_obj_list
from multiprocessing import Process, Semaphore, Queue, cpu_count
from time import sleep, time
from sys import argv, exit

def bead(line):
    
    # TODO change back to dict
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
    
    for i in range(num_beads):
        _block.append(bead(in_file.readline()))
    
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
    
    def __init__(self, idx, queue, my_lock, neighbor_lock, box_dimensions):
        super(Worker, self).__init__()
        self.solver = BinBox(box_dimensions)
        self.idx = idx
        self.queue = queue
        self.my_lock = my_lock
        self.neighbor_lock = neighbor_lock
        
    def run(self):
        
        while True:
            # Get the frame
            self.my_lock.acquire()
            _frame = self.queue.get()
            if _frame is None:
                self.queue.put(None)
                self.neighbor_lock.release()
                exit()
            self.neighbor_lock.release()
    
            calc_com(_frame)
            # Do stuff with the frame
            self.solver.derive_clusters(_frame["beads"])
            free_obj_list(_frame["beads"])
            
            _frame["micelles"] = []
            for cluster in self.solver.clusterList:
                _frame["micelles"].append(micelle(cluster))
                    
            self.solver.empty()
    
            # Print to output
            self.my_lock.acquire()
            print("\t" + str(self.idx) + " print results for frame " + str(_frame["idx"]))
            print("\t\tNumber of micelles: " + str(len(_frame["micelles"])))
            self.neighbor_lock.release()    


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

    num_workers = cpu_count()
    print("Creating " + str(num_workers) + " workers...")
    queue = Queue()
    workers = []
    locks = []
    for i in range(num_workers):
        locks.append(Semaphore(0))
            
    for i in range(num_workers):        
        ni = (i + 1) % num_workers
        w = Worker(i, queue, locks[i], locks[ni], kwargs["box_dimensions"])
        workers.append(w)
        w.start()
    
    print("Opening file... beginning processing...")
    with open(kwargs["input_file"], "r") as in_file:
        locks[0].release()
        for i in range(num_frames): 
            in_file.readline() # skip num atoms
            in_file.readline() # skip comment
            if i % 10 == 0:           
                _frame = frame(i, num_chains, poly_length, tail_length, in_file)
                queue.put(_frame)
            else:
                # skip frame
                for i in xrange(num_beads_per_frame):
                    in_file.readline()
                
    while not queue.empty():
        sleep(1.0)
        
    queue.put(None)
        
    for w in workers:
        w.join()
        
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

    