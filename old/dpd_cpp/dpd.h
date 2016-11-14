// Library for building and processing DPD simulations.
// Copyirght (c) 2015
// author: Hayden Fuss
// Yinglab labs

// a single byte to store integer values between 0 - 255
// when printing must cast to short in order to see integer value
// we'll have alot of beads so we want to try to minimize their
// size in memory
typedef unsigned char byte;

// index, 0 - 65,535 values, cant be negative
typedef unsigned short idx;

#define FLUID_DENSITY 3

#define DEFAULT_PBC 0.5f

double randomRealC();

// Class prototypes

class PosVect;

class DirVect;

class Bead;

class Bond;

class Gyration;

class BeadGyration;

class PolymerBlock;

class HydrophobicTail;

class CopolymerChain;

class PECTriblock;

class Bin;

class HydrophobicCore;

class Micelle;

class TriblockMicelle;

class DiblockMicelle;

class TriblockData;

class DPDPolymerData;

class ChargeTriblockData;

class Stem;

class ObjDPD {
  public:
    virtual void unlink() {}
    virtual ~ObjDPD() {}
};

// Abstract class for DPD frame processors to inherent from
template <class C>
class DPDFrame {
  public:
    virtual void unlink() {}
    virtual ~DPDFrame() {}
};

class CopolymerMicelleFrame;

class TriblockFrame;

class DPDTrajectory;

class DPDData : public ObjDPD {
  public:
    void unlink() {}
    ~DPDData() {}
};
