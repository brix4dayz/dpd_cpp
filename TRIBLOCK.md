#### Triblock Processor

##### Analysis Parameters

- **Micelle cutoff** = 1.25
- **Bin length** (neighbor list size) = 2
- **PBC Correction Factor** (percentage of box length) = 0.5
    + Can be varied though

##### Description of Output Files
So in each n_m folder, where n the length, and m is salt concentration, there are the following files:
- **n_m_frames_results.dat**
- **results.out**
- **lastFrameWrapped.xyz**
- **coloredFrames.xyz**

**coloredFrames.xyz** as you can guess has the 100 frames colored based on the chain configuration. **lastFrameWrapped.xyz** has between 1 and 10 of the final frames PBC corrected from triblockProcessor script. **n_m_frames_results.dat** has the individual measurements for each analyzed frame of the trajectory. And finally, **results.out** has the output from the triblockProcessor program, which at the very end has the average and population standard deviation of the frame measurements, so we don't have to calculate the points and error bars for our plots.


##### Old Tests Report  

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


