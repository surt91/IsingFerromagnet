#! /usr/bin/env python2
# -*- coding: utf-8 -*-

from numpy import arange, mean
from reader import *
import os
from subprocess import call

from getConfig import getConfig
directory = "../data/corr"
if not os.path.exists(directory):
    os.makedirs(directory)
outDirectory = "../data/equi_plots"
if not os.path.exists(outDirectory):
    os.makedirs(outDirectory)
N = 300

config = getConfig()

for s in [0,1]:
    for [l,t,t_eq,tau] in config:
        call(["time", "../test", "-T{0}".format(t),
               "-L{0}".format(l), "-e0",
               "-N{0}".format(N),
               "-v", "-u1", "-x13", "-w", "-p",
               "-s{0}".format(s), "-a0.5",
               "-t1", "-o{2}/S_{0}_L_{1}_up.dat".format(s,l, directory)])
        call(["time", "../test", "-T{0}".format(t),
               "-L{0}".format(l), "-e0",
               "-N{0}".format(N),
               "-v", "-u0", "-x42", "-w", "-p",
               "-s{0}".format(s), "-a0.5",
               "-t1", "-o{2}/S_{0}_L_{1}_ra.dat".format(s,l, directory)])

i=0
for filename in os.listdir(directory):
    if ".png" in filename:
        continue
    if ".dat" in filename and "up" in filename :
        i+=1
        filenameWithPath=os.path.join(directory,filename)
        r = output_reader(filenameWithPath)
        f = open("plot_tmp.gp", "w")
        for j in range(3,len(r.T)*2+3,2):
            f.write("set terminal png\n")
            f.write("set output '{0}'\n".format(os.path.join(outDirectory,filename)+"{0}_T_{1}_M".format(i,r.T[j/2-1])+".png"))
            f.write("plot '{0}' using 1:(abs(${1})) w l, '{2}' using 1:(abs(${1})) w l\n\n".format(filenameWithPath,j,filenameWithPath.replace("up","ra")))
            f.write("set terminal png\n")
            f.write("set output '{0}'\n".format(os.path.join(outDirectory,filename)+"{0}_T_{1}_E".format(i,r.T[j/2-1])+".png"))
            f.write("plot '{0}' using 1:{1} w l, '{2}' using 1:{1} w l\n\n\n".format(filenameWithPath,j-1,filenameWithPath.replace("up","ra")))
        f.close()

        call(["gnuplot", "plot_tmp.gp"])
