#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import os
from reader import *

def getJFromDat(dataPath):
    d = {}

    for f in sorted(os.listdir(dataPath)):
        if not ".dat" in f:
            continue
        if "L_032_" in f:
            continue

        r = output_reader(os.path.join(dataPath,f))
        try:
            d[r.sigma].append(float(r.J))
        except KeyError:
            d.update({r.sigma:[float(r.J)]})

    print d
    for key in d.keys():
        d[key] = [mean(d[key]), std(d[key])/sqrt(len(d[key]))]

    print dataPath
    for [key, val] in sorted(d.items()):
        print key, " : ", val[0], "+-", val[1]
    print " "

#~ getJFromDat("../data/GG")
getJFromDat("../data/RNG")
