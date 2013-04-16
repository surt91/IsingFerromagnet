#! /usr/bin/env python

from subprocess import call
from numpy import arange
from multiprocessing import Process, cpu_count

def f(l,t_eq,tau):
    t=",".join([str(i) for i in arange(1.0,3.3,0.1)])
    call(["time", "-f%U", "./test", "-p{0}".format(t),
           "-L{0}".format(l), "-e{0}".format(t_eq),
           "-N{0}".format(t_eq+500*tau), "-i{0}".format(tau),
           "-v", "-u1", "-x13", "-w"])

if __name__ == '__main__':
    n=0
    nWorker = cpu_count()*2
    # Tupel aus L, T_eq, tau
    # Also Grosse, Equilibriums Zeit und Korrelationszeit
    # Diese sind durch tests in zeiten.par eingetragen
    configs = [ ( 16,  200*2,  3),
                ( 32,  400*2,  3),
                ( 64, 1000*2,  3)
                #~ (128, 2000*2,  15*2)
                                  ]
    # Generieren der Ergebnisse
    for [l,t_eq,tau] in configs:
        n+=1
        #! bearbeite mehrere Prozesse gleichzeitig
        p = Process(target=f, args=(l,t_eq,tau))
        p.start()
        if n >= nWorker:
            p.join()
            n=0
