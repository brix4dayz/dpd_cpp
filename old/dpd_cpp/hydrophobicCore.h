#include "bin.h"

class HydrophobicCore : public ObjDPD {
	public:
		Micelle* micelle;
		std::vector< Bin* > binList;
		PosVect* com;
		bool grouped;
		unsigned short aggregation_num;
		unsigned short num_tails;
		double radius_of_gyration;
		double shape_anisotropy;
		double asphericity; 
		HydrophobicCore();
		~HydrophobicCore();
		void addBin( Bin* bin ); //calcs agg number, set grouped to true
		bool groupCores( HydrophobicCore* core ); //go through all tails in this core,
		// if tail's other's bin doesn't match theirs, then see if their other's bin
		// matches any of the bins in the other core
		void calcCenterOfMass( idx* box_length, const float& pbc_cor_factor );
		void printCore( FILE* stream );
		void unlink();
		void color( idx type );
};