#### Triblock Processor

##### Latest
- Colorer implemented. Stem chains are converted to type 1 and petal type 2.

##### Analysis Parameters

- **Micelle cutoff** = 1.25
- **Bin length** (neighbor list size) = 2
- **PBC Correction Factor** (percentage of box length) = 0.5
    + Can be varied though

##### Tests Report  

- Groups that have been tested:
	+ **10's** --> 25, 30, 40, 50, 70, 90  
	+ **30's** --> 25, 30, 40, 50, 70, 90  
	+ **50's** --> 25  
	+ **70's** --> 90  
	+ **90's** --> 90
- Groups with issues:
	+ **PBC "dismemberment"** --> 10's 25 through 50, 30_40
	+ **Illegal instruction fault when deleting cores** --> 70_90
- Fixed issues:
	+ **Illegal instruction fault when deleting cores** --> cores were being added multiple times to the same micelle so cores were being deleted multiple times, added grouped field to cores just like bins 


