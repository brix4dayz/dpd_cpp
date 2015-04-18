import subprocess
import os

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
        os.chdir("..")
