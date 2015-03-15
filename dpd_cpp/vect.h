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

// Vector3 from C#/Unity is an array with three doubles

class PosVect { // Position Vector class
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

class DirVect { // Direction Vector class
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
    void print( FILE* fp );
    ~DirVect();
    void calcMod();
};

// C-style interface to use C++ objects in Python via a DLL
#if defined(PYTHON_LIB)
#include <ctime>
#include <cstdlib>

extern "C" {

    void Set_Seed() {
        #if defined(TESTING)
        srand( 2 );
        #else
        srand( time( NULL ) );
        #endif
    }

    double RandomPercentage() {
        return gauss();
    }

    PosVect* Random_PosVect_In_Box( idx box_length ) {
        return new PosVect( &box_length );
    }

    PosVect* Make_PosVect( double x, double y, double z ) {
        return new PosVect( x, y, z );
    }

    PosVect* New_PosVect( PosVect* p, DirVect* d ) {
        return new PosVect( p, d );
    }

    DirVect* Random_DirVect_With_Length( float bond_length ) {
        return new DirVect( &bond_length );
    }

    DirVect* DirVect_Btwn_Positions( PosVect* p1, PosVect* p2 ) {
        return new DirVect( p1, p2 );
    }

    DirVect* Make_DirVect( double dx, double dy, double dz ) {
        return new DirVect( dx, dy, dz );
    }

    double Get_X( PosVect* ptr ) {
        return ptr->x;
    }

    double Get_Y( PosVect* ptr ) {
        return ptr->y;   
    }

    double Get_Z( PosVect* ptr ) {
        return ptr->z;
    }

    void printPos( PosVect* r ) {
        r->print( stdout );
    }

    double Get_DX( DirVect* ptr ) {
        return ptr->dx;
    }

    double Get_DY( DirVect* ptr ) {
        return ptr->dy;   
    }

    double Get_DZ( DirVect* ptr ) {
        return ptr->dz;
    }

    double Get_Distance_Mod( DirVect* ptr ) {
        return ptr->modulus;
    }

    void printDir( DirVect* d ) {
        d->print( stdout );
    }
}

#endif