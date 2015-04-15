#### Triblock Processor

##### ToDo's
 - Correct avg aggregation number.
 - Remove neither counter and change it to stem.
 - Look into Stem collisions with xor hashing.

##### Analysis Parameters

- **Micelle cutoff** = 1.25
- **Bin length** (neighbor list size) = 2
- **PBC Correction Factor** (percentage of box length) = .5

##### Tests Report  

- Groups that have been tested:
	+ **10's** --> 25, 30, 40, 50, 70, 90  
	+ **30's** --> 25, 30, 40, 50, 70, 90  
	+ **50's** --> 25  
	+ **70's** --> 90  
	+ **90's** --> 90
- Groups with issues:
	+ **PBC "dismemberment"** --> 10's 25 through 50, 30_40
	+ **Illegal instruction fault when deleting cores** --> 70_70
- Fixed issues:
	+ **Illegal instruction fault when deleting cores** --> cores were being added multiple times to the same micelle so cores were being deleted multiple times, added grouped field to cores just like bins 


