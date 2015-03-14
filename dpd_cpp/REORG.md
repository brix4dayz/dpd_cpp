### Reorganization of dpd_cpp

Creating dynamically linked libraries in C/C++: <http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_C_libraries.html>  

Basically put multiple `.o` files into one `.so` file (shared object), which can then be linked using `-l`, and `-L` if the `.so` file is not in the standard path.  

Main directories (for now): *types*, *init_data*, *frames*, *processors*, *builders*

* *Types:*
	+ Data structures (**List, Bin, BinCube**) with generic types T ???
	+ **ObjDPD** - base class with virtual functions for properly managing memory.
	+ **PosVect, DirVect, Bead, Bond, Gyration, BeadGyration**
	+ _Static Containers_:
		- **BeadList, PolymerBlock, HydrophobicTail, Nanoparticle, DNABlock**
	+ _Bonds_:
		- **BondList, AngBond, AngBondList**
	+ **Molecule**'s:
		- _Polymers_:
			* **CopolymerChain, PECTriblock, PEChargeTriblock, PECLinkerQuadblock, SSDNAChain, DNAChain**
		- _Solids_: 
			* **Nanoparticle, Surfaces**
	+ _Dynamics Containers_:
		- **Bin, BeadBin, BondBin, TailBin, BlockBin, ChainBin, NPBin**
	+ _Complex Structures_:
		- **Micelle, DiblockMicelle, TriblockMicelle, HydrophobicCore**
* *Initial Data:*
	+ **DPDData**, **DPDPolymerData**
	+ **TriblockData**, **ChargeTriblockData**
* *Builders:*
	+ **DPDBuilder**, **TriblockBuilder**, **ChargeTriblockBuilder**
* *Frames:*
	+ **DPDFrame**, **CopolymerMicelleFrame**, **TriblockFrame**, 
	+ **BinCube** (Special HashMap for "cubic unit cell" simulations)
* *Processors:*
	+ **DPDTrajectory**, **CopolymerMicelleTrajectory**, **TriblockTrajectory**
