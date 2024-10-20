import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('probe_result.csv')


# Apply a moving average to smooth the data
window_size = 5
df['Square_smooth'] = df['Square'].rolling(window=window_size, center=True).mean()
df['Multiply_smooth'] = df['Multiply'].rolling(window=window_size, center=True).mean()
df['Reduce_smooth'] = df['Reduce'].rolling(window=window_size, center=True).mean()

# Create the plot
plt.figure(figsize=(150, 8))

plt.plot(df['Slot'], df['Square'], 'r.', label='Square', markersize=1)
plt.plot(df['Slot'], df['Multiply'], 'g.', label='Multiply', markersize=1)
# plt.plot(df['Slot'], df['Reduce'], 'b.', label='Reduce', markersize=1)

# Set y-axis limits to focus on the relevant range
plt.ylim(0, 100) 

# Set x-axis limits to start from 0
plt.xlim(0, df['Slot'].max())

plt.xlabel('Time (samples)')
plt.ylabel('Access time (cycles)')
plt.title('Modular Exponentiation Trace')
plt.legend()

# Add a grid
plt.grid(True, linestyle='--', alpha=0.7)

plt.savefig('probe_result.png', dpi=300, bbox_inches='tight')
plt.show()