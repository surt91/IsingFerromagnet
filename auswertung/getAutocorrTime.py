#! /usr/bin/env python2
# -*- coding: utf-8 -*-

from numpy import arange, mean
import numpy
from reader import *
import os
from subprocess import call
from getConfig import getConfig

def autocorr(x):
    """! autocorr()

    Diese Funktion berechnet die Autokorrelationsfunktion.
    Berechnung erfolgt per FFT und Faltung. O(nlog(n))

    vgl. http://stackoverflow.com/questions/643699/how-can-i-use-numpy-correlate-to-do-autocorrelation
    """
    return numpy.correlate(x, x, mode='full')[len(x)-1:]

def getAutocorrTime(m):
    """! getAutocorrTime()

        Berechnet die Autokorrelationszeit \f$ \tau \f$

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
    sigmas = [0,1]

    out = [[0 for y in sigmas] for x in config]
    for [i,s] in enumerate(sigmas):
        for [j,[l,t,t_eq,tau]] in enumerate(config):
            filename = "{2}/S_{0}_L_{1}_ra.dat".format(s,l, directory)
            call(["time", "../test", "-T{0}".format(t),
                   "-L{0}".format(l), "-e{0}".format(t_eq),
                   "-N{0}".format(N),
                   "-v", "-u0", "-x42", "-w", "-p",
                   "-s{0}".format(s), "-a0.5",
                   "-t1", "-o{0}".format(filename)])
            r = output_reader(filename)
            tauM = [getAutocorrTime(r.M[n]) for n in range(len(r.T))]
            out[j][i] = (r.T,tauM)

    for [j,[l,t,t_eq,tau]] in enumerate(config):
        name="S_{0}_L_{1}_ra".format(s,l)
        filename = os.path.join(directory,name+".dat")
        f = open(filename+".gp", "w")
        #~ f.write("set terminal  postscript eps enhanced\n")
        #~ f.write("set output '{0}'\n".format(name+".eps"))
        f.write("set terminal png\n")
        f.write("set output '{0}'\n".format(name+".png"))
        f.write("set xlabel 'Temperatur'\n")
        f.write("set ylabel '{0}'\n".format("Autokorrelationszeit"))
        f.write("set key right\n")

        f.write("plot ")
        tmpStr = ""
        for [i,s] in enumerate(sigmas):
            tmpStr+=("'{0}' using 1:{1} w l title {2}, ".format(name+".dat", i+2, "'sigma" +" = {0}'".format(s)))
        f.write(tmpStr[:-2])
        f.close()

        f = open(filename, "w")
        for [n,T] in enumerate(out[j][i][0]):
            f.write("{0}".format(T))
            for [i,s] in enumerate(sigmas):
                # Spalten
                f.write(" {0}  ".format(out[j][i][1][n]))
            f.write("\n")
        f.close()
