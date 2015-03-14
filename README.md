## dpd_cpp
##### _Copyright (c) 2015, Yingling labs_
##### author: _Hayden Fuss_
##### _adapted from MATLAB and Perl scripts made by Nan Li and Hayden Fuss:_
+ gendata2.m
+ randomPolymerchain.m
+ getLastFrames_Diblock.pl
+ triblock_linkage.pl
+ silica_chains.pl
+ silica_micelles.pl  

#### _Description_
A library of C++ classes designed for building and processing **DPD** simulations, with several classes implemented specifically for block copolymer solutions.

There are generic classes such as **PosVect** (position vector), **DirVect** (direction vector), **Bond**, **Bead** (atom), **Gyration**, **PolymerBlock**, **CopolymerChain**, **Bin**, **DPDFrame** (trajectory processor), **DPDData** (generator), **DPDBuilder**.

Also has specific implementations descending from these classes: **BeadGyration**, **HydrophobicTail**, **PECTriblock**, **HydrophobicCore**, **Micelle**, **DPDPolymerData** (aqueous polymer builder), **TriblockData** (builder), **ChargeTriblockData** (builder), **DiblockMicelle**, **TriblockMicelle**, **CopolymerMicelleFrame** (processor), **TriblockFrame** (processor).

#### _Current Objectives_
* Finish _unlink_ and _deconstructors_ for objects to prevent memory leaks.  
* Finish **TriblockFrame** and its parent classes.  
* Reorganize **DPDData**, **DPDFrame**, and **Bin** classes as parent classes.   
   + **CopolymerMicelleFrame** inherents from **DPDFrame**, should have **TailBin** hash map instead of the now generic **Bin**'s.  
   + Builders could use **BondBin** cube for fluid collision detection.  
   + **Bond**'s need **PosVect** com and **Molecule**  reference.  
   + **AngBond** could be class containing two **Bond**'s.   
* Implement **Nanoparticle, Surface,** and **Layer** classes, etc.  
* Add collision detection to builders using **Bin**'s.  
* Organize library and executables into application.  
* Develop Python GUI to handle inputs and call executables to generate and process simulations.  
   + See `DPD/MD Engine`
* Consider using _references_ in some places instead of _pointers_.  
* Write code which can read output files produced by other software into data, process/edit/manipulate that data and print it in a friendly format.
   + See `DPD/MD Engine`
* Make generic classes for calculating measurements for **DPD** simulation objects such persistence length of a chain, or radius of gyration for micelles, etc.  
* Could make **ParticleBin**'s who determine their size based on the size of the **Nanoparticle**'s, which can be useful for preventing _fluidCollision_ in _deriveFluid_.  
* **Molecule** and **Macromolecule** parent classes.. Essentially just a list of beads for **Molecule**.  
   + **PolymerBlock** could extend **Molecule** and **CopolymerChain** could extend **Macromolecule**.
* Organize library into folders: _types_, _builders_, and _processors_.
* Look for redundant calculations within certain objects that can be given to their handlers (i.e. _num_atoms_ in **TriblockFrame** could be handled by **TriblockProcessor** which has a list of **TriblockFrames**).
* Incorporate multithreading and GPU paralleization eventually.
* C++ has _multiple inheritance_, which could work well for **Molecule**'s and **PolymerBlock**'s.  
* Creating dynamically linked libraries in C/C++: <http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_C_libraries.html>  
 

#### _Future Projects/Extensions_
* `DPD/MD Engine using Python:`
   + Look into using C++ objects in Python, so dpd_cpp can be extended without being reimplemented in Python.
   + Could make a similar MD library in Python which can read various file formats produced by NAMD, GROMACS, AMBER, etc., putting the data into MD objects which have predefined fields and behavior which can be manipulated for secondary analysis by our group who would write everything in Python/SciPy.   
   + Those who get better at Python could learn to extend these existing MD objects for specifc systems/calculations/etc. and our library could build.  
   + We can create a GUI application which uses the our library and scripts, as well as other MD software tools, to build/edit/process various systems.  
   + Maybe extend or utilize PyMol and/or VMD?
   + Using C++ libraries within Python without 3rd party dependencies: <http://stackoverflow.com/questions/145270/calling-c-c-from-python>  
