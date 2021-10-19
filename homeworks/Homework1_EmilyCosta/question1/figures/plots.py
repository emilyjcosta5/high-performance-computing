# CPU-FLOPs measured in G-FLOPS for 100k loops
benchmark1 = []
benchmark1.extend([2.91, 3.13, 3.12, 3.18, 3.15]) #O0
benchmark1.extend([7.93, 8.32, 8.44, 15.20, 6.43]) #O1
benchmark1.extend([15.29, 15.79, 15.48, 15.97, 15.32]) #O2
benchmark1.extend([14.02, 15.34, 13.90, 14.55, 14.52]) #O3

# CPU-IOPS measured in G-IOPS for 100k loops
benchmark2 = []
benchmark2.extend([9.27, 3.14, 3.29, 3.26, 3.13]) #O0
benchmark2.extend([16.93, 6.17, 5.89, 6.38, 6.46]) #O1
benchmark2.extend([25.12, 15.19, 14.80, 15.83, 16.22]) #O2
benchmark2.extend([17.37, 15.33, 14.07, 17.20, 14.55]) #O3
#benchmark2_O4 = [13.24, 16.37, 17.54, 15.68, 16.08]

# Memory benchmark Bytes Copied per Second (MB/s)
benchmark3 = []
benchmark3.extend([3499, 3339, 3173, 2995, 2985]) #O0
benchmark3.extend([7167, 7135, 7135, 6995, 6837]) #O1
benchmark3.extend([7143, 7175, 6955, 7205, 6960]) #O2
benchmark3.extend([7051, 6969, 6969, 6972, 7048]) #O3

optimizations = ['O0','O0','O0','O0','O0',
                'O1', 'O1', 'O1', 'O1', 'O1', 
                'O2', 'O2', 'O2', 'O2', 'O2', 
                'O3', 'O3', 'O3', 'O3', 'O3']

import seaborn as sns
import matplotlib.pyplot as plt


fig, ax = plt.subplots(1,1,figsize=(5,2.5))
sns.boxplot(ax=ax, x=optimizations, y=benchmark1)
ax.yaxis.grid(color='lightgrey', linestyle=':')
ax.set_axisbelow(True)
ax.set_ylim(0,20)
ax.set_title('CPU-FLOPs Benchmark')
ax.set_ylabel('G-FLOPs')
ax.set_xlabel('Optimization')
plt.tight_layout()
plt.savefig('benchmark_of_benchmark1.pdf')
plt.clf()

fig, ax = plt.subplots(1,1,figsize=(5,2.5))
sns.boxplot(ax=ax, x=optimizations, y=benchmark2)
ax.yaxis.grid(color='lightgrey', linestyle=':')
ax.set_axisbelow(True)
ax.set_ylim(0,20)
ax.set_title('CPU-IOPs Benchmark')
ax.set_ylabel('G-IOPs')
ax.set_xlabel('Optimization')
plt.tight_layout()
plt.savefig('benchmark_of_benchmark2.pdf')
plt.clf()

fig, ax = plt.subplots(1,1,figsize=(5,2.5))
sns.boxplot(ax=ax, x=optimizations, y=benchmark3)
ax.yaxis.grid(color='lightgrey', linestyle=':')
ax.set_axisbelow(True)
ax.set_ylim(0,8000)
ax.set_title('Memory Bandwidth Benchmark')
ax.set_ylabel('Bytes Copied per\nSecond (MB/s)')
ax.set_xlabel('Optimization')
plt.tight_layout()
plt.savefig('benchmark_of_benchmark3.pdf')
plt.clf()
