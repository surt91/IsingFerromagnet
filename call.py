#! /usr/bin/env python

from subprocess import call
from numpy import arange

for l in [16, 32, 64, 128, 256]:
    call(["make","clean"])
    call(["sed", "-e", "s/#-DUP/-DUP/g","-i", "makefile"])
    call(["sed", "-e", "s/L=[0-9]\+/L={0}/g".format(l),"-i", "spin.c"])
    call(["make"])
    # Generieren der Ergebnisse
    for i in arange(1.0,4.0,0.2):
        call(["./test", "{0:.1f}".format(i)])

    call(["make","clean"])
    call(["sed", "-e", "s/-DUP/#-DUP/g".format(i-0.2,i),"-i", "makefile"])
    call(["make"])
    # Generieren der Ergebnisse
    for i in arange(1.0,4.0,0.2):
        call(["./test", "{0:.1f}".format(i)])

    # Plotten der Kurven
    call(["gnuplot", "plot.gp"])
    for i in arange(1.0,4.0,0.2):
        call(["sed", "-e", "s/{0:.2f}/{1:.2f}/g".format(i-0.2,i),"-i", "plot.gp"])
        call(["sed", "-e", "s/L_[0-9]\+/L_{0}/g".format(l),"-i", "plot.gp"])
        call(["gnuplot", "plot.gp"])
    call(["sed", "-e", "s/3.80/1.00/g","-i", "plot.gp"])
