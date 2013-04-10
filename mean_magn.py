#! /usr/bin/env python

import csv
from numpy import arange, mean, var

for n in arange(1.0,4.0,0.2):
    with open('data/data_T_{0:.2f}_rand.dat'.format(n), 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=' ')
        reader.next()
        m = [float(i[2]) for i in reader]
        m = mean(m)
        print n, m
