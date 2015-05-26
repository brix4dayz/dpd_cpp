## Programming with _dpd_cpp_ objects
<br>

===================================================================================
### A simple example on the HPC

##### Including a header

		#include "copolymerChain.h"
		
		
		int main() {
		
			return 0;
		}

##### Compiling an executable on the HPC
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

<!-- Highlight syntax for Mou.app, insert at the bottom of the markdown document  -->
 
<script src="http://yandex.st/highlightjs/7.3/highlight.min.js"></script>
<link rel="stylesheet" href="http://yandex.st/highlightjs/7.3/styles/github.min.css">
<script>
  hljs.initHighlightingOnLoad();
</script>
