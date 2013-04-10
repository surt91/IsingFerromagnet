#! /usr/bin/env python

from numpy import arange, mean
import csv

def binder(m):
    m4 = mean([i**4 for i in m])
    m2 = mean([i**2 for i in m])

    g=(3-m4/(m2**2))/2

    return g


for n in arange(1.0,4.0,0.2):
    print n,
    for l in [16,32,64,128]:
        with open('data/data_T_{0:.2f}_L_{1}_rand.dat'.format(n,l), 'rb') as csvfile:
            reader = csv.reader(csvfile, delimiter=' ')
            reader.next()
            m = [float(i[2]) for i in reader]

        print binder(m),
    print " "
            

