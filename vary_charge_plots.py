import numpy as np
import matplotlib.pyplot as plt

del_min = 0.2

del_max = 1.1

delta = np.arange((1.0/3.0), 1.0, (1.0/3.0))

cores = np.array([2.0, 1.8, 15.9])

c_err = np.array([0.1, 0.5, 1.1])

plt.errorbar(delta, cores, yerr=c_err, marker='o' )

plt.ylabel('# of Cores')
plt.xlabel('$ \delta $')

plt.axis([del_min, del_max, 0.0, 18.0])

plt.savefig("cores.png", dpi=96)

plt.clf()

agg = np.array([257.6, 272, 33.1])

a_err = np.array([8.0, 61, 2.4])

plt.errorbar(delta, agg, yerr=a_err, marker='o')

plt.ylabel('Avg. Aggregation Number / Core')
plt.xlabel('$ \delta $')

plt.axis([del_min, del_max, 0.0, 350.0])

plt.savefig("agg.png", dpi=96)

plt.clf()

dist = np.array([7.6, 1.9, 13.2])

d_err = np.array([2.3, 1.8, 2.4])

plt.errorbar(delta, dist, yerr=d_err, marker='o')

plt.ylabel('Avg. Distance Btwn Linked Cores')
plt.xlabel('$ \delta $')

plt.axis([del_min, del_max, 0.0, 16.0])

plt.savefig("dist.png", dpi=96)

plt.clf()

stems = np.array([40.6, 24, 43.4])

st_err = np.array([0.5, 15, 2.1])

plt.errorbar(delta, stems, yerr=st_err, marker='o')

plt.ylabel('%' + ' Stems')
plt.xlabel('$ \delta $')

plt.axis([del_min, del_max, 0.0, 50.0])

plt.savefig("stems.png", dpi=96)

plt.clf()