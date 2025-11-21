import numpy as np

# Load the original file
data = np.loadtxt("Lauren_underground_energy_spectrum.txt")

# Filter out zero or near-zero flux values
filtered = data[data[:, 1] > 0]

# Save to spectrum.dat with 2 columns
np.savetxt("spectrum.dat", filtered, fmt="%.8e %.8e")