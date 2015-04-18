import subprocess

block_lengths = [ 4, 10, 30, 50, 70, 90 ]
salt_concs = [ 25, 30, 40, 50, 70, 90 ]

for l in block_lengths:
  for a in salt_concs:
    with cd("./" + str(l) + "_" + str(s)):
      traj_files = subprocess.check_output("ls traj*.xyz", shell=True).split()
      print traj_files