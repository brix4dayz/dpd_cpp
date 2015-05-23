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

measurements = {'cores': {}, 'agg': {}, 'dist': {}, 'stems': {}}

colors = {30:'b', 50:'g', 70:'m', 90:'c'}

titles = {'cores': '# of Cores', 'agg': 'Avg. Aggregation Number / Core', 'dist': 'Avg. Distance Between Linked Cores', 'stems': 'Percentage of Stem Chains'}

for i in measurements.keys():
  for l in block_lengths:
    measurements[i][l] = { 'mean': np.array([]), 'std': np.array([])}

# for all groups
for l in block_lengths:
  for a in salt_concs:
    folder = str(l) + "_" + str(a) # relative path to folder
    os.chdir(folder) # go to group folder
    print(os.getcwd()) # print which group we're in

    tail_results = subprocess.Popen('tail -n 2 results.out', shell=True, stdout=subprocess.PIPE).communicate()[0]
    print tail_results
    tail_results = tail_results.split()

    measurements['cores'][l]['mean'] = np.append(measurements['cores'][l]['mean'], [float(tail_results[1])])
    measurements['cores'][l]['std'] = np.append(measurements['cores'][l]['std'], [float(tail_results[7])])

    measurements['agg'][l]['mean'] = np.append(measurements['agg'][l]['mean'], [float(tail_results[2])])
    measurements['agg'][l]['std'] = np.append(measurements['agg'][l]['std'], [float(tail_results[8])])

    measurements['dist'][l]['mean'] = np.append(measurements['dist'][l]['mean'], [float(tail_results[3])])
    measurements['dist'][l]['std'] = np.append(measurements['dist'][l]['std'], [float(tail_results[9])])

    measurements['stems'][l]['mean'] = np.append(measurements['stems'][l]['mean'], [float(tail_results[4])])
    measurements['stems'][l]['std'] = np.append(measurements['stems'][l]['std'], [float(tail_results[10])])

    os.chdir(current_dir)

# make plots
for i in measurements.keys():
  # plot each length for the current measurement
  for l in colors.keys():
    plt.errorbar(salt_concs, measurements[i][l]['mean'], yerr=measurements[i][l]['std'], 
                 marker='o', color=colors[l], label=str(l), linewidth=1.5)

  plt.ylabel(titles[i])
  plt.xlabel('$ a_{2,2} $')

  # sort legend and put in top right corner
  ax = plt.gca()
  handles, labels = ax.get_legend_handles_labels()
  hl = sorted(zip(handles, labels), key=operator.itemgetter(1))
  handles2, labels2 = zip(*hl)
  ax.legend(handles2, labels2, bbox_to_anchor=(1, 1), bbox_transform=plt.gcf().transFigure)

  x1, x2, y1, y2 = plt.axis()

  plt.axis([0, 100, y1, y2])

  plt.savefig(i + ".png", dpi=96)

  plt.clf()
