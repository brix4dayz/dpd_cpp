# dpd_cpp
#### Copyright (c) 2015
##### author: Hayden Fuss
##### Yingling labs


A library of C++ classes designed for building and processing DPD simulations, with several classes implemented specifically for block copolymer solutions.

There are generic classes such as PosVect (position vector), DirVect (direction vector), Bond, Bead (atom), Gyration, PolymerBlock, CopolymerChain, DPDFrame (trajectory processor), DPDData (builder).

Also has specific implementations descending from these classes: HydrophobicTail, PECTriblock, HydrophobicCore, Micelle, DPDPolymerData (aqueous polymer builder), TriblockData (builder), ChargeTriblockData (builder), DiblockMicelle, TriblockMicelle, CopolymerMicelleFrame (processor), TriblockFrame (processor).
