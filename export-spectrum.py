import matplotlib.pyplot as plt
from scipy.fftpack import fft
from scipy.io import wavfile # get the api
fs, data = wavfile.read('1.wav') # load the data
a = data.T[0] # this is a two channel soundtrack, I get the first track
a = a[:10000]
c = fft(a)
for spec in c:
	print "%f %f" % (spec.real, spec.imag)