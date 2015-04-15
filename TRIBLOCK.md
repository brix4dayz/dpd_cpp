#### Triblock Processor

##### Tests Report  

- Groups that have been tested:
	+ 10's --> 25, 30, 40, 50, 90  
	+ 30's --> 25, 30, 40  
	+ 50's --> 25  
	+ 70's --> 90  
	+ 90's --> 90
- Groups with issues:
	+ PBC "dismemberment" --> 10's, 25 through 50
	+ Illegal instruction fault when deleting cores --> 70_70
	+ Last frame not being printed --> 30_40
- Fixed issues:
	+ Illegal instruction fault when deleting cores  


