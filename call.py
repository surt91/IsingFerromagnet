#! /usr/bin/env python
# -*- coding: utf-8 -*-

import os
from subprocess import call
from numpy import arange
from auswertung.getConfig import getConfig

def f(config):
    """Diese Funktion startet die Simulation für verschiedene Parameter"""
    for [s,l,t,t_eq,tau] in config:
        end = 12
        for x in range(1,end+1):
            opts = ["./ising", "-T{0}".format(t),
                   "-L{0}".format(l), "-e{0}".format(t_eq),
                   "-N{0}".format(t_eq+100*tau),
                   "-v", "-u0", "-x{0}".format(x), "-w", "-p",
                   "-s{0}".format(s), "-a0.5", #"-gOut_s_{0}_x_{1}_L{2}.svg".format(s,x,l),
                   "-t1", "-i{0}".format(tau)]
            print " ".join(opts)
            #~ call(opts)

if __name__ == '__main__':
    directory = "data/"
    if not os.path.exists(directory):
        os.makedirs(directory)
    configs = getConfig()
    # Generieren der Ergebnisse
    f(configs)
