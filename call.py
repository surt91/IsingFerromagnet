#! /usr/bin/env python

from subprocess import call
from numpy import arange
from multiprocessing import Process, cpu_count

def f(t,l,t_eq,tau):
    call(["time", "-f%E", "./test", "-T{0:.2f}".format(t),
           "-L{0}".format(l), "-e{0}".format(t_eq),
           "-N{0}".format(t_eq+1000*tau), "-i{0}".format(tau),
           "-v", "-u1", "-x13", "-w"])

if __name__ == '__main__':
    n=0
    nWorker = cpu_count()*2
    # Tupel aus L, T_eq, tau
    # Also Größe, Equilibriums Zeit und Korrelationszeit
    # Diese sind durch tests in zeiten.par eingetragen
    configs = [ ( 16,  200*2,  15*2),
                ( 32,  400*2,  15*2),
                ( 64, 1000*2,  15*2)
                                  ]
    # Generieren der Ergebnisse
    for [l,t_eq,tau] in configs:
        for t in arange(2.1,2.4,0.01):
            n+=1
            #! bearbeite mehrere Prozesse gleichzeitig
            p = Process(target=f, args=(t,l,t_eq,tau))
            p.start()
            if n >= nWorker:
                p.join()
                n=0
