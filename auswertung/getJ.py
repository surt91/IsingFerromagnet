#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import os
from reader import *

def getJFromDat(dataPath, outPath, L):
    J = {}
    sumJ = {}
    deg = {}
    if not os.path.exists(outPath):
        os.makedirs(outPath)

    for f in sorted(os.listdir(dataPath)):
        if not ".dat" in f:
            continue
        if not "L_{0:03d}_".format(L) in f:
            continue

        r = output_reader(os.path.join(dataPath,f))
        try:
            J[r.sigma].append(float(r.J))
        except KeyError:
            J.update({r.sigma:[float(r.J)]})
        try:
            sumJ[r.sigma].append(float(r.sumJ))
        except KeyError:
            sumJ.update({r.sigma:[float(r.sumJ)]})
        try:
            deg[r.sigma].append(float(r.deg))
        except KeyError:
            deg.update({r.sigma:[float(r.deg)]})

    #~ print d
    for key in J.keys():
        J[key] = [mean(J[key]), std(J[key])/sqrt(len(J[key]))]
    for key in sumJ.keys():
        sumJ[key] = [mean(sumJ[key]), std(sumJ[key])/sqrt(len(sumJ[key]))]
    for key in deg.keys():
        deg[key] = [mean(deg[key]), std(deg[key])/sqrt(len(deg[key]))]

    print dataPath
    print "<J>"
    with open(os.path.join(outPath, "meanJ_L{0}.dat".format(L)), "w") as f:
        for [key, val] in sorted(J.items()):
            print key, " : ", val[0], "+-", val[1]
            f.write("{0} {1} {2}\n".format(key,val[0],val[1]))
    print " "

    print "sumJ"
    with open(os.path.join(outPath, "sumJ_L{0}.dat".format(L)), "w") as f:
        for [key, val] in sorted(sumJ.items()):
            print key, " : ", val[0], "+-", val[1]
            f.write("{0} {1} {2}\n".format(key,val[0],val[1]))
    print " "

    print "deg"
    with open(os.path.join(outPath, "deg_L{0}.dat".format(L)), "w") as f:
        for [key, val] in sorted(deg.items()):
            print key, " : ", val[0], "+-", val[1]
            f.write("{0} {1} {2}\n".format(key,val[0],val[1]))
    print " "

getJFromDat("../data/GG", "GG", 16)
getJFromDat("../data/RNG", "RNG", 16)
getJFromDat("../data/GG", "GG", 32)
getJFromDat("../data/RNG", "RNG", 32)
