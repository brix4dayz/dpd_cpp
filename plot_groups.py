import subprocess
import os

# location of where we are
current_dir = os.getcwd()

# simulation variables
block_lengths = [ 4, 10, 30, 50, 70, 90 ]
salt_concs = [ 25, 30, 40, 50, 70, 90 ]

# for all groups
for l in block_lengths:
  for a in salt_concs:
    folder = str(l) + "_" + str(a) # relative path to folder
    os.chdir(folder) # go to group folder
    print(os.getcwd()) # print which group we're in

    os.chdir(current_dir)