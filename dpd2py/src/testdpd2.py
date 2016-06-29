'''
Created on Mar 2, 2016

@author: Hayden
'''

from dpd2 import SimulationObject
from dpd2 import BinBox

'''
    Test parameters. Need to be "cleaned" up after each test.
'''
solver = None
l = None

def print_clusters(clusters):
    if isinstance(clusters, list):
        print("Number of clusters:" + str(len(clusters)))
        for i in range(len(clusters)):
            c = clusters[i]
            print("Cluster " + str(i))
            for objs in c:
                if isinstance(objs, SimulationObject):
                    print("\t" + str(objs))
                else:
                    print(objs.__class__.__name__ + " is not SimulationObject.")
    else:
        print("Clusters must be a list, cannot be a " + clusters.__class__.__name__)
    return


def clean_up():
    global solver
    global l
    # frees the underlying C++ pointers, use when your
    # done with a BinBox
    # use empty() to clear BinBox before a new frame
    try:
        solver.destroy()
    except NotImplementedError as e:
        print(e)
    finally:
        del solver
        
    for i in l:
        try:
            i.destroy()
        except NotImplementedError as e:
            print(e)
        except AttributeError:
            pass
        finally:
            del i
    del l
    
    solver = None
    l = None
    
    return

'''
    1 cluster.
'''
def testcase1():
    global solver
    global l
    
    print("Beginning: TEST 1")
    l = []
    l.append(SimulationObject(0,1,0))
    l.append(SimulationObject(0,35,0))
    l.append(SimulationObject(35.5, 0.5, 39.8))
    l.append(SimulationObject(0.0, 35.25, 39.38))
    
    
    # describes box dimensions, can be non-cubic
    dimensions = {
                    'x': 36.0,
                    'y': 36.0,
                    'z': 40.0
                  }
    
    # bin cube length, box is broken into cubes, or bins, of a specified length
    binSize = 4.0
    
    # cutoff distance for determining clusters
    # NOTE: binSize <= cutoffDist <= sqrt(3) * binSize
    # AKA cutoff distance shouldnt longer than the bin cube
    cutoffDist = 4.25
    
    # parameters: box dimensions, bin cube length, cluster cutoff distance
    # NOTE: 
    solver = BinBox(dimensions, binSize, cutoffDist)
    
    clusters = solver.derive_clusters(l) # print out of bounds
    
    print_clusters(clusters)
    
    clean_up()
    
    print("Ending: TEST 1 ")
        
    return
 
'''
    0 clusters. 4 separate points.
'''
def testcase2():
    global solver
    global l
    print("Beginning: TEST 2")
    
    l = []
    solver = BinBox({
                    'x': 36.0,
                    'y': 36.0,
                    'z': 40.0
                  }, 4.0, 4.25)
    
    l.append(SimulationObject(5.0, 12.0, 21.0))

    l.append(SimulationObject(0.0, 35.0, 32.0))
    
    l.append(SimulationObject(1.0, 0.5, 0.8))
    
    l.append(SimulationObject(29.7, 8.25, 15.79))

    clusters = solver.derive_clusters(l)
    print_clusters(clusters)
    
    clean_up()
    
    print("Ending: TEST 2 ")
    
    return

'''
    1 cluster. 2 separate points
'''
def testcase3():
    global solver
    global l

    print("Beginning: TEST 3")

    l = []
    solver = BinBox({'x': 15.0, 'y': 21.0, 'z': 13.0}, 3.0, 3.125)
    
    l.append(SimulationObject(7.5, 0.5, 6.5))
    l.append(SimulationObject(7.5, 20.5, 6.5))
    l.append(SimulationObject(7.0, 21.3333, 7.0))
    l.append(SimulationObject(8.0, 0.0, 6.2))
    l.append(SimulationObject(7.2, 20.9, 6.9))
        
    l.append(SimulationObject(11.0, 13.0, 0.0))
    
    l.append(SimulationObject(3.0, 20.0, 2.0))
        
    clusters = solver.derive_clusters(l)
    print_clusters(clusters)
    
    clean_up()
        
    print("Ending: TEST 3")
        
    return

'''
    3 clusters.
'''
def testcase4():
    return 

 
    
'''
    Test error handling for BinBox. Invalid dimensions parameter.
    Invalid object in frame list. 
'''
def testerrors():
    global solver
    global l
    
    print("Beginning: TEST ERRORS")
    # invalid dimensions, dictionary doesn't x, y, or z
    dimensions = {'a':'stuff'}
    solver = BinBox(dimensions) # print error message

    # create BinBox with default parameters:
    #   boxDimensions={'x':36.0, 'y':36.0, 'z':36.0},
    #   binSize=1.0,
    #   cutoffDist=1.25
    solver = BinBox()
    
    l = []
    l.append("string") # invalid object to give to binbox
    l.append(SimulationObject(100,100,100)) # out of bounds point
       
    solver.derive_clusters(l) # print error about str not being allowed and out of bounds

    print_clusters(solver.clusterList)

    print("Ending: TEST ERRORS")

    return

'''
    Main function for testing.
'''
if __name__ == '__main__':
    
    testcase1()
    testcase2()
    testcase3()
    testcase4()
    testerrors()
    
    ####### FRAME LOOP FLOW ##########
    #
    # 
    # Build list/frame of SimulationObject's
    # get clusters from solver.deriveClusters(frame)
    # call freeObjList on frame
    # solver.empty()
    # Process clusterList
    # Store results of processing
    # del frame
    #
    # *THIS ENSURES MEMORY IS MANAGED EFFICIENTLY*
    #
    ####### FRAME LOOP FLOW ##########
    


    
