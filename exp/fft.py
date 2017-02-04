from fileinput import input
import numpy
import matplotlib.pyplot as plt

samples = []

for line in input():
    samples.append(line)

samples = samples[0].split(',')
samples = map(float, samples)

f = numpy.fft.rfft(samples)

plt.plot(f)
plt.show()
