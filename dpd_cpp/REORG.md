### Reorganization of dpd_cpp

Creating dynamically linked libraries in C/C++: <http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_C_libraries.html>  

Basically put multiple `.o` files into one `.so` file (shared object), which can then be linked using `-l`, and `-L` if the `.so` file is not in the standard path.  

Main directories (for now): *types*, *init_data*, *frames*, *processors*, *builders*

* *Types:*
* Data structures (**List, Bin, BinCube**) with generic types T
	+ **ObjDPD** - base class with virtual functions for properly managing memory.
	+ **PosVect, DirVect, Bead, Bond, Gyration, BeadGyration**
	+ **BeadList, PolymerBlock, HydrophobicTail, Nanoparticle, DNABlock**
	+ **BondList, AngBond, AngBondList**
	+ **Molecule, CopolymerChain, PECTriblock, SSDNAChain, DNAChain, Nanopartcle**
	+ **Bin, BeadBin, BondBin, TailBin, BlockBin, ChainBin, NPBin**
	+ **BinCube** (Special HashMap for "cubic unit cell" simulations)
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
