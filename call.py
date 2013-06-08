#! /usr/bin/env python2
# -*- coding: utf-8 -*-

import os
from subprocess import call
from multiprocessing import Process, cpu_count
from numpy import arange
from auswertung.getConfig import getConfig

def f(config):
    """Diese Funktion startet die Simulation für verschiedene Parameter"""
    n=0
    nWorker = cpu_count()
    p=[]
    for [s,x,N,l,t,t_eq,tau] in config:
        for x in range(1,x+1):
            opts = ["./ising", "-T{0}".format(t),
                   "-L{0}".format(l), "-e{0}".format(t_eq),
                   "-N{0}".format(t_eq+N*tau),
                   "-v", "-u0", "-x{0}".format(x), "-w", "-p",
                   "-s{0}".format(s), "-a0.5", #"-gOut_s_{0}_x_{1}_L{2}.svg".format(s,x,l),
                   "-t2", "-i{0}".format(tau)]
            print " ".join(opts)

            if n < nWorker:
                p.append(Process(target=call, args=(opts,)))
                p[-1].start()
                n+=1
            else:
                [i.join() for i in p]
                n=0
                p=[]

if __name__ == '__main__':
    directory = "data/"
    if not os.path.exists(directory):
        os.makedirs(directory)
    configs = getConfig()
    # Generieren der Ergebnisse
    f(configs)
