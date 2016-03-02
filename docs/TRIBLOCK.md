# OUTDATED

#### Triblock Processor

##### Analysis Parameters

- **Micelle cutoff** = 1.25
- **Bin length** (neighbor list size) = 2
- **PBC Correction Factor** (percentage of box length) = 0.5
    + Can be varied though

##### Description of Output Files
So in each n_m folder, where n the length, and m is salt concentration, there are the following files:
- **n_m_frames_results.dat**
- **n_m_time_evol.dat**
- **results.out**
- **lastFrameWrapped.xyz**
- **coloredFrames.xyz**

**coloredFrames.xyz** as you can guess has the 100 frames colored based on the chain configuration. **lastFrameWrapped.xyz** has between 1 and 10 of the final frames PBC corrected from triblockProcessor script. **n_m_frames_results.dat** has the individual measurements for each analyzed frame of the trajectory. **n_m_time_evol.dat** has the time evolution of the measurements for each entire trajectory. And finally, **results.out** has the output from the triblockProcessor program, which at the very end has the average and population standard deviation of the frame measurements, so we don't have to calculate the points and error bars for our plots.
