#! /usr/bin/env python2

from numpy import arange, mean
from reader import *
import os
from subprocess import call

path = "../data/corr"
N = 150

t=",".join([str(i) for i in arange(0.1,3.3,0.1)]) #passend fur 16 und 32 fur grossere muss ich noch finden
#~ for s in arange(0,1.3,0.1):
    #for l in [16,32,64,128]:
for s in [0,1]:
    for l in [16, 32, 64, 128]:
        call(["time", "../test", "-T{0}".format(t),
               "-L{0}".format(l), "-e0",
               "-N{0}".format(N),
               "-v", "-u1", "-x13", "-w", "-p",
               "-s{0}".format(s), "-a0.5",
               "-t1", "-o{2}/S_{0}_L_{1}_up.dat".format(s,l, path)])
        call(["time", "../test", "-T{0}".format(t),
               "-L{0}".format(l), "-e0",
               "-N{0}".format(N),
               "-v", "-u0", "-x42", "-w", "-p",
               "-s{0}".format(s), "-a0.5",
               "-t1", "-o{2}/S_{0}_L_{1}_ra.dat".format(s,l, path)])

i=0
for filename in os.listdir(path):
    if ".png" in filename:
        continue
    if ".dat" in filename and "up" in filename :
        i+=1
        filenameWithPath=os.path.join(path,filename)
        f = open("plot_tmp.gp", "w")
        for j in range(3,66,2):
            f.write("set terminal png\n")
            f.write("set output '{0}'\n".format(os.path.join("../data/equi_plots",filename)+"{0}_T_{1}_M".format(i,t.split(",")[j/2-1])+".png"))
            f.write("plot '{0}' using 1:(abs(${1})) w l, '{2}' using 1:(abs(${1})) w l\n\n".format(filenameWithPath,j,filenameWithPath.replace("up","ra")))
            f.write("set terminal png\n")
            f.write("set output '{0}'\n".format(os.path.join("../data/equi_plots",filename)+"{0}_T_{1}_E".format(i,t.split(",")[j/2-1])+".png"))
            f.write("plot '{0}' using 1:{1} w l, '{2}' using 1:{1} w l\n\n\n".format(filenameWithPath,j-1,filenameWithPath.replace("up","ra")))
        f.close()

        call(["gnuplot", "plot_tmp.gp"])
