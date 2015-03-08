# dpd_cpp
#### Copyright (c) 2015
##### author: Hayden Fuss
##### Yingling labs


A library of C++ classes designed for building and processing DPD simulations, with several classes implemented specifically for block copolymer solutions.

There are generic classes such as PosVect (position vector), DirVect (direction vector), Bond, Bead (atom), Gyration, PolymerBlock, CopolymerChain, DPDFrame (trajectory processor), DPDData (builder).

Also has specific implementations descending from these classes: HydrophobicTail, PECTriblock, HydrophobicCore, Micelle, DPDPolymerData (aqueous polymer builder), TriblockData (builder), ChargeTriblockData (builder), DiblockMicelle, TriblockMicelle, CopolymerMicelleFrame (processor), TriblockFrame (processor).

__Current__ __Objectives__:
* Finish TriblockFrame and its parent classes.
* Reorganize dpdData, dpdFrame, and bin classes as parent classes.
* Implement Nanoparticle class.
* Add collision detection to builders using bins.
* Organize library and executables into application.
* Develop Python GUI to handle inputs and call executables to generate and process simulations.
* Finish unlink and deconstructors for objects to prevent memory leaks.
* Consider using references in some places instead of pointers.
