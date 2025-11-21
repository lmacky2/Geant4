import numpy as np
import os

# ---- SETTINGS ----
input_file = "sample_flux_matrix.npy"  # Change this to your .npy file
output_cdf_file = "output_cdf.txt"
output_zenith_edges_file = "zenith_edges.npy"
output_azimuth_edges_file = "azimuth_edges.npy"

# ---- LOAD FLUX MATRIX ----
matrix = np.load(input_file)  # Should be shape (90, 360)
zenith_bins, azimuth_bins = matrix.shape
assert zenith_bins == 90 and azimuth_bins == 360, "Matrix must be 90x360"

# ---- FLATTEN AND NORMALIZE TO PROBABILITY ----
flat_flux = matrix.flatten()
prob = flat_flux / np.sum(flat_flux)

# ---- COMPUTE CDF ----
cdf = np.cumsum(prob)

# ---- ANGLE BIN EDGES ----
zenith_edges = np.linspace(0, np.pi / 2, zenith_bins + 1)   # 0 to 90° in radians
azimuth_edges = np.linspace(0, 2 * np.pi, azimuth_bins + 1) # 0 to 360° in radians

# ---- SAVE OUTPUTS ----
np.savetxt(output_cdf_file, cdf, fmt="%.10e")
np.save(output_zenith_edges_file, zenith_edges)
np.save(output_azimuth_edges_file, azimuth_edges)

print("✅ Done! CDF and bin edges saved.")