## Setting up _dpd_cpp_ on the HPC
<br>
**_dpd_cpp_** has been compiled on the **HPC** in such a way that allows us to use it's executable programs from anywhere within the **HPC**. It does so by using what are known as runtime libraries, in the form of `.so` or shared object files. These libraries contain most of the actual code the programs use. And so, certain path variables in your `csh` environment need to be updated, so it can find the executables, and the `.so` files they require, whenever you run them.  
  
Instructions for updating your paths as well as compiling **_dpd_cpp_** are given below. You can also find a brief description of **_dpd_cpp_**'s current executable programs which build and process our triblock simulations.
  
### Configuring your `csh` environment
There's a short `bash` script on the **HPC** which you can run to update your `csh` path variables. If you log on to the **HPC** and type the commands below, you should get similar output as to what has been shown.  

	[whfuss@login04 ~]$ cd /gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/
	[whfuss@login04 dpd_cpp]$ ./configureHPC.sh 
		You need to run: `source ~/.cshrc` when this is done running.
		You need to run: `source ~/.cshrc` when this is done running.
		You need to run: `source ~/.cshrc` when this is done running.
	[whfuss@login04 dpd_cpp]$ source ~/.cshrc
	[whfuss@login04 dpd_cpp]$ ./configureHPC.sh 
		Path already up-to-date.
		LD_LIBRARY_PATH already up-to-date.
		LD_LIBRARY_PATH already up-to-date.
	[whfuss@login04 dpd_cpp]$  
	
When you initially run `configureHPC.sh` you may see the "You need to run..." message only once or twice and one or two of the up-to-date messages, that just means you already had some of the required folders in your path variables.  

If you see the "You need to run..." message your first time running `configreHPC.sh` then you must then run the `source ~/.cshrc` command to update your environment without having to logout and log back in.  

You can run `configureHPC.sh` a second time after updating your environment, to make sure you get all three up-to-date messages and that your environment is configured.

### The current executables

##### Builders
+ **triblockBuilder** - this program generates the `newdata.dat` for our normal BAB triblock system, a chain with a single PEC block in the center and two hydrophobic tails. It takes the following parameters:
	- Box length
	- Bond length --> Initial distance between beads in a chain, always set to 0.1
	- Polymer volume fraction
	- Tail length --> if the tails are each 4 beads long, this value is 4
	- PEC length
	- PEC charge density
	- Output filename --> usually set to `newdata.dat`
<br><br>
+ **linkerBuilder** - generates `newdata.dat` for the new "linker" BACAB triblock system, a chain made with two PEC diblock chains linked by a stiff polymer block. It takes in the same parameters as `triblockBuilder` (PEC length has different meaning though), as well as:
	- Link length
	- PEC length --> if each PEC block is 15 beads long, this value is 15

**NOTE**: If PEC charge density is set to 1.0, the builders do not make an additional atom type for the uncharged PEC bead. So `triblockBuilder` would only make 3 atom types instead of 4, and `linkerBuilder` would make 4 instead of 5. This is because **DPD** does not allow atom types to be defined if they are not used. Meaning systems with charge density equal to 1.0 will require a different `dpd.in` file than those systems with charge density less than 1.0.

##### Processors
+ **triblockProcessor** - this program processes a group of `.xyz` trajectories for a BAB triblock simulation for every 10th frame of the last 2000 frames. It outputs the means and standard deviations of the measurements to the console, so this program should always have it's `stdout` redirected to another file. It takes in the following parameters:
	- Box length
	- Bin size --> size of neighbor list bin, always set to 2
	- Number of `.xyz` files
	- The names of the `.xyz` files
	- Output file name --> typically named `<group name>_frames_results.dat`
	- Tail length --> if the tails are each 4 beads long, this value is 4
	- PEC length
	- Micelle cutoff radius --> always set to 1.25
	- PBC correction factor --> always set to 0.5
<br>
<br>
+ **colorTriblock** - this program "colors" a group of `.xyz` trajectories for a BAB triblock simulation for every 10th frame of the last 2000 frames, so that every stem triblock is type 1 and every petal triblock is type 2. It takes in the same parameters as `triblockProcessor` except the output file is typically named `coloredFrames.xyz`. IT DOES NOT NEED ITS `stdout` REDIRECTED.  
<br>
+ **triblockTime** - this program processes a group of `.xyz` trajectories for a BAB triblock simulation for every 10th frame of the whole trajectory, in order to get a time evolution. It takes in the same parameters as `triblockProcessor` except the output file is typically named `<group name>_time_evol.dat`. IT DOES NOT NEED ITS `stdout` REDIRECTED.

##### Sample Executions

The following shows how to run `linkerBuilder` to initialize a BACAB triblock system:

	[whfuss@login04 dpd_cpp]$ linkerBuilder
		Enter box length: 36
		Enter bond length: 0.1
		Enter polymer volume fraction: 0.1
		Enter hydrophobic tail length: 4
		Enter polyelectrolyte (pec) block length: 15
		Enter linker block length: 4
		Enter pec block charge density: 1.0
		Enter desired filename: newdata.dat
	[whfuss@login04 dpd_cpp]$
	

The following shows to run `triblockProcessor` to process a BAB triblock trajectory by redirecting its `stdin` to `params.in` and its `stout` to `results.out`:

	[whfuss@login04 dpd_cpp]$ vim params.in
	[whfuss@login04 dpd_cpp]$ tail params.in
		36
		2
		1
		last.xyz
		testResults.dat
		4
		30
		1.25
		0.5
	[whfuss@login04 dpd_cpp]$ triblockProcessor < params.in > results.out
	[whfuss@login04 dpd_cpp]$ tail -n 3 results.out
   			Cores     AvgAgg     AvgDistCores     Stem     Petal
		AVG:   1.0000 368.0000   0.0000   0.0082   0.9918
		STDDEV:   0.0000   0.0000   0.0000   0.0000   0.0000
	[whfuss@login04 dpd_cpp]$ 

### Compiling `libdpd.so` and the executables
If you need to recompile the executables or **_dpd_cpp_**'s runtime library `libdpd.so` on the **HPC**, you can do so easily using the commands below. 

	[whfuss@login04 ~]$ cd /gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/
	[whfuss@login04 dpd_cpp]$ ./compileHPC.sh > myCompile.out
	[whfuss@login04 dpd_cpp]$ diff myCompile.out compile.out 
	[whfuss@login04 dpd_cpp]$
	
`compile.out` is a text file containing the output that should be produced by `compileHPC.sh` if the compilation was successful. So it's important to redirect the output of `compileHPC.sh` to another text file, in the example above it was `myCompile.out`, so the two outputs can be compared.  

If the `diff` command produces any output then the compilation went wrong, and you should email <whfuss@ncsu.edu> with a copy of your `myCompile.out` file.


			