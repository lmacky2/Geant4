import numpy as np
import matplotlib.pyplot as plt

energies = np.loadtxt("..\output\EnergyLog.txt")

plt.hist(energies, bins=np.logspace(np.log10(100), np.log10(1e11), 100), density=True)
plt.xscale('log')
plt.xlabel("Muon Energy [MeV]")
plt.ylabel("Normalized Counts")
plt.title("Sampled Muon Energy Spectrum")
plt.grid(True)
plt.show()