#! /usr/bin/env python

from subprocess import call
from numpy import arange

for l in [16, 32, 64]:
    call(["make","clean"])
    call(["sed", "-e", "s/L=[0-9]\+/L={0}/g".format(l),"-i", "spin.c"])
    call(["make"])
    # Generieren der Ergebnisse
    for i in arange(2.2,2.4,0.01):
        call(["./test", "{0:.2f}".format(i)])

