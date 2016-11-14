#include "vect.h"
#include <cmath>
#include <cstdlib>

// Constants
#define PI 3.14159265

double gauss() {
  return ( ( double ) rand() / RAND_MAX );
}

// Constructors
PosVect::PosVect( double x, double y, double z ) {
  this->x = x;
  this->y = y;
  this->z = z;
}

PosVect::PosVect(): PosVect( 0, 0, 0 ) {}

PosVect::PosVect( PosVect* pos, DirVect* d ) {
  this->x = pos->x + d->dx;
  this->y = pos->y + d->dy;
  this->z = pos->z + d->dz;
}

// Generates random point within a square box
// Adapted from gendata2.m
PosVect::PosVect( idx* box_length ) {
  this->x = gauss() * ( *box_length - 2 ) + 1;
  this->y = gauss() * ( *box_length - 2 ) + 1;
  this->z = gauss() * ( *box_length - 2 ) + 1;
}

PosVect::~PosVect() {}

void PosVect::print( FILE* fp ) {
  fprintf( fp, "%10.5f %10.5f %10.5f\n", this->x, this->y, this->z );
}

void DirVect::print( FILE* fp ) {
  fprintf( fp, "%10.5f %10.5f %10.5f\n", this->dx, this->dy, this->dz );  
}

void PosVect::reset() {
  this->x = 0;
  this->y = 0;
  this->z = 0;
}

void PosVect::pbcCorrectDistanceCompForCluster( double* d, 
 idx* box_length, float* micelle_cutoff ) {
  float absD = fabsf( *d );
  if ( absD >= ( *box_length - *micelle_cutoff ) )
    *d = *box_length - absD;
}

double PosVect::getCorrectedDist( PosVect* r, idx* box_length, 
 float* micelle_cutoff ) {
  DirVect* d = new DirVect( this, r );
  pbcCorrectDistanceCompForCluster( &( d->dx ), box_length, micelle_cutoff );
  pbcCorrectDistanceCompForCluster( &( d->dy ), box_length, micelle_cutoff );
  pbcCorrectDistanceCompForCluster( &( d->dz ), box_length, micelle_cutoff );
  d->calcMod();
  double dist = d->modulus;
  delete d;
  return dist;
}

void PosVect::addCoords( PosVect* r ) {
  this->x += r->x;
  this->y += r->y;
  this->z += r->z;
}

void PosVect::divideCoords( int* value ) {
  this->x /= *value;
  this->y /= *value;
  this->z /= *value;
}

// Builds random direction with given length
// Adapted from random_polymerchain.m
DirVect::DirVect( float* bond_length ) {
  float angle1 = gauss() * 2 * PI;
  float angle2 = gauss() * 2 * PI;
  float height = std::cos( angle2 ) * *bond_length;
  this->dx = std::sin( angle1 ) * height;
  this->dy = std::cos( angle1 ) * height;
  this->dz = std::sin( angle2 ) * *bond_length;
  this->calcMod();
}

// Builds direction given two position vectors
DirVect::DirVect( PosVect* pos1, PosVect* pos2 ): DirVect(
  pos2->x - pos1->x,
  pos2->y - pos1->y,
  pos2->z - pos1->z ) {}

// Builds direction given two position vectors
DirVect::DirVect( double dx, double dy, double dz ) {
  this->dx = dx;
  this->dy = dy;
  this->dz = dz;
  this->calcMod();
}

// Deconstructor, don't worry about this.
DirVect::~DirVect() {}

void DirVect::calcMod() {
  this->modulus = this->dx * this->dx + this->dy * this->dy + this->dz * this->dz;
}