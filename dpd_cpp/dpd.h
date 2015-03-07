// Library for building and processing DPD simulations.
// Copyirght (c) 2015
// author: Hayden Fuss
// Yinglab labs


// a single byte to store integer values between 0 - 255
// when printing must cast to short in order to see integer value
// we'll have alot of beads so we want to try to minimize their
// size in memory
typedef unsigned char idx;

#define FLUID_DENSITY 3

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

// Abstract class for DPD frame processors to inherent from
class DPDFrame {};

class CopolymerMicelleFrame;

class TriblockFrame;

class DPDData {};

class TriblockData;

class DPDPolymerData;

class ChargeTriblockData;
