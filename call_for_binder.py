#! /usr/bin/env python

from subprocess import call
from numpy import arange

# Generieren der Ergebnisse
for l in [16, 32, 64]:
    for t in arange(1,4,0.02):
        call(["./test", "-T{0:.2f}".format(t), "-L{0}".format(l), "-e5000", "-N80000", "-v", "-u1", "-x13"])
        call(["./test", "-T{0:.2f}".format(t), "-L{0}".format(l), "-e5000", "-N80000", "-v", "-u0", "-x42"])
