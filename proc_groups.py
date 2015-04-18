import subprocess
import os

exec_path = "/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/"

current_dir = os.getcwd()

os.chdir(exec_path)

subprocess.call("compileTriblock.sh", shell=True)

os.chdir(current_dir)

box_length = 36
bin_size = 2
mic_cut = 1.25
pbc = 0.5
tail_length = 4

block_lengths = [ 4, 10, 30, 50, 70, 90 ]
salt_concs = [ 25, 30, 40, 50, 70, 90 ]

for l in block_lengths:
    for a in salt_concs:
        folder = str(l) + "_" + str(a)
        os.chdir(folder)
        print(os.getcwd())
        
        trajs = subprocess.Popen('ls traj*.xyz', shell=True, stdout=subprocess.PIPE).communicate()[0]
        print trajs
        trajs = trajs.split()
        
        f = open('params.in', 'w')
        f.write(str(box_length) + "\n")
        f.write(str(bin_size) + "\n")
        f.write(str(len(trajs)) + "\n")
        for t in trajs:
          f.write(t + "\n")
        f.write(str(l) + "_" + str(a) + "_frames_results.dat\n")
        f.write(str(tail_length) + "\n")
        f.write(str(l) + "\n")
        f.write(str(mic_cut) + "\n")
        f.write(str(pbc) + "\n")
        f.close()

        f = open('colors.in', 'w')
        f.write(str(box_length) + "\n")
        f.write(str(bin_size) + "\n")
        f.write(str(len(trajs)) + "\n")
        for t in trajs:
          f.write(t + "\n")
        f.write("coloredFrames.xyz\n")
        f.write(str(tail_length) + "\n")
        f.write(str(l) + "\n")
        f.write(str(mic_cut) + "\n")
        f.write(str(pbc) + "\n")
        f.close()

        f = open('analyzeTrajectory.sh', 'w')
        f.write("#! /bin/bash\n")
        f.write("#BSUB -q yingling\n")
        f.write("#BSUB -n 1\n")
        f.write("#BSUB -W 300:00\n")
        f.write("#BSUB -J tri_" + str(l) + "_" str(a) + "\n")
        f.write("#BSUB -R \"em64t span[ptile=2]\"\n")
        f.write("#BSUB -o triblock.out\n");
        f.write("#BSUB -e triblock.err\n\n\n\n");
        f.write(exec_path + "triblockProcessor < params.in > results.out\n\n")
        f.write(exec_path + "colorTriblock < colors.in\n\n")
        f.close()

        subprocess.call("bsub < analyzeTrajectory.sh", shell=True)

        os.chdir(current_dir)
