import subprocess
import os
import numpy as np
import matplotlib.pyplot as plt

# location of where we are
current_dir = os.getcwd()

# simulation variables
block_lengths = np.array([ 30, 50, 70, 90 ])
salt_concs = np.array([ 25, 30, 40, 50, 70, 90 ])

measurements = {'cores': {}, 'agg': {}, 'dist': {}, 'stems': {}}

colors = { 30:'b', 50:'g', 70:'m', 90:'c'}

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

# cores
for l in colors.keys():
  plt.errorbar(salt_concs, measurements['cores'][l]['mean'], yerr=measurements['cores'][l]['std'], marker='o', color=colors[l], label=str(l))

plt.ylabel('# of Cores')
plt.xlabel('$ a_{2,2} $')
plt.legend()

x1, x2, y1, y2 = plt.axis()

plt.axis([0, 100, y1, y2])

plt.savefig("cores.png", dpi=96)

plt.clf()

# agg
for l in colors.keys():
  plt.errorbar(salt_concs, measurements['agg'][l]['mean'], yerr=measurements['agg'][l]['std'], marker='o', color=colors[l], label=str(l))

plt.ylabel('Avg. Aggregation Number / Core')
plt.xlabel('$ a_{2,2} $')
plt.legend()

x1, x2, y1, y2 = plt.axis()

plt.axis([0, 100, y1, y2])

plt.savefig("agg.png", dpi=96)

plt.clf()

# dist
for l in colors.keys():
  plt.errorbar(salt_concs, measurements['dist'][l]['mean'], yerr=measurements['dist'][l]['std'], marker='o', color=colors[l], label=str(l))

plt.ylabel('Avg. Distance Between Linked Cores')
plt.xlabel('$ a_{2,2} $')
plt.legend()

x1, x2, y1, y2 = plt.axis()

plt.axis([0, 100, y1, y2])

plt.savefig("dist.png", dpi=96)

plt.clf()

# stems
for l in colors.keys():
  plt.errorbar(salt_concs, measurements['stems'][l]['mean'], yerr=measurements['stems'][l]['std'], marker='o', color=colors[l], label=str(l))

plt.ylabel('Percentage of Stems')
plt.xlabel('$ a_{2,2} $')
plt.legend()

x1, x2, y1, y2 = plt.axis()

plt.axis([0, 100, y1, y2])

plt.savefig("stems.png", dpi=96)

plt.clf()