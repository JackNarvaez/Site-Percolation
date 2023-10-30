import matplotlib.pyplot as plt
import numpy as np

# Read Data
Data = np.loadtxt("Grid.txt");

# Plot Data
plt.matshow(Data)

plt.show()