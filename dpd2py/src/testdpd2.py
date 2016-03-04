'''
Created on Mar 2, 2016

@author: Hayden
'''

from dpd2 import SimulationObject
from dpd2 import BinBox
from dpd2 import destroyObjList

'''
    Main function for testing.
'''
if __name__ == '__main__':
    
    ###################### CLUSTER CASE 1 #################################
    l = []
    l.append(SimulationObject(0,1,0))
    l.append(SimulationObject(0,35,0))
    l.append(SimulationObject(35.5, 0.5, 39.8))
    l.append(SimulationObject(0.0, 35.25, 39.38))
    l.append(SimulationObject(100,100,100))
    l.append("string") # print error message

    dimensions = {
                    'x': 36,
                    'y': 36,
                    'z': 40
                  }
    
    solver = BinBox(dimensions,4.0,4.25)
    
    solver.deriveClusters(l) # print out of bounds
    
    print(solver.numClusters()) # 1
    
    clusters = solver.clusterList
    
#     for i in l:
#         try:
#             i.destroy()
#         except NotImplementedError as e:
#             print(e)
#         except AttributeError:
#             pass
#         finally:
#             del i
#     del l
    
    ################# MEMORY MANAGEMENT TEST ##########################
    try:
        solver.destroy()
    except NotImplementedError as e:
        print(e)
    finally:
        del solver
        
    for i in range(len(clusters)):
        c = clusters[i]
        print(str(i))
        for objs in c:
            print(str(objs))
            
    destroyObjList(l)
        
    #################### ERROR TEST ###############################
    dimensions = {'a':'stuff'}
    solver = BinBox(dimensions) # print error message
    
