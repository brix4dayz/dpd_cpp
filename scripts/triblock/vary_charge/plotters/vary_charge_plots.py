import subprocess
import os
import numpy as np
import matplotlib.pyplot as plt
import operator

# location of where we are
current_dir = os.getcwd()

Na = 30

# simulation variables
a22 = np.array([ 30, 50 ])
delta = np.array([ (1.0/3.0), (2.0/3.0), 1.0 ])
deltaStrings = { (1.0/3.0): '33%', (2.0/3.0): '67%', 1.0: '100%'}

measurements = {'cores': {}, 'agg': {}, 'dist': {}, 'stems': {}}

colors = {30: 'b', 50:'g'}

titles = {'cores': '# of Cores', 'agg': 'Avg. Aggregation Number / Core', 'dist': 'Avg. Distance Between Linked Cores', 'stems': 'Percentage of Stem Chains'}

for i in measurements.keys():
  for a in a22:
    measurements[i][a] = { 'mean': np.array([]), 'std': np.array([])}

# for all groups
for a in a22:
  group = str(Na) + "_" + str(a)
  os.chdir(group)
  print(os.getcwd())
  for d in delta:
    folder = deltaStrings[d] # relative path to folder
    os.chdir(folder) # go to group folder
    print(os.getcwd()) # print which group we're in

    tail_results = subprocess.Popen('tail -n 2 results.out', shell=True, stdout=subprocess.PIPE).communicate()[0]
    print tail_results
    tail_results = tail_results.split()

    measurements['cores'][a]['mean'] = np.append(measurements['cores'][a]['mean'], [float(tail_results[1])])
    measurements['cores'][a]['std'] = np.append(measurements['cores'][a]['std'], [float(tail_results[7])])

    measurements['agg'][a]['mean'] = np.append(measurements['agg'][a]['mean'], [float(tail_results[2])])
    measurements['agg'][a]['std'] = np.append(measurements['agg'][a]['std'], [float(tail_results[8])])

    measurements['dist'][a]['mean'] = np.append(measurements['dist'][a]['mean'], [float(tail_results[3])])
    measurements['dist'][a]['std'] = np.append(measurements['dist'][a]['std'], [float(tail_results[9])])

    measurements['stems'][a]['mean'] = np.append(measurements['stems'][a]['mean'], [float(tail_results[4])])
    measurements['stems'][a]['std'] = np.append(measurements['stems'][a]['std'], [float(tail_results[10])])

    os.chdir(group)
  os.chdir(current_dir)

# make plots
for i in measurements.keys():
  # plot each length for the current measurement
  for l in colors.keys():
    plt.errorbar(delta, measurements[i][l]['mean'], yerr=measurements[i][l]['std'], 
                 marker='^', markersize=12, color=colors[l], label=str(l), linewidth=2.0)

  plt.ylabel(titles[i])
  plt.xlabel('$ \delta $', fontsize=25)

  # sort legend and put in top right corner
  ax = plt.gca()
  handles, labels = ax.get_legend_handles_labels()
  hl = sorted(zip(handles, labels), key=operator.itemgetter(1))
  handles2, labels2 = zip(*hl)
  ax.legend(handles2, labels2, bbox_to_anchor=(1, 1), bbox_transform=plt.gcf().transFigure)

  x1, x2, y1, y2 = plt.axis()

  plt.axis([0, 1.2, y1, y2])

  plt.savefig(i + ".png", dpi=96)

  plt.clf()
