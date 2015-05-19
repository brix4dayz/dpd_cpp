import subprocess
import os
import numpy as np
import matplotlib.pyplot as plt
import operator


Na = 30
a22 = 50

# location of where we are
current_dir = os.getcwd()

measurements = {'t': { 'idx':0 }, 'cores': { 'idx':1 }, 'agg': { 'idx':2 }, 'dist': { 'idx':3 }, 'stems': { 'idx':4 }}

delta = {'33%':'g', '67%':'b', '100%':'m'}

titles = {'cores': '# of Cores', 'agg': 'Avg. Aggregation Number / Core', 'dist': 'Avg. Distance Between Linked Cores', 'stems': 'Percentage of Stem Chains'}

for i in measurements.keys():
  for d in delta.keys():
    measurements[i][d] = np.array([])

for d in delta:
   # relative path to folder
  os.chdir(d) # go to group folder
  print(os.getcwd()) # print which group we're in

  f = open( str(Na) + "_" + str(a22) + "_time_evol.dat", "r")

  for line in f:
    tokens = line.split()
    if tokens[0] != "Frame":
      for m in measurements.keys():
        measurements[m][d] = np.append(measurements[m][d], [float(tokens[measurements[m]['idx']])])

  f.close()

  os.chdir(current_dir)

# make plots
for i in measurements.keys():
  if i != 't':
    # plot each length for the current measurement
    for d in delta.keys():
      plt.plot(measurements['t'][d], measurements[i][d], 
               color=delta[d], label=d, linewidth=2.0)

    plt.ylabel(titles[i])
    plt.xlabel('$ \\tau $')

    # sort legend and put in top left corner
    ax = plt.gca()
    handles, labels = ax.get_legend_handles_labels()
    hl = sorted(zip(handles, labels), key=operator.itemgetter(1))
    handles2, labels2 = zip(*hl)
    ax.legend(handles2, labels2, bbox_to_anchor=(1, 1), bbox_transform=plt.gcf().transFigure)

    x1, x2, y1, y2 = plt.axis()

    plt.axis([0, 600, y1, y2])

    plt.savefig(i + "_time.png", dpi=96)

    plt.clf()


