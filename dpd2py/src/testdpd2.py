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

def printClusters(clusters):
    if isinstance(clusters, list):
        print("Number of clusters: " + str(len(clusters)))
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


def cleanUp():
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
    
    clusters = solver.deriveClusters(l) # print out of bounds
    
    printClusters(clusters)
    
    cleanUp()
    
    print("Ending: TEST 1 ")
        
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
       
    solver.deriveClusters(l) # print error about str not being allowed and out of bounds

    print("Ending: TEST ERRORS")

    return

'''
    Main function for testing.
'''
if __name__ == '__main__':
    
    testcase1()
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
    


    
