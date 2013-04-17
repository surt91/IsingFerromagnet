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

def X(t, m):
    """! Autokorreltationsfunktion

        Diese Funktion berechnet dei Autokorrelationsfunktion bei der
        Verschiebung um t. Berechnung erfolgt naiv per Definition
        in O(n) Laufzeit.
    """
    t_max = len(m)
    eins = mean([m[i] * m[i+t] for i in range(0,t_max-t)])
    zwei = mean([m[i] for i in range(0,t_max-t)]) * mean([m[i+t] for i in range(0,t_max-t)])
    return abs(eins - zwei)

def getAutocorrTime(m):
    """! Berechnet die Autokorrelationszeit \f$ \tau \f$

        Die Berechnung erfolgt durch Integration (hier sollte eine
        Summation genau genug sein) der Autokorrelationsfunktion.
        vgl. \cite newman1999monte S. 62 (3.20)
    """
    x0 = X(0,m)
    autocorr = [X(t,m) for t in range(0,len(m))]
    #~ self.tau = trapz(autocorr)/x0
    tau = sum(autocorr)/x0
    #~ for i in autocorr:
        #~ print i
    return tau

g=[]
M=[]
V=[]
tau=[]
for f in os.listdir("../data"):
    if not ".dat" in f:
        continue

    r = output_reader(os.path.join("../data",f))
    L = r.L

    print "calculating"

    for i in range(len(r.T)):
        g.append([r.T[i],L,(getBinder(r.M[i]), getErrorJackknife(getBinder,r.M[i]))])
        M.append([r.T[i],L,(mean([abs(j) for j in r.M[i]]), std([abs(j) for j in r.M[i]]))])
        #~ tau.append([r.T[i],L,(getAutocorrTime(r.M[i]), 0)])

for [val,fname] in ((g,"binder.dat"),(M,"mean.dat"),):
    out = sorted(val)
    f = open(fname, "w")

    #output for gnuplot
    f.write("#")
    for i in range(len(out[1])):
        f.write(" {0}".format(out[i][1]))
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
