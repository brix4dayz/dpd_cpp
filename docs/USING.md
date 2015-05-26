## Programming with _dpd_cpp_ objects

### A simple example on the HPC

##### Including a header
```C++
	
	#include "polymerBlock.h" // for PosVect, DirVect, HydrophobicTail
	#include <iostream> // for cout/endl
	using namespace std;

	int main() {

        	idx box_length = 42;

        	float init_bond_length = 0.1;

        	unsigned int idTracker = 0;

        	PosVect* r = new PosVect( &box_length );

        	DirVect* d = new DirVect( &init_bond_length );

        	HydrophobicTail* tail = new HydrophobicTail(NULL, 4, d, &box_length, r, &idTracker, 0);

        	cout << "Printing tail for newdata.dat format: " << endl;

        	tail->printData( stdout );

        	cout << "Printing tail for .xyz format: " << endl;

        	tail->printBlock( stdout );
	
		delete d;
		
        	delete tail;

        	return 0;
	}

```
##### Compiling an executable on the HPC

	[whfuss@login04 triblock]$ /usr/local/apps/gcc/473/bin/g++ -Wall -std=gnu++11 -ldpd -L/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/lib/ -I/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/ -o test testDPD.cpp
	[whfuss@login04 triblock]$ ./test
		Printing tail for newdata.dat format: 
		0 1 3.729090e+01 1.438004e+01 2.445791e+00 0
		1 1 3.733408e+01 1.444274e+01 2.380946e+00 0
		2 1 3.737726e+01 1.450543e+01 2.316101e+00 0
		3 1 3.742044e+01 1.456813e+01 2.251257e+00 0
		Printing tail for .xyz format: 
		1   37.29090   14.38004    2.44579
		1   37.33408   14.44274    2.38095
		1   37.37726   14.50543    2.31610
		1   37.42044   14.56813    2.25126
	[whfuss@login04 triblock]$

<br>

===================================================================================
### Writing a CopolymerChain with PoylmerBlock's

##### Header file/Class description

##### The Constructors

##### The Printers

##### The Destructors

##### Additional functions
<br>

===================================================================================
### Building DPDPolymerData with a CopolymerChain

##### Header file/Class description

##### Constructor

##### deriveChainList

##### deriveBondList

##### printChainList

##### Destructor/unlink

##### Using it to make a builder
<br>

===================================================================================
### Making a CopolymerMicelleFrame

##### Creating a Micelle

##### Using the Bins

##### deriveMicelleList

##### Header file/Class description

##### Constructor

##### Destructor/unlink

##### Example functions/calculations
<br>

===================================================================================
### Using DPDTrajectory with a CopolymerMicelleFrame

##### Header file/Class description

##### Constructor

##### Overriding process/setupOutputFile for TimeEvolution's

##### The Helpers

###### setupHelp

###### analyzeHelp

###### calcHelp

##### Destructor/unlink

##### Using it to make a processor
<br>

===================================================================================
