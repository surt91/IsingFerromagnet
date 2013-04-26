#! /usr/bin/env python

from subprocess import call
from numpy import arange
from auswertung.getConfig import getConfig

def f(config):
    for s in arange(0,1.3,0.1):
        for [l,t,t_eq,tau] in config:
            #~ if s == 0: end = 1
            #~ else: end = 12
            end = 12
            for x in range(1,end+1):
                call(["time", "./test", "-T{0}".format(t),
                       "-L{0}".format(l), "-e{0}".format(t_eq),
                       "-N{0}".format(t_eq+500),
                       "-v", "-u0", "-x{0}".format(x), "-w", "-p",
                       "-s{0}".format(s), "-a0.5", #"-gOut_s_{0}_x_{1}_L{2}.svg".format(s,x,l),
                       "-t1", "-i{0}".format(tau)])

if __name__ == '__main__':
    # Also Grosse und Equilibriums Zeit
    # Diese sind durch tests in zeiten.par eingetragen
    configs = getConfig()
    # Generieren der Ergebnisse
    f(configs)
