import subprocess
import os
import numpy as np
import matplotlib.pyplot as plt
import operator

Na = 30
salt_concs = [ 30, 50, 70, 90 ]


# location of where we are
current_dir = os.getcwd()


for a22 in salt_concs:
  folder = str(Na) + "_" + str(a22)
  os.chdir(folder)
  print(os.getcwd())

  measurements = {'t': { 'idx':0 }, 'cores': { 'idx':1 }, 'agg': { 'idx':2 }, 'dist': { 'idx':3 }, 'stems': { 'idx':4 }}

  delta = {'33%':'g', '50%':'r', '67%':'b', '85%':'c' , '100%':'m'}

  labs = { '33%': '33%', '50%':'50%', '67%': '67%', '85%':'85%', '100%': '99%'}

  titles = {'cores': '# of Cores', 'agg': 'Avg. Aggregation Number / Core', 'dist': 'Avg. Distance Between Linked Cores', 'stems': 'Percentage of Stem Chains'}

  for i in measurements.keys():
    for d in delta.keys():
      measurements[i][d] = np.array([])

  for d in delta:
    if ((d != "50%" || d != "85%") and (a22 != 30 || a22 != 90):
       # relative path to folder
      os.chdir(d) # go to group folder
      print(os.getcwd()) # print which group we're in

      f = open( folder + "_time_evol.dat", "r")

      for line in f:
        tokens = line.split()
        if tokens[0] != "Frame":
          for m in measurements.keys():
            measurements[m][d] = np.append(measurements[m][d], [float(tokens[measurements[m]['idx']])])

      f.close()

      os.chdir("../")

  # make plots
  for i in measurements.keys():
    if i != 't':
      # plot each length for the current measurement
      for d in delta.keys():
        if ((d != "50%" || d != "85%") and (a22 != 30 || a22 != 90):
          plt.plot(measurements['t'][d]*10, measurements[i][d], 
                 color=delta[d], label=labs[d], linewidth=2.0)

      plt.ylabel(titles[i])
      plt.xlabel('$ \\tau $', fontsize=25)

      # sort legend and put in top right corner
      ax = plt.gca()
      handles, labels = ax.get_legend_handles_labels()
      hl = sorted(zip(handles, labels), key=operator.itemgetter(1))
      handles2, labels2 = zip(*hl)
      # the sorting would put 100% at the top, so I mapped it to 99%
      # then replace the 99% label with 100%, an ugly hack but
      # it gets the job done
      labels2 = list(labels2)
      labels2[2] = '100%'
      labels2 = tuple(labels2)
      ax.legend(handles2, labels2, bbox_to_anchor=(1, 1), bbox_transform=plt.gcf().transFigure)

      x1, x2, y1, y2 = plt.axis()

      if i != 'cores':
        plt.axis([0, 6000, y1, y2])
      else:
        plt.axis([0, 6000, y1, 40])

      plt.savefig( i + "_" + folder + "_time.png", dpi=96)

      plt.clf()

  os.chdir(current_dir)


