import numpy as np
import matplotlib.pyplot as plt
from sys import argv as args

datafile = open(args[1], 'r')
joint = int(args[2])

strsamples = datafile.read().split(',')
samples = map(float, strsamples)
samplesarr = np.asarray(samples)

xs = np.arange(len(samplesarr))
colors = [[0,0,1] for i in xs]
# colors[joint] = [1,0,0]

plt.plot(xs, samplesarr, linestyle = 'solid')
plt.scatter([joint], [samples[joint]], c = [[1,0,0]])
plt.show()

# print range(0, len(samples))
