#include <string>
#include <cstdio>
#include "dpd.h"
#include "dice.h"

// Primitive data pointers shold be references

class DPDVect : public ObjDPD {
  private:
    float *a;
    idx dimensions;
  public:
    DPDVect();
    DPDVect( idx dimensions );
    ~DPDVect();
    void unlink();
    void print( FILE* fp );
    void add( const DPDVect& v );
    void divideByScalar( const int& value );
    void multiplyByScalar( const int& value );
    void normalize();
    float magnitude();
    float modulus();
    float dot();
};

class Vect3D : public DPDVect {
    public:
        Vect3D( float a1, float a2, float a3 );
};

class Vect4D : public DPDVect {
    public:
        Vect4D( float a1, float a2, float a3, float a4 );
};

// rotation quaterion or unit quaterion
class RotQuat : public Vect4D {
    public:
        RotQuat( PosVect p1, PosVect p2 );
        RotQuat( float theta, DirVect n );
        RotQuat* hamiltonianProdut( RotQuat* other );
        DirVect* rotate( DirVect* d );
        void inverse();
};

class PosVect { // Position Vector class
  private:
    void pbcCorrectDistanceCompForCluster( double* d, idx* box_length,
                                           float* micelle_cutoff );
    void pbcCorrectDistCompForLink( double* d, idx* box_length, const float& pbc_correction_factor );
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
    double getDistSquared( PosVect* p );
    double getLinkedDist( PosVect* r, idx* box_length, const float& pbc_correction_factor );
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
#include <random>

double randomRealCPP() {
  static std::random_device seed;
  static std::mt19937 gen(seed());
  static std::uniform_real_distribution<double> dist(0.0, 1.0);
  static auto fraction = std::bind( dist, gen );
  return fraction();
}

extern "C" {

    void Set_Seed() {
        #if defined(TESTING)
        srand( 2 );
        #else
        srand( time( NULL ) );
        #endif
    }

    double RandomPercentage() {
        #if defined(TESTING)
        return randomRealC();
        #else
        return randomRealCPP();
        #endif
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

    void resetPos( PosVect* r ) {
        r->reset();
    }

    double getCorrectedDistBtwnPos( PosVect* r1, PosVect* r2, idx box_length, float micelle_cutoff ) {
        return r1->getCorrectedDist( r2, &box_length, &micelle_cutoff );
    }

    void addPos( PosVect* r1, PosVect* r2 ) {
        r1->addCoords( r2 );
    }

    void dividePosByScalar( PosVect* r, int value ) {
        r->divideCoords( &value );
    }

}



#endif