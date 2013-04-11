#! /usr/bin/env python2

from numpy import arange, mean, var
from reader import *
import os
from subprocess import call

def binder(m):
    m4 = mean([abs(i**4) for i in m])
    m2 = mean([abs(i**2) for i in m])

    g=(3-m4/(m2**2))/2

    return g

g=[]
M=[]
V=[]
for f in os.listdir("../data"):
    if ".png" in f or "_up" in f:
        continue
    T=f.split("_")[2]
    L=f.split("_")[4]

    r = output_reader(os.path.join("../data",f))
    m = r.M
    g.append([T,L,binder(m)])
    M.append([T,L,mean(m)])
    V.append([T,L,var(m)])

out = sorted(g)

f = open("binder.dat", "w")

#output for gnuplot
tmp=""
for i in out:
    if tmp != i[0]:
        if tmp != "":
            f.write("\n")
        f.write("{} ".format(i[0]))
        tmp=i[0]
    f.write("{} ".format(i[2]))

f.close()

#~ call(["gnuplot", "plot_binder.gp"])

##
out = sorted(M)

f = open("mean.dat", "w")

#output for gnuplot
tmp=""
for i in out:
    if tmp != i[0]:
        if tmp != "":
            f.write("\n")
        f.write("{} ".format(i[0]))
        tmp=i[0]
    f.write("{} ".format(i[2]))

f.close()


##
out = sorted(M)

f = open("var.dat", "w")

#output for gnuplot
tmp=""
for i in out:
    if tmp != i[0]:
        if tmp != "":
            f.write("\n")
        f.write("{} ".format(i[0]))
        tmp=i[0]
    f.write("{} ".format(i[2]))

f.close()

call(["gnuplot", "plot_all.gp"])
