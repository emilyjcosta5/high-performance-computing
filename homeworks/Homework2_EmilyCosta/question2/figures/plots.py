import matplotlib.pyplot as plt
from matplotlib.patches import Patch
import pandas as pd
import numpy as np

df = pd.read_csv('hw2-q2.csv')
df['Time in Microseconds'] = df['N']/df['Time in Microseconds']

# left: weak, right: strong
# bottom: Global Access, top: Scatter Data
# Hue- skyblue: pthreads, Hue- maroon: OpenMP
fig, axes = plt.subplots(2,2,figsize=(6,4),sharex=True,sharey=True)
d = 1 # degree of polyfit for linear regs

# 1. Plot all pthread results
mask = df['Parallel Framework'] == 'pthreads'
pos = np.flatnonzero(mask)
framework = df.iloc[pos]
p_color='lightblue'
l_color='blue'
    # 1.a. Plot weak scale
mask = framework['Type of Scaling'] == 'weak'
pos = np.flatnonzero(mask)
scale = framework.iloc[pos]
        # 1.a.i. Plot scatter
mask = scale['Technique'] == 'Scatter'
pos = np.flatnonzero(mask)
technique = scale.iloc[pos]
x = technique['Number of Threads']
y = technique['Time in Microseconds']
axes[0,0].scatter(x,y, color=p_color)
m, b = np.polyfit(x, y, d)
axes[0,0].plot(x, m*x+b, '--', color=l_color)
        # 1.a.ii. Plot global
mask = scale['Technique'] == 'Global'
pos = np.flatnonzero(mask)
technique = scale.iloc[pos]
x = technique['Number of Threads']
y = technique['Time in Microseconds']
axes[1,0].scatter(x,y, color=p_color)
m, b = np.polyfit(x, y, d)
axes[1,0].plot(x, m*x+b, '--', color=l_color)
    # 1.b. Plot strong scale
mask = framework['Type of Scaling'] == 'strong'
pos = np.flatnonzero(mask)
scale = framework.iloc[pos]
        # 1.b.i. Plot scatter
mask = scale['Technique'] == 'Scatter'
pos = np.flatnonzero(mask)
technique = scale.iloc[pos]
x = technique['Number of Threads']
y = technique['Time in Microseconds']
axes[0,1].scatter(x,y, color=p_color)
m, b = np.polyfit(x, y, d)
axes[0,1].plot(x, m*x+b, '--', color=l_color)
        # 1.b.ii. Plot global
mask = scale['Technique'] == 'Global'
pos = np.flatnonzero(mask)
technique = scale.iloc[pos]
x = technique['Number of Threads']
y = technique['Time in Microseconds']
axes[1,1].scatter(x,y, color=p_color)
m, b = np.polyfit(x, y, d)
axes[1,1].plot(x, m*x+b, '--', color=l_color)

# 2. Plot all OpenMP results
mask = df['Parallel Framework'] == 'omp'
pos = np.flatnonzero(mask)
framework = df.iloc[pos]
p_color='lightcoral'
l_color='red'
    # 2.a. Plot weak scale
mask = framework['Type of Scaling'] == 'weak'
pos = np.flatnonzero(mask)
scale = framework.iloc[pos]
        # 2.a.i. Plot scatter
mask = scale['Technique'] == 'Scatter'
pos = np.flatnonzero(mask)
technique = scale.iloc[pos]
x = technique['Number of Threads']
y = technique['Time in Microseconds']
axes[0,0].scatter(x,y, color=p_color)
m, b = np.polyfit(x, y, d)
axes[0,0].plot(x, m*x+b, '--', color=l_color)
        # 2.a.ii. Plot global
mask = scale['Technique'] == 'Global'
pos = np.flatnonzero(mask)
technique = scale.iloc[pos]
x = technique['Number of Threads']
y = technique['Time in Microseconds']
axes[1,0].scatter(x,y, color=p_color)
m, b = np.polyfit(x, y, d)
axes[1,0].plot(x, m*x+b, '--', color=l_color)
    # 2.b. Plot strong scale
mask = framework['Type of Scaling'] == 'strong'
pos = np.flatnonzero(mask)
scale = framework.iloc[pos]
        # 2.b.i. Plot scatter
mask = scale['Technique'] == 'Scatter'
pos = np.flatnonzero(mask)
technique = scale.iloc[pos]
x = technique['Number of Threads']
y = technique['Time in Microseconds']
axes[0,1].scatter(x,y, color=p_color)
m, b = np.polyfit(x, y, d)
axes[0,1].plot(x, m*x+b, '--', color=l_color)
        # 2.b.ii. Plot global
mask = scale['Technique'] == 'Global'
pos = np.flatnonzero(mask)
technique = scale.iloc[pos]
x = technique['Number of Threads']
y = technique['Time in Microseconds']
axes[1,1].scatter(x,y, color=p_color)
m, b = np.polyfit(x, y, d)
axes[1,1].plot(x, m*x+b, '--', color=l_color)

# Aesthetics
axes[0,0].set_yscale('log')
axes[0,0].set_xlim(1,8)
axes[0,0].set_title('(1) scatter, weak scale')
axes[0,1].set_title('(2) scatter, strong scale')
axes[1,0].set_title('(3) global, weak scale')
axes[1,1].set_title('(4) global, strong scale')
fig.text(0.5, 0.01, 'Number of Threads', va='bottom', ha='center')
fig.text(0.01, 0.5, 'Performance (arraySize/microseconds)', va='center', ha='left',rotation=90)
legend_elements = [Patch(facecolor='lightblue',edgecolor='blue',label='pthreads'),
        Patch(facecolor='lightcoral',edgecolor='red',label='OpenMP')]
fig.legend(handles=legend_elements, loc='upper center', ncol=2)
fig.subplots_adjust(left=0.11, bottom=0.12, right=0.98, top=0.85, wspace=0.05, hspace=0.25)
plt.savefig('q2-benchmarks.pdf')
