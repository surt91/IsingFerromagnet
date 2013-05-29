#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import sqlite3
from scipy.optimize import fsolve
import numpy
from subprocess import call
import os
from itertools import combinations
from numpy import mean, std

import warnings
warnings.simplefilter('ignore', numpy.RankWarning)
numpy.seterr(all='ignore')

class BinderIntersections():

    def __init__(self, database="data.db", deg=4, down=2.15, up=2.4):
        self.deg = deg

        self.conn = sqlite3.connect(database)

    def getBinderForSigmaAndL(self, sigma, L):
        c = self.conn.cursor()
        c.execute('SELECT T,binder,binderErr FROM calculated_data\
            WHERE sigma = ? AND L = ? AND T BETWEEN ? AND ? ORDER BY T ASC',
                                         (sigma, L, self.down, self.up))

        x = c.fetchall()

        dy = [i[2] for i in x if i[1]]
        y = [i[1] for i in x if i[1]]
        x = [i[0] for i in x if i[1]]
        return x, y, dy

    def findIntersection(self, fun1, fun2, x0):
        return fsolve(lambda x : fun1(x) - fun2(x), x0)

    def getIntersection(self, sigma, L1, L2, write=True, down=2.15, up=2.4):
        self.down = down
        self.up = up

        x1,y1,dy1 = self.getBinderForSigmaAndL(sigma, L1)
        x2,y2,dy2 = self.getBinderForSigmaAndL(sigma, L2)
        try:
            p1 = numpy.polyfit(x1, y1, self.deg)
            p2 = numpy.polyfit(x2, y2, self.deg)
        except:
            return 0

        if write:
            self.writeForGnuplot(p1, p2, x1, x2, y1, y2, dy1, dy2, L1, L2, sigma)

        result = self.findIntersection(lambda x : numpy.polyval(p1,x), lambda x : numpy.polyval(p2,x), (self.up + self.down)/2)
        return result

    def writeForGnuplot(self, p1, p2, x1, x2, y1, y2, dy1, dy2, L1, L2, sigma):
        X = numpy.arange(self.down, self.up, (self.up - self.down)/500)
        with open(os.path.join(directory,"tmpFit{0}_s_{1:.3f}.dat".format(L1,sigma)), "w") as f:
            for x in X:
                f.write(" ".join([str(x), str(numpy.polyval(p1,x))]))
                f.write("\n")

        with open(os.path.join(directory,"tmpFit{0}_s_{1:.3f}.dat".format(L2,sigma)), "w") as f:
            for x in X:
                f.write(" ".join([str(x), str(numpy.polyval(p2,x))]))
                f.write("\n")

        with open(os.path.join(directory,"tmp{0}_s_{1:.3f}.dat".format(L1,sigma)), "w") as f:
            for [x,y,dy] in zip(map(str,x1), map(str,y1), map(str,dy1)):
                f.write(" ".join([x, y, dy]))
                f.write("\n")

        with open(os.path.join(directory,"tmp{0}_s_{1:.3f}.dat".format(L2,sigma)), "w") as f:
            for [x,y,dy] in zip(map(str,x2), map(str,y2), map(str,dy2)):
                f.write(" ".join([x, y, dy]))
                f.write("\n")

        with open(os.path.join(directory,"plotAll_s_{0:.3f}.gp".format(sigma)), "w") as f:
            f.write("set terminal png\n")
            f.write('set output "out_s_{0:.3f}.png"\n'.format(sigma))
            f.write('plot "tmpFit16_s_{0:.3f}.dat" u 1:2 w l lc rgb "red", \
                          "tmpFit32_s_{0:.3f}.dat" u 1:2 w l lc rgb "green", \
                          "tmpFit64_s_{0:.3f}.dat" u 1:2 w l lc rgb "blue", \
                          "tmpFit128_s_{0:.3f}.dat" u 1:2 w l lc rgb "yellow", \
                          "tmp16_s_{0:.3f}.dat" u 1:2:3 w ye lc rgb "red", \
                          "tmp32_s_{0:.3f}.dat" u 1:2:3 w ye lc rgb "green", \
                          "tmp64_s_{0:.3f}.dat" u 1:2:3 w ye lc rgb "blue", \
                          "tmp128_s_{0:.3f}.dat" u 1:2:3 w ye lc rgb "yellow"\n'.format(sigma))


if __name__ == "__main__":
    directory = "../data/binderIntersect/"
    if not os.path.exists(directory):
        os.makedirs(directory)

    a = BinderIntersections()
    sizes = [16,32,64,128]
    print [x for x in combinations(sizes, 2)]
    sigmas = (0.00, 0.03, 0.05, 0.08, 0.10, 0.12, 0.15, 0.20, 0.25, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00, 1.10, 1.20)
    results = {}
    for [s, down, up] in zip(   sigmas,
                                (2.25, 2.25, 2.25, 2.23, 2.18, 2.08, 1.85, 1.60, 1.45, 1.38, 1.30, 1.24, 1.22, 1.20, 1.18, 1.17, 1.18, 1.18, 1.18),
                                (2.30, 2.30, 2.30, 2.30, 2.25, 2.15, 1.95, 1.68, 1.55, 1.45, 1.38, 1.32, 1.30, 1.28, 1.26, 1.26, 1.24, 1.24, 1.24)):
        tmpIntersects = [a.getIntersection(s, L1, L2, down=down, up=up) for [L1,L2] in combinations(sizes, 2)]
        results.update({s:tmpIntersects})

    with open(os.path.join(directory,"Tc.dat"), "w") as f:
        for s in sigmas:
            f.write("{0} {1} {2}\n".format(s, mean(results[s]), std(results[s])))

    with open(os.path.join(directory,"plotTc.gp"), "w") as f:
        f.write("set terminal png\n")
        f.write('set output "Tc.png"\n')
        f.write("plot 'Tc.dat' w ye\n")
