#include "vect.h"

// considering storing xyz as floats?

#define HYDROPHOBIC 1
#define HYDROPHILIC 2

class Gyration : public ObjDPD {
	public:
		DirVect* d;
		double sxx;
		double syy;
		double szz;
		double sxy;
		double sxz;
		double syz;
    Gyration();
		Gyration( double dx, double dy, double dz );
		Gyration( DirVect* d );
		~Gyration();
		void unlink();
};

class BeadGyration : public Gyration {
	public:
    BeadGyration();
		BeadGyration( DirVect* d );
		BeadGyration( Bead* b, PosVect* com );
    ~BeadGyration();
};

class Bead : public ObjDPD {
	private:
		void pbcCorrectDistanceCompInChain( double* d, double* coord, 
			                                  idx* box_length, const float& pbc_correction_factor );
		void pbcCorrectDistanceCompForCluster( double* d, idx* box_length,
		 	                                     float* micelle_cutoff );
	public:
		//Fields (data)
		PosVect* r;
		idx type;
		unsigned int id;
		unsigned int mol_id;
		std::string type_name;
		//Constructor/Destructor
		Bead( PosVect* r, idx type );
    Bead( PosVect* r, idx type, unsigned int* id, unsigned int mol_id);
    Bead( double x, double y, double z, idx type );
		Bead( double x, double y, double z, idx type, 
			    std::string name );
		Bead();
		Bead( std::string line );
		~Bead();
		//Functions
		double getDistanceModulus( Bead* other );
		void pbcCorrectBeadInChain( Bead* base, idx* box_length, const float& pbc_correction_factor );
		double getPbcCorrectedDistance( Bead* other, idx* box_length, 
			                              float* micelle_cutoff );
		void printBead( FILE* stream );
		void printData( FILE* stream );
		void reset();
    void addBeadCoords( Bead* other );
    void divideCoords( int* value );
    void unlink();
};

class Bond : public ObjDPD {
	public:
		idx type;
		unsigned int id;
		Bead* bead1;
		Bead* bead2;
		Bond();
		Bond( idx type, unsigned int id, Bead* bead1, Bead* bead2);
		~Bond();
		void printBond( FILE* fp );
		void unlink();
};