#! /usr/bin/env python2

from reader import *
import os, sys
from subprocess import call

def getBinder(m):
    """! Berechnet die Binder Kumulante

        vgl. \cite katzgraber2011introduction S. 12 (19)
    """
    m4 = mean([i**4 for i in m])
    m2 = mean([i**2 for i in m])

    g=(3-m4/(m2**2))/2

    return g

def getErrorJackknife(estimator,data):
    """! berechnet eine Unsicherheit der Binderkumulante

        Dabei wird die Jackknife Methode benutzt.
        vgl \cite young2012everything S. 12
    """
    m = data
    g = [0 for i in range(len(m))]
    for i in range(len(m)):
        m_tmp = [j for j in m]
        del m_tmp[i]

        g[i] = estimator(m_tmp)

    # Jackknife Estimate
    #mean(g)
    # Jackknife Error
    return ( sqrt(len(m)-1) * sqrt(mean([i**2 for i in g]) - mean(g)**2) )


g=[]
M=[]
V=[]
tau=[]
for f in os.listdir("../data"):
    if ".png" in f:# or "_up" in f:
        continue
    L=f.split("_")[2]

    r = output_reader(os.path.join("../data",f))

    print "calculating"

    for i in range(len(r.T)):
        g.append([r.T[i],L,(getBinder(r.M[i]), getErrorJackknife(getBinder,r.M[i]))])
        #~ g.append([r.T[i],L,(getBinder(r.M[i]), 0)])
        M.append([r.T[i],L,(mean([abs(j) for j in r.M[i]]), std([abs(j) for j in r.M[i]]))])

for [val,fname] in ((g,"binder.dat"),(M,"mean.dat"),):
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

print "plotting"
call(["gnuplot", "plot_all.gp"])
