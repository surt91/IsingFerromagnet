#! /usr/bin/env python

import csv
with open('data/data_T_1.00_rand.dat', 'rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=' ')
    reader.next()
    m = [float(i[2]) for i in reader]

t_max = len(m)

def X(t):
    eins = [m[i] * m[i+t] for i in range(0,t_max-t)]
    zwei = [m[i] for i in range(0,t_max-t)]
    drei = [m[i+t] for i in range(0,t_max-t)]
    return (1/(t_max-t)* (sum(eins) - sum(zwei) * sum(drei)))

autocorr = [X(t) for t in range(0,t_max)]

for i in autocorr:
    print i
