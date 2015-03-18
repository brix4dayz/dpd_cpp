#include "hydrophobicCore.h"

class Micelle : public ObjDPD {
	public:
		unsigned short aggreg_num;
		std::vector< HydrophobicCore* > coreList;
		DPDFrame* frame;
		PosVect* com;
		Micelle( DPDFrame* frame );
		Micelle();
		~Micelle();
		void addCore( HydrophobicCore* core );
		void printMicelleCore( FILE* stream );
		void unlink();
		virtual void pbcCorrectMicelle( idx* box_length ) {}
		virtual void calcCenterOfMass( idx* box_length ) {}
		virtual void printMicelle( FILE* stream ) {}
		virtual void deriveChainList() {}
};

class TriblockMicelle : public Micelle {
	public:
		std::vector< PECTriblock* > chainList;
		//double avg_distance_btwn_cores;
		TriblockMicelle();
		TriblockMicelle( DPDFrame* frame );
		~TriblockMicelle();
		void printMicelle( FILE* stream );
		void addChain( PECTriblock* chain );
		void deriveChainList();
    void pbcCorrectMicelle( idx* box_length );
		void calcCenterOfMass( idx* box_length );
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
		DiblockMicelle( DPDFrame* frame );
		~DiblockMicelle();
		void printMicelle( FILE* stream );
		void deriveChainList();
    void pbcCorrectMicelle( idx* box_length );
		void calcCenterOfMass( idx* box_length );
};