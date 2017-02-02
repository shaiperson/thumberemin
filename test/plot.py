import matplotlib.pyplot as plt
from sys import argv as args

datafile = open(args[1], 'r')

strsamples = datafile.read().split(',')
samples = map(float, strsamples)

plt.plot(samples, 'c.')
plt.show()
