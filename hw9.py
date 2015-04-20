import numpy as np
import pylab as plt

# source for figure with text in pylab: 
# http://stackoverflow.com/questions/11143619/add-graph-description-under-graph-in-pylab

def V_Sphere(r):
  return (4.0/3.0)*np.pi*(r**3)

def SA_Sphere(r):
  return 4.0*np.pi*(r**2)

def deltaG_Sphere(Gv, g, r):
  return V_Sphere(r)*Gv + SA_Sphere(r)*g

plt.figure()
plt.gca().set_position((.1, .3, .8, .6))

Gv = -60*(10**6) # J / ( m ^ 3 )
g = .4 # J / ( m ^ 2 )

r = np.arange(0, (3*(10**(-8))), (10**(-9))) # m
r0 = -2*g/Gv # m

delG0 = deltaG_Sphere(Gv, g, r0) # J
delG = deltaG_Sphere(Gv, g, r) # J
delGV = V_Sphere(r)*Gv # J
delGSA = SA_Sphere(r)*g # J

zero = 0*r

plt.plot(r, delG)
plt.plot(r, delGV, '--r')
plt.plot(r, delGSA, '--r')
plt.plot(r0, delG0, 'g^')
plt.plot(r, zero, '--k')

plt.ylabel('$\Delta G$ (J)')
plt.xlabel('$r$ (m)')

plt.figtext(.02, .02, ('Critical radius, $r_0$: %1.4e meters\n'%r0) + 
  ('Nucleation activation energy, $\Delta G_0$: %1.4e Joules\n'%delG0))

plt.axis([0, (3*(10**(-8))), (-2.0*(10**-15)), (1.5*(10**-15))])

plt.savefig("prob16_5.png", dpi=96)
