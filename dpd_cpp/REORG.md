### Reorganization of dpd_cpp

Creating dynamically linked libraries in C/C++: <http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_C_libraries.html>  

Basically put multiple `.o` files into one `.so` file (shared object, or `.dylib` for Mac), which can then be linked using `-l`, and `-L` if the `.so` file is not in the standard path.  

Using C++ libraries within Python without 3rd party dependencies: <http://stackoverflow.com/questions/145270/calling-c-c-from-python>
<http://stackoverflow.com/questions/23713949/how-to-expose-c-class-to-python-without-using-boost>
<https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch01s05.html>

Using this `.so` file to contain the library, the library can then be imported to Python using `extern "C"` functions, a C interface I will have to write to communicate with these C++ objects. Once the interface is written and the `.so` file is compiled, wrapper Python classes can be written using Python's "natural" `ctypes`: 
     
     # Get cdll and other C data types for Python
     from ctypes import *  
     # Open library, either `.so` or `.dylib` file (Linux or Mac)
     library = cdll.LoadLibrary('path to .so file')   
     
     # Use extern "C" function, must declare it's C return type  
     library.externFunction.restype = c_void_p  
     library.externFunction()   


Needs to be organized into folders/directories:

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
			* **Nanoparticle, Surfaces, Silica, Nanotube**
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
