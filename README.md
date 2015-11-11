# _dpd_cpp_
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
A C++ framework designed for building and processing **DPD** simulations, with several classes implemented specifically for block copolymer solutions.

There are generic classes such as **PosVect** (position vector), **DirVect** (direction vector), **Bond**, **Bead** (atom), **Gyration**, **PolymerBlock**, **CopolymerChain**, **Bin**, **DPDFrame** (trajectory processor), **DPDData** (generator), **DPDBuilder**.

Also has specific implementations descending from these classes: **BeadGyration**, **HydrophobicTail**, **PECTriblock**, **HydrophobicCore**, **Micelle**, **DPDPolymerData** (aqueous polymer builder), **TriblockData** (builder), **ChargeTriblockData** (builder), **DiblockMicelle**, **TriblockMicelle**, **CopolymerMicelleFrame** (processor), **TriblockFrame** (processor).

<div style="text-align:center" markdown="1">

<img src="hd_micelles.jp2" height=650>

</div>
