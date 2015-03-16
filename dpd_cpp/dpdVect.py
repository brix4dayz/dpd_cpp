from ctypes import *

# Import custom dynamic library on Mac
libvect = cdll.LoadLibrary('./libvect.dylib')
# Import libc so can use a bunch of useful C functions in Python
# libc = cdll.LoadLibrary('/usr/lib/libSystem.B.dylib')

# For Unix use `ld` to make libvect.so
# libvect = cdll.LoadLibrary('./libvect.so')

########################################################
# Must declare return types of C interface functions

libvect.Random_PosVect_In_Box.restype = c_void_p

libvect.Make_PosVect.restype = c_void_p

libvect.New_PosVect.restype = c_void_p

libvect.Random_DirVect_With_Length.restype = c_void_p

libvect.DirVect_Btwn_Positions.restype = c_void_p

libvect.Make_DirVect.restype = c_void_p

libvect.Get_X.restype = c_double

libvect.Get_Y.restype = c_double

libvect.Get_Z.restype = c_double

libvect.Get_DX.restype = c_double

libvect.Get_DY.restype = c_double

libvect.Get_DZ.restype = c_double

libvect.Get_Distance_Mod.restype = c_double

libvect.RandomPercentage.restype = c_double

libvect.getCorrectedDistBtwnPos.restype = c_double

########################################################

########################################################
# Wrappers

# Python wrapper class for PosVect
class PosVect(object):
  # Make an object that points to nothing
  def __init__(self):
    self.obj = 0

  @classmethod
  def makePos(cls, x, y, z):
    self = cls() # makes empty object
    # Have object point to C++ object
    self.obj = libvect.Make_PosVect(c_double(x), c_double(y), c_double(z)) # convert all params to c_types
    return self

  @classmethod
  def randomPos(cls, box_length):
    self = cls()
    self.obj = libvect.Random_PosVect_In_Box(c_ubyte(box_length))
    return self

  @classmethod
  def posFromPosDir(cls, p, d):
    self = cls()
    self.obj = libvect.New_PosVect(c_void_p(p.obj), c_void_p(d.obj))
    return self

  @property
  def x(self):
    return libvect.Get_X(c_void_p(self.obj))

  @property
  def y(self):
    return libvect.Get_Y(c_void_p(self.obj))

  @property
  def z(self):
    return libvect.Get_Z(c_void_p(self.obj))

  def output(self):
    libvect.printPos(c_void_p(self.obj))

  # Test new functions
  def reset(self):
    libvect.resetPos(c_void_p(self.obj))

  def getPBCCorrectDistanceModForPhobes(self, other, box_length, micelle_cutoff):
    return libvect.getCorrectedDistBtwnPos(c_void_p(self.obj), c_void_p(other.obj), c_ubyte(box_length), c_float(micelle_cutoff))

  def add(self, other):
    libvect.addPos(c_void_p(self.obj), c_void_p(other.obj))

  def divideByScalar(self, scalar):
    libvect.dividePosByScalar(c_void_p(self.obj), c_int(scalar))

# Python wrapper class for DirVect
class DirVect(object):
  def __init__(self):
    self.obj = 0

  @classmethod
  def randomDir(cls, bond_length):
    self = cls()
    self.obj = libvect.Random_DirVect_With_Length(c_float(bond_length))
    return self

  @classmethod
  def makeDir(cls, dx, dy, dz):
    self = cls()
    self.obj = libvect.Make_DirVect(c_double(dx), c_double(dy), c_double(dz))
    return self

  @classmethod
  def dirBtwnPositions(cls, p1, p2):
    self = cls()
    self.obj = libvect.DirVect_Btwn_Positions(c_void_p(p1.obj), c_void_p(p2.obj))
    return self

  @property
  def dx(self):
    return libvect.Get_DX(c_void_p(self.obj))

  @property
  def dy(self):
    return libvect.Get_DY(c_void_p(self.obj))

  @property
  def dz(self):
    return libvect.Get_DZ(c_void_p(self.obj))

  @property
  def modulus(self):
    return libvect.Get_Distance_Mod(c_void_p(self.obj))

  def output(self):
    libvect.printDir(c_void_p(self.obj))

# Wrapper functions for additional C/C++ functions
def setSeedForC():
  libvect.Set_Seed()

def getRandomPercentageFromC():
  return libvect.RandomPercentage()

########################################################

# Test
def main():
  setSeedForC()
  
  r = PosVect.randomPos(36)
  r.output()
  
  d = DirVect.randomDir(0.1)
  d.output()
  
  r2 = PosVect.posFromPosDir(r, d)
  r2.output()
  
  print(str(getRandomPercentageFromC()))

  r.divideByScalar(-1)
  r.output()

  r2.add(r)
  r2.output()

  r.reset()
  r.output()

  print(str(r.getPBCCorrectDistanceModForPhobes(r2, 36, 3.5)))

if __name__ == "__main__":
  main()
