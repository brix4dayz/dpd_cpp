import subprocess
import os

# location of processing/building programs
exec_path = "/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/bin/"

# location of where we are
current_dir = os.getcwd()

# go to dpd_cpp and run the compile script
os.chdir(exec_path)
subprocess.call("./compileTriblock.sh", shell=True)

# go back to where we were
os.chdir(current_dir)

# simulation params
box_length = 36
bin_size = 2
mic_cut = 1.25
pbc = 0.5
tail_length = 4

# simulation variables
block_lengths = [ 4, 10, 30, 50, 70, 90 ]
salt_concs = [ 25, 30, 40, 50, 70, 90 ]

# for all groups
for l in block_lengths:
    for a in salt_concs:
        folder = str(l) + "_" + str(a) # relative path to folder
        os.chdir(folder) # go to group folder
        print(os.getcwd()) # print which group we're in
        
        # returns results of ls into a string called trajs
        trajs = subprocess.Popen('ls traj*.xyz', shell=True, stdout=subprocess.PIPE).communicate()[0]
        print trajs
        # makes the string an array of strings, each one being the name of an xyz file for the group
        trajs = trajs.split()
        
        # write params.in for triblockProcessor
        f = open('time.in', 'w')
        f.write(str(box_length) + "\n") # prints box_length
        f.write(str(bin_size) + "\n") # prints bin size
        f.write(str(len(trajs)) + "\n") # prints number of xyz files for this group
        # prints each xyz file on a separate line
        for t in trajs:
          f.write(t + "\n")
        # prints the output file name: ${l}_${a}_frames_results.dat
        f.write(str(l) + "_" + str(a) + "_time_evol.dat\n")
        f.write(str(tail_length) + "\n") # prints constant tail_length
        f.write(str(l) + "\n") # prints variable block length
        f.write(str(mic_cut) + "\n") # prints micelle cutoff
        f.write(str(pbc) + "\n") # prints pbc correction factor
        f.close() # done writing params.in

        # write LSF job as a bash script
        f = open('timeEvol.sh', 'w')
        f.write("#! /bin/bash\n")
        f.write("#BSUB -q yingling\n")
        f.write("#BSUB -n 1\n")
        f.write("#BSUB -W 300:00\n")
        f.write("#BSUB -J tri_" + str(l) + "_" + str(a) + "\n")
        f.write("#BSUB -R \"em64t span[ptile=2]\"\n")
        f.write("#BSUB -o triblock.out\n");
        f.write("#BSUB -e triblock.err\n\n\n\n");
        # run the programs in this folder with it's inputs
        f.write(exec_path + "triblockTime < time.in\n")
        f.close()

        # submit the job to the HPC
        subprocess.call("bsub < timeEvol.sh", shell=True)

        # go back to original dir and onto the next group
        os.chdir(current_dir)
# done
