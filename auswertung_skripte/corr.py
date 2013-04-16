#! /usr/bin/env python2

from numpy import arange, mean
from reader import *
import os
from subprocess import call

i=0
for filename in os.listdir("../data"):
    if ".png" in filename:
        continue
    if ".dat" in filename:
        i+=1
        filenameWithPath=os.path.join("../data",filename)
        f = open("plot_tmp.gp", "w")
        for j in range(3,47,2):
            f.write("set terminal png\n")
            f.write("set output '{0}'\n".format(os.path.join("../data/equi_plots",filename)+"{0}_{1}_M".format(i,j)+".png"))
            f.write("plot '{0}' using 1:(abs(${1})) w l\n\n".format(filenameWithPath,j))
            f.write("set terminal png\n")
            f.write("set output '{0}'\n".format(os.path.join("../data/equi_plots",filename)+"{0}_{1}_E".format(i,j)+".png"))
            f.write("plot '{0}' using 1:{1} w l\n\n\n".format(filenameWithPath,j-1))
        f.close()

        call(["gnuplot", "plot_tmp.gp"])
