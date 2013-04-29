#! /usr/bin/env python2

from numpy import arange, mean
import numpy
from reader import *
import os
from subprocess import call
from getConfig import getConfig

def autocorr(x):
    """! Autokorreltationsfunktion

    Diese Funktion berechnet die Autokorrelationsfunktion.
    Berechnung erfolgt per FFT und Faltung. O(nlog(n))

    vgl. http://stackoverflow.com/questions/643699/how-can-i-use-numpy-correlate-to-do-autocorrelation
    """
    return numpy.correlate(x, x, mode='full')[len(x)-1:]

def getAutocorrTime(m):
    """! Berechnet die Autokorrelationszeit \f$ \tau \f$

        Die Berechnung erfolgt durch Integration (hier sollte eine
        Summation genau genug sein) der Autokorrelationsfunktion.
        vgl. \cite newman1999monte S. 62 (3.20)
    """
    auto = autocorr(m)
    x0 = auto[0]
    tau = sum(auto)/x0
    return tau

if __name__ == "__main__":
    directory = "../data/tau"
    if not os.path.exists(directory):
        os.makedirs(directory)
    N = 500

    config = getConfig()

    for s in [0,1]:
        for [l,t,t_eq,tau] in config:
            filename = "{2}/S_{0}_L_{1}_ra.dat".format(s,l, directory)
            call(["time", "../test", "-T{0}".format(t),
                   "-L{0}".format(l), "-e{0}".format(t_eq),
                   "-N{0}".format(N),
                   "-v", "-u0", "-x42", "-w", "-p",
                   "-s{0}".format(s), "-a0.5",
                   "-t1", "-o{0}".format(filename)])
            r = output_reader(filename)
            tauM = [getAutocorrTime(r.M[i]) for i in range(len(r.T))]
            print "L = {0}".format(l)
            print "T tau"
            for i in range(len(r.T)):
                print "{0}  {1}".format(r.T[i], tauM[i])
            print
            print
