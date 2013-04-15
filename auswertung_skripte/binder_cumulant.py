#! /usr/bin/env python2

from reader import *
import os, sys
from subprocess import call

g=[]
M=[]
V=[]
tau=[]
for f in os.listdir("../data"):
    if ".png" in f:# or "_up" in f:
        continue
    T=f.split("_")[2]
    L=f.split("_")[4]

    r = output_reader(os.path.join("../data",f))
    g.append([T,L,(r.getBinder(),r.getBinderErrorJackknife(r.getBinder,r.M))])
    #~ M.append([T,L,(r.getMeanM(),sqrt(r.getVarM))])
    #tau.append([T,L,(r.getAutocorrTime(),0)])

for [val,fname] in ((g,"binder.dat"),):
    out = sorted(val)
    f = open(fname, "w")

    #output for gnuplot
    f.write("#")
    for i in out:
        f.write("{0} ".format(i[1]))
    f.write("\n")
    tmp=""
    for i in out:
        if tmp != i[0]:
            if tmp != "":
                f.write("\n")
            f.write("{0} ".format(i[0]))
            tmp=i[0]
        f.write("{0} {1} ".format(i[2][0],i[2][1]))

    f.close()

call(["gnuplot", "plot_all.gp"])
