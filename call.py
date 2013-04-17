#! /usr/bin/env python

from subprocess import call
from numpy import arange

def f(l,t_eq):
    t=",".join([str(i) for i in arange(1.0,3.3,0.1)])
    call(["time", "./test", "-T{0}".format(t),
           "-L{0}".format(l), "-e{0}".format(t_eq),
           "-N{0}".format(t_eq+500),
           "-v", "-u1", "-x13", "", "-p"])

if __name__ == '__main__':
    # Tupel aus L, T_eq
    # Also Grosse und Equilibriums Zeit
    # Diese sind durch tests in zeiten.par eingetragen
    configs = [ ( 16,  200*2),
                ( 32,  400*2),
                ( 64, 1000*2)
                #~ (128, 2000*2)
                                  ]
    # Generieren der Ergebnisse
    for [l,t_eq] in configs:
        f(l,t_eq)
