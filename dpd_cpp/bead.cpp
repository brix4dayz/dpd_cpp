#include "bead.h"
#include <iostream>
#include <cmath>
#include <sstream>

double Bead::getDistanceModulus( Bead* other ) {
  DirVect* distance = new DirVect( this->r, other->r );
  double dist = distance->modulus;
  delete distance;
  return dist;
}

void Bead::pbcCorrectDistanceCompInChain( double* d, double* coord, 
 idx* box_length, const float& pbc_correction_factor ) {
	if ( std::abs( *d ) >= ( *box_length )*pbc_correction_factor ) {
		if ( *d > 0 )
			*coord -= *box_length;
		else
			*coord += *box_length;
	}
}

double Bead::getPbcCorrectedDistance( Bead* other, idx* box_length, 
                                      float* micelle_cutoff ) {
  return this->r->getCorrectedDist( other->r, box_length, micelle_cutoff );
}

void Bead::pbcCorrectBeadInChain( Bead* base, idx* box_length, const float& pbc_correction_factor ) {
  DirVect* d = new DirVect( base->r, this->r );
	pbcCorrectDistanceCompInChain( &( d->dx ), &( this->r->x ), box_length, pbc_correction_factor );
	pbcCorrectDistanceCompInChain( &( d->dy ), &( this->r->y ), box_length, pbc_correction_factor );
	pbcCorrectDistanceCompInChain( &( d->dz ), &( this->r->z ), box_length, pbc_correction_factor );
  delete d;
}

void Bead::printBead( FILE *stream ) {
	fprintf( stream, "%d ", this->type );
  this->r->print( stream );
}

void Bead::printData( FILE* stream ) {
  fprintf( stream, "%d %d %e %e %e %d\n", this->id, this->type, this->r->x,
           this->r->y, this->r->z, this->mol_id );
}

Bead::Bead() {
  this->r = new PosVect( 0, 0 , 0 );
	this->type = 0;
}

Bead::Bead( std::string line ) {
  //std::cout << line << std::endl;
	std::istringstream iss(line);
  double x, y, z;
	iss >> this->type >> x >> y >> z;
	this->type = this->type - '0'; //correct for char
  this->r = new PosVect( x, y, z );
}

Bead::Bead( PosVect* r, idx type ) {
  this->r = r;
  this->type = type;
}

Bead::Bead( PosVect* r, idx type, unsigned int* id,
            unsigned int mol_id ) : Bead( r, type ) {
  this->id = *id;
  *id += 1;
  this->mol_id = mol_id;
}

Bead::Bead( double x, double y, double z, idx type ): Bead( new PosVect( x, y, z ), type ) {}
		
Bead::Bead( double x, double y, double z, idx type, std::string name ): Bead( x, y, z, type ) {
  this->type_name = name;
}

void Bead::unlink() {
  this->r = NULL;
}

Bead::~Bead() {
  delete this->r;
  this->unlink();
}

void Bead::reset() {
	this->r->x = 0;
	this->r->y = 0;
	this->r->z = 0;
}

void Bead::addBeadCoords( Bead* other ) {
  this->r->addCoords( other->r );
}

void Bead::divideCoords( int* value ) {
  this->r->divideCoords( value );
}

Gyration::Gyration() {
  this->d = NULL;
  this->sxx = 0;
  this->syy = 0;
  this->szz = 0;
  this->sxy = 0;
  this->sxz = 0;
  this->syz = 0;
}

Gyration::Gyration( DirVect* d ) {
  this->d = d;
	this->sxx = this->d->dx*this->d->dx;
	this->syy = this->d->dy*this->d->dy;
	this->szz = this->d->dz*this->d->dz;
	this->sxy = this->d->dx*this->d->dy;
	this->sxz = this->d->dx*this->d->dz;
	this->syz = this->d->dy*this->d->dz;
}

Gyration::~Gyration() {
  delete d;
  this->unlink();
}

void Gyration::unlink() {
  this->d = NULL;
}

Gyration::Gyration( double dx, double dy, double dz ): Gyration( new DirVect( dx, dy, dz ) ) {}

BeadGyration::BeadGyration(): Gyration() {}

BeadGyration::BeadGyration( DirVect* d ): Gyration( d ) {}

BeadGyration::BeadGyration( Bead* b, PosVect* com ): BeadGyration( new DirVect( b->r, com ) ) {}

BeadGyration::~BeadGyration() {}

Bond::Bond() {}

Bond::Bond( idx type, unsigned int id, Bead* bead1, Bead* bead2) {
	this->type = type;
	this->id = id;
	this->bead1 = bead1;
	this->bead2 = bead2;
}

void Bond::unlink() {
  this->bead1 = NULL;
  this->bead2 = NULL;
}

Bond::~Bond() {
  delete this->bead1;
  delete this->bead2;
  this->unlink();
}

void Bond::printBond( FILE* fp ) {
	fprintf(fp, "%d %d %d %d\n", this->id, this->type, this->bead1->id, this->bead2->id );
}


//if you compile bead.cpp with the following commands then the
//it can be tested:  g++ -Wall -c beadGyration.cpp
// g++ -Wall -DTESTING beadGyration.o bead.cpp -o bead
// -DTESTING "turns on" main function
//the following main function contains examples of how to use these functions
#if defined(TESTING)

#include <fstream>


int main() {

	Bead *b1 = new Bead(0, 3, 4, 1);
	Bead *b2 = new Bead(0, 3, -4, 1);
	Bead *com = new Bead(0.0, 0.0, 0.0, 1);
  float pbc_correction_factor = 0.5f;

	//Test Distance Modulus
	printf("%10.5f %10.5f\n", b1->getDistanceModulus( com ), 
		b2->getDistanceModulus( com ) );

	//Test Bead Gyration
	BeadGyration* result = new BeadGyration( com, b1->r );

	printf( "%10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\n", result->sxx, result->syy,
		result->szz, result->sxy, result->sxz, result->syz );
	printf( "%10.5f\n", result->d->modulus );

	//Test PBC-Bead Correct
	idx *box_length = new idx(7);
	b1->pbcCorrectBeadInChain( com, box_length, pbc_correction_factor ); //should now be (0, 3, -3)
	b2->pbcCorrectBeadInChain( com, box_length, pbc_correction_factor ); //should now be (0, 3, 3)

	b1->printBead( stdout );
	b2->printBead( stdout );
	b1->id = 101;
	b2->id = 102;
	Bond* bond1 = new Bond( 1, 103, b1, b2 );
	bond1->printBond( stdout );

	//Test PBC-Distance Correct
	float *micelle_cutoff = new float(3.0);
	printf("%10.5f\n", b1->getPbcCorrectedDistance( com, box_length, 
		                                              micelle_cutoff ) );

	std::ifstream infile("bead_test.txt");
	std::string line;
	std::getline(infile, line);

	Bead *fileTest = new Bead( line );
	fileTest->printBead( stdout );

	infile.close();

	delete fileTest;
	delete b1;
	delete b2;
	delete com;
	delete micelle_cutoff;
	delete box_length;
  bond1->unlink();
  delete bond1;
  delete result;

	return 0;
}

#endif