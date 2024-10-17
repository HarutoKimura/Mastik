import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('probe_gmp_rsa.csv')

# Apply a moving average to smooth the data
window_size = 5
df['Square_smooth'] = df['Square'].rolling(window=window_size, center=True).mean()
df['Multiply_smooth'] = df['Multiply'].rolling(window=window_size, center=True).mean()
df['Reduce_smooth'] = df['Reduce'].rolling(window=window_size, center=True).mean()

# Create the plot
plt.figure(figsize=(100, 8))

# Plot smoothed data as lines
# plt.plot(df['Slot'], df['Square_smooth'], 'r-', label='Square', linewidth=1)
# plt.plot(df['Slot'], df['Multiply_smooth'], 'g-', label='Multiply', linewidth=1)
# plt.plot(df['Slot'], df['Reduce_smooth'], 'b-', label='Reduce', linewidth=1)

plt.plot(df['Slot'], df['Square'], 'r.', label='Square', markersize=7)
plt.plot(df['Slot'], df['Multiply'], 'g.', label='Multiply', markersize=7)
# plt.plot(df['Slot'], df['Reduce'], 'b.', label='Reduce', markersize=7)
# Set y-axis limits to focus on the relevant range
plt.ylim(0, 1000)  

plt.xlabel('Time (samples)')
plt.ylabel('Access time (cycles)')
plt.title('Modular Exponentiation Trace')
plt.legend()

# Add a grid
plt.grid(True, linestyle='--', alpha=0.7)

plt.savefig('probe_results_gmp_rsa.png', dpi=300, bbox_inches='tight')
plt.show()




# # Read the CSV file
# df = pd.read_csv('probe_results_ver2.csv')

# # Create the plot
# plt.figure(figsize=(15, 8))
# # plt.plot(df['Slot'], df['Square'], 'r.', label='Square', markersize=2)
# # plt.plot(df['Slot'], df['Multiply'], 'g.', label='Multiply', markersize=2)
# # plt.plot(df['Slot'], df['Reduce'], 'g.', label='Reduce', markersize=2)

# plt.plot(df['Slot'], df['Square'], 'r.', label='Square', linewidth=1)
# plt.plot(df['Slot'], df['Multiply'], 'g.', label='Multiply', linewidth=1)

# plt.axhline(y=100, color='k', linestyle='--', label='Threshold')

# plt.xlabel('Time Slot Number')
# plt.ylabel('Probe Time (cycles)')
# plt.title('Time measurements of probes')
# plt.legend()

# # Add a rectangle to highlight a specific area (it should be similar to the paper)
# plt.axvspan(3920, 3930, alpha=0.3, color='gray')

# plt.savefig('probe_results_Mod_Expo_ver2.png', dpi=300, bbox_inches='tight')
# plt.show()