#include "hydrophobicCore.h"

class Micelle : public ObjDPD {
	public:
		unsigned short aggreg_num;
		std::vector< HydrophobicCore* > coreList;
		DPDFrame<CopolymerChain>* frame;
		PosVect* com;
		Micelle( DPDFrame<CopolymerChain>* frame );
		Micelle();
		~Micelle();
		void addCore( HydrophobicCore* core );
		void printMicelleCore( FILE* stream );
		void unlink();
		virtual void pbcCorrectMicelle( idx* box_length, const float& pbc_correction_factor ) {}
		virtual void calcCenterOfMass( idx* box_length, const float& pbc_correction_factor ) {}
		virtual void printMicelle( FILE* stream ) {}
		virtual void deriveChainList() {}
};

class TriblockMicelle : public Micelle {
	public:
		std::vector< PECTriblock* > chainList;
		TriblockMicelle();
		TriblockMicelle( DPDFrame<CopolymerChain>* frame );
		~TriblockMicelle();
		void printMicelle( FILE* stream );
		void addChain( PECTriblock* chain );
		void deriveChainList();
    void pbcCorrectMicelle( idx* box_length, const float& pbc_correction_factor );
		void calcCenterOfMass( idx* box_length, const float& pbc_correction_factor );
		void unlink();
};

class DiblockMicelle : public Micelle {
	public:
		// Diblock
		double radius_of_gyration;
		double shape_anisotropy;
		double asphericity;
		double corona_thickness;
		std::vector< CopolymerChain* > chainList;
		DiblockMicelle();
		DiblockMicelle( DPDFrame<CopolymerChain>* frame );
		~DiblockMicelle();
		void printMicelle( FILE* stream );
		void deriveChainList();
    void pbcCorrectMicelle( idx* box_length, const float& pbc_correction_factor );
		void calcCenterOfMass( idx* box_length, const float& pbc_correction_factor );
};