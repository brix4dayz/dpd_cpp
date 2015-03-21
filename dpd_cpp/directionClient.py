import dpdVect
import sys

"""

"""
def main():
  #
  dpdVect.setSeedForC()

  #
  numVects = int(sys.argv[1])

  #
  vectLength = float(sys.argv[2])

  #
  molVar = str(sys.argv[3])

  #
  for i in range(0, numVects):
    k = numVects - i
    direction = dpdVect.DirVect.randomDir( vectLength )
    print("translate " + molVar + "." + str(k) + " { " + str(direction.dx) +  " " + str(direction.dy) + " " + str(direction.dz) + " }")

if __name__ == "__main__":
  main()