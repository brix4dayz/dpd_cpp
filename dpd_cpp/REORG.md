### Reorganization of dpd_cpp

Creating dynamically linked libraries in C/C++: <http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_C_libraries.html>  

Basically put multiple `.o` files into one `.so` file (shared object), which can then be linked using `-l`, and `-L` if the `.so` file is not in the standard path.  

Main directories (for now): *types*, *init_data*, *frames*, *processors*, *builders*

* *Types:*
	+ **ObjDPD** - base class with virtual functions for properly managing memory.
	+ **PosVect, DirVect, Bead, Bond, Gyration, BeadGyration**
	+ **BeadList, PolymerBlock, HydrophobicTail, Nanoparticle, DNABlock**
	+ **Molecule, CopolymerChain, PECTriblock, SSDNAChain, DNAChain, Nanopartcle**
	+ **Bin, BeadBin, BondBin, TailBin, BlockBin, ChainBin, NPBin**
	+ **Micelle, DiblockMicelle, TriblockMicelle, HydrophobicCore**
* *Initial Data:*
	+ **DPDData**, **DPDPolymerData**
	+ **TriblockData**, **ChargeTriblockData**
* *Builders:*
	+ **DPDBuilder**, **TriblockBuilder**, **ChargeTriblockBuilder**
* *Frames:*
	+ **DPDFrame**, **CopolymerMicelleFrame**, **TriblockFrame**
* *Processors:*
	+ **DPDTrajectory**, **CopolymerMicelleTrajectory**, **TriblockTrajectory**