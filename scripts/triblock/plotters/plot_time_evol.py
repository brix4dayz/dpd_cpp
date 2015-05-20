import subprocess
import os
import numpy as np
import matplotlib.pyplot as plt
import operator

# location of where we are
current_dir = os.getcwd()

# simulation variables
block_lengths = np.array([ 30, 50, 70, 90 ])
salt_concs = np.array([ 25, 30, 40, 50, 70, 90 ])

for Na in block_lengths:

  measurements = {'t': { 'idx':0 }, 'cores': { 'idx':1 }, 'agg': { 'idx':2 }, 'dist': { 'idx':3 }, 'stems': { 'idx':4 }}

  saltColors = { 25:'b', 30:'g', 40:'r', 50:'c', 70:'m', 90:'y'}

  titles = {'cores': '# of Cores', 'agg': 'Avg. Aggregation Number / Core', 'dist': 'Avg. Distance Between Linked Cores', 'stems': 'Percentage of Stem Chains'}

  for i in measurements.keys():
    for a22 in salt_concs:
      measurements[i][a22] = np.array([])

  for a22 in salt_concs:
    # relative path to folder
    folder = str(Na) + "_" + str(a22)
    os.chdir(folder) # go to group folder
    print(os.getcwd()) # print which group we're in

    f = open( folder + "_time_evol.dat", "r")

    for line in f:
      tokens = line.split()
      if tokens[0] != "Frame":
        for m in measurements.keys():
          measurements[m][a22] = np.append(measurements[m][a22], [float(tokens[measurements[m]['idx']])])

    f.close()

    os.chdir(current_dir)

  # make plots
  for i in measurements.keys():
    if i != 't':
      # plot each length for the current measurement
      for a22 in salt_concs:
        plt.plot(measurements['t'][a22]*10, measurements[i][a22], 
                 color=saltColors[a22], label=str(a22), linewidth=2.0)

      plt.ylabel(titles[i])
      plt.xlabel('$ \\tau $', fontsize=25)

      # sort legend and put in top right corner
      ax = plt.gca()
      handles, labels = ax.get_legend_handles_labels()
      hl = sorted(zip(handles, labels), key=operator.itemgetter(1))
      handles2, labels2 = zip(*hl)
      ax.legend(handles2, labels2, bbox_to_anchor=(1, 1), bbox_transform=plt.gcf().transFigure)

      x1, x2, y1, y2 = plt.axis()

      if i != 'cores':
        plt.axis([0, 6000, y1, y2])
      else:
        plt.axis([0, 6000, y1, 40])

      plt.savefig( i + "_" + str(Na) + "_time.png", dpi=96)

      plt.clf()


