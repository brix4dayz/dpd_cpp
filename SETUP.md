## Setting up _dpd_cpp_ on the HPC  
  
### Configuring your `csh` environment

	[whfuss@login04 ~]$ cd /gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/
	[whfuss@login04 dpd_cpp]$ ./setup_dpd_cpp_path.sh 
		You need to run: `source ~/.cshrc` when this is done running.
		You need to run: `source ~/.cshrc` when this is done running.
		You need to run: `source ~/.cshrc` when this is done running.
	[whfuss@login04 dpd_cpp]$ source ~/.cshrc
	[whfuss@login04 dpd_cpp]$ ./setup_dpd_cpp_path.sh 
		Path already up-to-date.
		LD_LIBRARY_PATH already up-to-date.
		LD_LIBRARY_PATH already up-to-date.
	[whfuss@login04 dpd_cpp]$

### Compiling `libdpd.so` and the Triblock executables

	[whfuss@login04 dpd_cpp]$ ./compileHPC.sh > myCompile.out
	[whfuss@login04 dpd_cpp]$ diff myCompile.out compile.out 
	[whfuss@login04 dpd_cpp]$

			