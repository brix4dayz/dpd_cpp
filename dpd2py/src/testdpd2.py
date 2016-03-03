'''
Created on Mar 2, 2016

@author: Hayden
'''

'''
    Import DLL and ctypes.
'''
from ctypes import cdll
from ctypes import c_char_p
from ctypes import c_void_p
from ctypes import c_uint
from ctypes import c_float

from sys import argv

# current makefiles only can compile .dylib (in eclipse) and .so
if (len(argv) == 1):
    libdpd2 = cdll.LoadLibrary("/Users/Hayden/Documents/Research/NCSU/Triblock/triblock/dpd_cpp/dpd2/Debug/libdpd2.dylib") # my mac
else:
    libdpd2 = cdll.LoadLibrary("/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd2/lib/libdpd2.so") # HPC

'''
    Declare non-void return types for interface functions.
'''
libdpd2.SimObjList.restype = c_void_p

libdpd2.SimObj.restype = c_void_p

libdpd2.BinBox.restype = c_void_p

libdpd2.GetGUID.restype = c_char_p

libdpd2.GetClusters.restype = c_void_p

libdpd2.GetNumClusters.restype = c_uint

libdpd2.Cluster.restype = c_void_p

libdpd2.GetNumObjects.restype = c_uint

libdpd2.ObjectFromCluster.restype = c_void_p


class DPDObject(object):
    def destroy(self):
        raise NotImplementedError("Please implement destroy()")

'''
    Python wrapper class for dpd2::SimulationObject.
'''
class SimulationObject(DPDObject):
    def __init__(self,x,y,z=0.0):
        self.x = x
        self.y = y
        self.z = z
        self.obj = c_void_p(libdpd2.SimObj(c_float(x), c_float(y), c_float(z)))
        self.guid = str(libdpd2.GetGUID(self.obj))
        return
    
    def __str__(self):
        return self.guid + " @ (" + str(self.x) + "," + str(self.y) + "," + str(self.z) + ")"
   
    def destroy(self):
        libdpd2.DeleteSimObj(self.obj)
        return
   
'''
    Python wrapper class for dpd2::cluster::BinBox.
'''
class BinBox(DPDObject):
    def __init__(self,
                 boxDimensions={'x':0.0, 'y':0.0, 'z':0.0},
                 binSize=1.0,
                 cutoffDist=1.25):
        self.boxDimensions = boxDimensions
        if (isinstance(boxDimensions, dict) and ('x' in boxDimensions.keys()) and
         ('y' in boxDimensions.keys()) and ('z' in boxDimensions.keys())):
            self.binSize = binSize
            self.cutoffDist = cutoffDist
            self.objectMap = {}
            self.objectList = libdpd2.SimObjList()
            self.obj = libdpd2.BinBox(c_float(boxDimensions["x"]), c_float(boxDimensions["y"]), c_float(boxDimensions["z"]),
                                      c_float(binSize), c_float(cutoffDist))
            self.clusterList = []
        else:
            print("python: Could not instantiate dpd2::cluster::BinBox in c++, improper dimensions.")
        return
    
    def addObj(self, sObj):
        self.objectMap[sObj.guid] = sObj;
        libdpd2.AddObjToList(c_void_p(self.objectList), sObj.obj)
        return
        
    def deriveClusters(self, sObjList):
        for sObj in sObjList:
            if isinstance(sObj, SimulationObject):
                self.addObj(sObj)
            else:
                print("python: Object is not a SimulationObject.")
        libdpd2.DeriveClusters(c_void_p(self.obj), c_void_p(self.objectList))
        
        clusters = c_void_p(libdpd2.GetClusters(c_void_p(self.obj)))
        
        numClusters = int(libdpd2.GetNumClusters(clusters))
        
        tempCluster = None
        
        numObjs = 0
        clust = None
        objPtr = None
        for i in range(numClusters):
            tempCluster = []
            clust = c_void_p(libdpd2.Cluster(clusters, c_uint(i)))
            numObjs = int(libdpd2.GetNumObjects(clust))
            for j in range(numObjs):
                objPtr = c_void_p(libdpd2.ObjectFromCluster(clust, c_uint(j)))
                tempCluster.append(self.objectMap[str(libdpd2.GetGUID(objPtr))])
            self.clusterList.append(tempCluster)
        return
        
    def numClusters(self):
        for cluster in self.clusterList:
            for obj in cluster:
                print(str(obj))
        return len(self.clusterList)
    
    def destroy(self):
        libdpd2.DeleteBinBox(c_void_p(self.obj))
        libdpd2.DeleteObjList(c_void_p(self.objectList))
        return

'''
    Main function for testing.
'''
if __name__ == '__main__':
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
    
    try:
        solver.destroy()
    except NotImplementedError as e:
        print(e)
    finally:
        del solver
        
    
    dimensions = {'a':'stuff'}
    solver = BinBox(dimensions) # print error message
    
