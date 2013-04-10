#! /usr/bin/env python

from subprocess import call
from numpy import arange

# Generieren der Ergebnisse
for i in arange(1.0,4.0,0.2):
    call(["./test", "{0:.1f}".format(i)])

# Plotten der Kurven
call(["gnuplot", "plot.gp"])
for i in arange(1.0,4.0,0.2):
    call(["sed", "-e", "s/{0:.2f}/{1:.2f}/g".format(i-0.2,i),"-i", "plot.gp"])
    call(["gnuplot", "plot.gp"])
call(["sed", "-e", "s/3.80/1.00/g","-i", "plot.gp"])
