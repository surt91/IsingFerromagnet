#! /usr/bin/env python

from subprocess import call
from numpy import arange

# Generieren der Ergebnisse
for i in arange(1.0,4.0,0.2):
    call(["./test", "{0:.1f}".format(i)])
    
# Plotten der Kurven
#for i in arange(1.0,4.0,0.2):
#    call(["sed", "-i", {0:.1f}".format(i)])
