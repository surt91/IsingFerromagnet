#! /usr/bin/env python

from subprocess import call
from numpy import arange

def f(config):
    t=",".join([str(i) for i in arange(0.1,3.3,0.1)]) #passend fur 16 und 32 fur grossere muss ich noch finden
    for s in arange(0,1.3,0.1):
        for [l,t_eq] in config:
            if s == 0: end = 1
            else: end = 12
            for x in range(1,end+1):
                call(["time", "./test", "-T{0}".format(t),
                       "-L{0}".format(l), "-e{0}".format(t_eq),
                       "-N{0}".format(t_eq+500),
                       "-v", "-u0", "-x{0}".format(x), "-w", "-p",
                       "-s{0}".format(s), "-a0.5", #"-gOut_s_{0}_x_{1}_L{2}.svg".format(s,x,l),
                       "-t1"])

if __name__ == '__main__':
    # Tupel aus L, T_eq
    # Also Grosse und Equilibriums Zeit
    # Diese sind durch tests in zeiten.par eingetragen
    configs = [ ( 16,  30*2),
                ( 32,  50*2)
                #~ ( 64, 1000*2)
                #~ (128, 2000*2)
                                  ]
    # Generieren der Ergebnisse
    f(configs)
