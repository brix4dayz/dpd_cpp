#include <string>
#include <cstdio>
#include "dpd.h"

// A position vector is another word for a point in 3d
// Mathematically a vector is expressed like this: r = < x, y, z >
// A direction vector is another term for a distance between two 3d points
// A direction vector can be described as follows: d = r2 - r1 = < dx, dy, dz >
// where r1 = < x1, y1, z1 > ; r2 = < x2, y2, z2 >
// and dx = x2 - x1 ; dy = y2 - y1 ; dz = z2 - z1

class ObjDPD {
  public:
    virtual void unlink() {}
    virtual ~ObjDPD() {}
};

class PosVect {
  private:
    void pbcCorrectDistanceCompForCluster( double* d, idx* box_length,
                                           float* micelle_cutoff );
  public:
    double x;
    double y;
    double z;
    PosVect( double x, double y, double z ); // Creates a position vector
    PosVect( PosVect* pos, DirVect* d ); // Creates a position vector from another position and given direction
    PosVect( idx* box_length ); // Create a random point within a 3d cubic space of a given length
    PosVect();
    ~PosVect();
    void print( FILE* fp );
    void reset();
    void addCoords( PosVect* r );
    void divideCoords( int* value );
    double getCorrectedDist( PosVect* r, idx* box_length, float* micelle_cutoff );
};

class DirVect {
  public:
    // Data
    double dx; //x2 - x1
    double dy; //y2 - y1
    double dz; //z2 - z1
    double modulus; // distance squared
    // Behavior
    DirVect( float* bond_length ); // Creates a random direction of given length
    DirVect( PosVect* pos1, PosVect* pos2 ); // Creates direction vector given two position vectors
    DirVect( double dx, double dy, double dz );
    ~DirVect();
    void calcMod();
};