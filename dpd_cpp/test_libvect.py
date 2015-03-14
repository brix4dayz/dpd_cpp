from ctypes import *

# Import dynamic library on Mac
libvect = cdll.LoadLibrary('./libvect.dylib')

# For Unix use `ld` to make libvect.so
# libvect = cdll.LoadLibrary('./libvect.so')

# Must declare return types of C interface functions

libvect.Random_PosVect_In_Box.restype = c_void_p

libvect.Get_X.restype = c_double

libvect.Get_Y.restype = c_double

libvect.Get_Z.restype = c_double

# Python wrapper class
class PosVect(object):
  def __init__(self, box_length):
    self.obj = libvect.Random_PosVect_In_Box(box_length)

  @property
  def x(self):
    return libvect.Get_X(c_void_p(self.obj))

  @property
  def y(self):
    return libvect.Get_Y(c_void_p(self.obj))

  @property
  def z(self):
    return libvect.Get_Z(c_void_p(self.obj))

# Test
def main():
  r = PosVect(36)
  string = str(r.x) + " " + str(r.y) + " " + str(r.z)
  print(string)

if __name__ == "__main__":
  main()
 