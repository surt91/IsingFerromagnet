#! /usr/bin/env python

from subprocess import call
from numpy import arange
from multiprocessing import Process, Pool,cpu_count

def f(t,l):
    call(["time", "-f%E", "./test", "-T{0:.2f}".format(t), "-L{0}".format(l), "-e5000", "-N15000", "-v", "-u1", "-x13"])
    call(["time", "-f%E", "./test", "-T{0:.2f}".format(t), "-L{0}".format(l), "-e5000", "-N15000", "-v", "-u0", "-x42"])

if __name__ == '__main__':
    n=0
    nWorker = cpu_count()*2
    # Generieren der Ergebnisse
    for l in [16, 32, 64]:
        for t in arange(1.8,3.3,0.1):
            #! bearbeite mehrere Prozesse gleichzeitig
            if n < nWorker:
                p = Process(target=f, args=(t,l))
                p.start()
                n+=1
            else:
                p = Process(target=f, args=(t,l))
                p.start()
                p.join()
                n=0
