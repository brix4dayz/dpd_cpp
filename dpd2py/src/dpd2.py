'''
Created on Mar 3, 2016

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
if (len(argv) != 1):
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

'''
    "Abstract" class that requires destroy to be implemented for pointer clean up.
'''
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
   
    '''
        Frees the SimulationObject* that is behind
        the scenes in C++. The Python SimulationObject will still hold useful
        data such as the position and GUID.
    '''
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
        # box dimensions requirement
        if (isinstance(boxDimensions, dict) and ('x' in boxDimensions.keys()) and
         ('y' in boxDimensions.keys()) and ('z' in boxDimensions.keys())):
            self.binSize = binSize
            self.objectList = None
            self.cutoffDist = cutoffDist
            self.objectMap = None
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
        self.objectList = libdpd2.SimObjList()
        self.objectMap = {}
        
        # build vector<SimulationObject*> for BinBox
        for sObj in sObjList:
            if isinstance(sObj, SimulationObject):
                self.addObj(sObj)
            else:
                print("python: " + sObj.__class__.__name__ + " is not a SimulationObject.")
        
        # pass vector to BinBox and derive vector<Cluster*>
        libdpd2.DeriveClusters(c_void_p(self.obj), c_void_p(self.objectList))
        
        # retrieve vector<Cluster*> from BinBox
        clusters = c_void_p(libdpd2.GetClusters(c_void_p(self.obj)))
        
        # build array of arrays containing SimulationObjects,
        # each array is a cluster
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
            
        libdpd2.DeleteObjList(c_void_p(self.objectList))
        del self.objectList
        self.objectList = None
        del self.objectMap
        self.objectMap = None
        
        return self.clusterList
        
    def numClusters(self):
#         for cluster in self.clusterList:
#             for obj in cluster:
#                 print(str(obj))
        return len(self.clusterList)
    
    def empty(self):
        libdpd2.EmptyBinBox(c_void_p(self.obj))
        return
    
    def destroy(self):
        libdpd2.DeleteBinBox(c_void_p(self.obj))
        del self.obj
        return

'''
    Utility function for calling destroy on an instance of
    SimulationObject.
'''
def freeObjList(objL):
    if isinstance(objL, list):
        for obj in objL:
            if isinstance(obj, SimulationObject):
                obj.destroy()
    return

#TODO: class Bead(SimulationObject):
