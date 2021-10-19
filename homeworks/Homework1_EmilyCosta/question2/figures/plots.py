import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('benchmarks.csv')
df['Size of Array'] = df['Size of Array'].astype(int)
df['Number of Threads'] = df['Number of Threads'].astype(int)
df['Execution Time (ms)'] = df['Execution Time (ms)'].astype(int)

mask = df['Size of Array'] == 5000
pos = np.flatnonzero(mask)
weak_scale = df.iloc[pos]
mask = df['Size of Array'] == 5000*df['Number of Threads']
pos = np.flatnonzero(mask)
strong_scale = df.iloc[pos]

fig, axes = plt.subplots(3,1,figsize=(5,4.5),sharex=True)
plt.minorticks_off()

x=weak_scale.loc[:,'Number of Threads']
y=weak_scale.loc[:,'Execution Time (ms)']
axes[0].plot(x, y, 'ro')
m, b = np.polyfit(x, y, 1)
axes[0].plot(x, m*x+b, '--', color='orange') 
axes[0].set_xlim(1,8)
axes[0].set_xscale('log')
axes[0].set_xticks([1,2,4,8])
axes[0].set_xticklabels([1,2,4,8])
axes[0].tick_params(axis='x', which='minor', bottom=False, labelcolor='white')
axes[0].yaxis.grid(color='lightgrey', linestyle=':')
axes[0].set_axisbelow(True)
axes[0].set_ylim(0,6)
axes[0].set_title('(a) Weak Scaling at n=5000')
#axes[0].set_ylabel('Execution Time (ms)')

x=strong_scale.loc[:,'Number of Threads']
y=strong_scale.loc[:,'Execution Time (ms)']
axes[1].plot(x, y,'bo')
m, b = np.polyfit(x, y, 1)
axes[1].plot(x, m*x+b, '--', color='green')
axes[1].set_xlim(1,8)
axes[1].set_xscale('log')
axes[1].set_xticks([1,2,4,8])
axes[1].set_xticklabels([1,2,4,8])
axes[1].tick_params(axis='x', which='minor', labelcolor='white', color='white')
axes[1].yaxis.grid(color='lightgrey', linestyle=':')
axes[1].set_axisbelow(True)
axes[1].set_ylim(0,150)
axes[1].set_title('(b) Strong Scaling at n=5000*x')
axes[1].set_ylabel('Execution Time (ms)')

df = pd.read_csv('benchmarks_log.csv')
x=df.loc[:,'Number of Threads']
y=df.loc[:,'Execution Time (ms)']
axes[2].plot(x, y,'bo')
m, b = np.polyfit(x, y, 1)
axes[2].plot(x, m*x+b, '--', color='green')
axes[2].set_xlim(1,8)
axes[2].set_xscale('log')
axes[2].set_xticks([1,2,4,8])
axes[2].set_xticklabels([1,2,4,8])
axes[2].tick_params(axis='x', which='minor', labelcolor='white', color='white')
axes[2].yaxis.grid(color='lightgrey', linestyle=':')
axes[2].set_axisbelow(True)
axes[2].set_ylim(0,600)
axes[2].set_title('(c) Strong Scaling at n=5000*x*log(5000*x)')
#axes[2].set_ylabel('Execution Time (ms)')
axes[2].set_xlabel('Number of Threads')

plt.tight_layout()
plt.savefig('benchmarks.pdf')
