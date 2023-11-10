import matplotlib.pyplot as plt
from  matplotlib.colors import LinearSegmentedColormap
from random import random, seed
import numpy as np

seed(1234)

''' ---------- Plot Grid ---------- '''
data = np.fromfile("Cluster", dtype=np.int32, count=-1)
n2 = len(data)
L  = int(np.sqrt(n2))
Data = np.reshape(data, [L, L], order='A')
fig, ax = plt.subplots(figsize=(8,8))
ax.matshow(Data.astype(bool), cmap='Greys',  interpolation='nearest', origin='lower')
ax.set_xlabel("$x$")
ax.set_ylabel("$y$")
plt.savefig("Grid.png", transparent=True, dpi=300)
plt.show()

n = int(Data.max())

''' ----- Define Random Color Map -----'''
colors = [(0,0,0)] + [(random(),random(),random()) for i in range(n)]
new_map = LinearSegmentedColormap.from_list('new_map', colors, N=n+1)

''' ---------- Plot Grid ---------- '''
fig, ax = plt.subplots(figsize=(8,8))
ax.matshow(Data, cmap=new_map,  interpolation='nearest', origin='lower')
ax.set_xlabel("$x$")
ax.set_ylabel("$y$")
plt.savefig("Cluster.png", transparent=True, dpi=300)
plt.show()