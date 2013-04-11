#! /usr/bin/env python2

from numpy import arange, mean
from reader import *
import os
from subprocess import call

for filename in os.listdir("../data"):
    if ".png" in filename:
        continue
    if "_up" in filename:
        filenameWithPath=os.path.join("../data",filename)
        f = open("plot_tmp.gp", "w")
        f.write("set terminal png\n")
        f.write("set output '{0}'\n".format(filenameWithPath.replace("_up.dat",".png")))
        f.write("plot '{0}' using 1:3 w l, '{1}' using 1:3 w l;\n".format(filenameWithPath, filenameWithPath.replace("_up","_rand")))
        f.close()

        call(["gnuplot", "plot_tmp.gp"])
