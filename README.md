## _dpd_cpp_
##### _Copyright (c) 2015_
##### _author: Hayden Fuss_
##### _Yingling labs_


A library of C++ classes designed for building and processing **DPD** simulations, with several classes implemented specifically for block copolymer solutions.

There are generic classes such as **PosVect** (position vector), **DirVect** (direction vector), **Bond**, **Bead** (atom), **Gyration**, **PolymerBlock**, **CopolymerChain**, **Bin**, **DPDFrame** (trajectory processor), **DPDData** (builder).

Also has specific implementations descending from these classes: **BeadGyration**, **HydrophobicTail**, **PECTriblock**, **HydrophobicCore**, **Micelle**, **DPDPolymerData** (aqueous polymer builder), **TriblockData** (builder), **ChargeTriblockData** (builder), **DiblockMicelle**, **TriblockMicelle**, **CopolymerMicelleFrame** (processor), **TriblockFrame** (processor).

#### __Current Objectives__:  
* Finish **TriblockFrame** and its parent classes.  
* Reorganize **DPDData**, **DPDFrame**, and **Bin** classes as parent classes.   
   + **CopolymerMicelleFrame** inherents from **DPDFrame**, should have **TailBin** cube instead **Bin**'s.  
   + Builders could use **BondBin** cube for fluid collision detection.  
   + **Bond**'s need com and reference to molecule.  
   + **AngBond** could be class containing two **Bond**'s.   
* Implement **Nanoparticle** class.  
* Add collision detection to builders using **Bin**'s.  
* Organize library and executables into application.  
* Develop Python GUI to handle inputs and call executables to generate and process simulations.  
* Finish _unlink_ and _deconstructors_ for objects to prevent memory leaks.  
* Consider using _references_ in some places instead of _pointers_.  
* Write code which can read output files produced by other software into data, process/edit/manipulate that data and print it in a friendly format.  
* Make generic classes for calculating measurements for **DPD** simulation objects such persistence length of a chain, or radius of gyration for micelles, etc.
