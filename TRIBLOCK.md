#### Triblock Processor

##### Analysis Parameters

- **Micelle cutoff** = 1.25
- **Bin length** (neighbor list size) = 2

##### Tests Report  

- Groups that have been tested:
	+ **10's** --> 25, 30, 40, 50, 70, 90  
	+ **30's** --> 25, 30, 40, 50, 70, 90  
	+ **50's** --> 25  
	+ **70's** --> 90  
	+ **90's** --> 90
- Groups with issues:
	+ **PBC "dismemberment"** --> 10's 25 through 50, 30_40
		- 30_70 isn't wrong necessarily, but it's so wide and isn't "complete"
	+ **Illegal instruction fault when deleting cores** --> 70_70
- Fixed issues:
	+ **Illegal instruction fault when deleting cores** --> cores were being added multiple times to the same micelle so cores were being deleted multiple times, added grouped field to cores just like bins 


