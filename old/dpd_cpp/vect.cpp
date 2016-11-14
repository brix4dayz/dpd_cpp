#include "vect.h"
#include <cmath>
#include <cstdlib>

// Constants
#define PI 3.14159265

double randomRealC() {
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
// sources for static dice: 
// http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx
// http://stackoverflow.com/questions/6223355/static-variables-in-class-methods
PosVect::PosVect( idx* box_length ) {
  #if defined( TESTING )
  this->x = randomRealC() * ( *box_length - 2 ) + 1;
  this->y = randomRealC() * ( *box_length - 2 ) + 1;
  this->z = randomRealC() * ( *box_length - 2 ) + 1;
  #else
  static RealDice<double> positionDice( 1.0, ( (double) *box_length - 1 ) );
  this->x = positionDice.roll();
  this->y = positionDice.roll();
  this->z = positionDice.roll();
  #endif
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
  float absD = std::abs( *d );
  if ( absD >= ( *box_length - *micelle_cutoff ) )
    *d = *box_length - absD;
}

void PosVect::pbcCorrectDistCompForLink( double* d, 
 idx* box_length, const float& pbc_correction_factor ) {
  if ( std::abs( *d ) >= ( *box_length )*pbc_correction_factor ) {
    if ( *d > 0 )
      *d -= *box_length;
    else
      *d += *box_length;
  }
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

double PosVect::getLinkedDist( PosVect* r, idx* box_length, const float& pbc_correction_factor ) {
  DirVect* d = new DirVect( this, r );
  pbcCorrectDistCompForLink( &( d->dx ), box_length, pbc_correction_factor );
  pbcCorrectDistCompForLink( &( d->dy ), box_length, pbc_correction_factor );
  pbcCorrectDistCompForLink( &( d->dz ), box_length, pbc_correction_factor);
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

double PosVect::getDistSquared( PosVect* p ) {
  DirVect* d = new DirVect( this, p );
  double distSquared = d->modulus;
  delete d;
  return distSquared;
}

// Builds random direction with given length
// Adapted from random_polymerchain.m
DirVect::DirVect( float* bond_length ) {
  #if defined( TESTING )
  float angle1 = randomRealC() * 2 * PI;
  float angle2 = randomRealC() * 2 * PI;
  #else
  static RealDice<float> angleDice( 0.0f, 360.0f );
  float angle1 = angleDice.roll() * PI / 180.0f;
  float angle2 = angleDice.roll() * PI / 180.0f;
  #endif
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