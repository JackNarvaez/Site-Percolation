import matplotlib.pyplot as plt
from  matplotlib.colors import LinearSegmentedColormap
from random import random
import numpy as np

''' ---------- Plot Grid ---------- '''
Data = np.loadtxt("Cluster.txt");
fig, ax = plt.subplots(figsize=(8,8))
ax.matshow(Data.astype(np.bool), cmap='Greys',  interpolation='nearest', origin='lower')
ax.set_xlabel("$x$")
ax.set_ylabel("$y$")
plt.savefig("Grid.png", transparent=True, dpi=300)
plt.show()


''' ----- Define Random Color Map -----'''
colors = [(1,1,1)] + [(random(),random(),random()) for i in range(255)]
new_map = LinearSegmentedColormap.from_list('new_map', colors, N=256)


''' ---------- Plot Grid ---------- '''
fig, ax = plt.subplots(figsize=(8,8))
ax.matshow(Data, cmap=new_map,  interpolation='nearest', origin='lower')
ax.set_xlabel("$x$")
ax.set_ylabel("$y$")
plt.savefig("Cluster.png", transparent=True, dpi=300)
plt.show()