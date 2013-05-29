#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import sqlite3
from scipy.optimize import fsolve
import numpy
from subprocess import call
import os

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
    with open(os.path.join(directory,"Tc.dat"), "w") as f:
        for [s, down, up] in zip(   (0.00, 0.03, 0.05, 0.08, 0.10, 0.12, 0.15, 0.20, 0.25, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00, 1.10, 1.20),
                                    (2.20, 2.20, 2.20, 2.20, 2.10, 2.00, 1.80, 1.50, 1.40, 1.35, 1.25, 1.20, 1.20, 1.15, 1.15, 1.15, 1.15, 1.15, 1.15),
                                    (2.35, 2.35, 2.35, 2.30, 2.30, 2.20, 2.00, 1.70, 1.55, 1.50, 1.40, 1.35, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30, 1.30)):
            f.write("{0} {1}\n".format(s,a.getIntersection(s, 16, 32, down=down, up=up)))
            f.write("{0} {1}\n".format(s,a.getIntersection(s, 32, 64, down=down, up=up)))
            f.write("{0} {1}\n".format(s,a.getIntersection(s, 64, 16, down=down, up=up)))
            f.write("{0} {1}\n".format(s,a.getIntersection(s, 128, 16, down=down, up=up)))
            f.write("{0} {1}\n".format(s,a.getIntersection(s, 128, 32, down=down, up=up)))
            f.write("{0} {1}\n".format(s,a.getIntersection(s, 128, 64, down=down, up=up)))

    with open(os.path.join(directory,"plotTc.gp"), "w") as f:
        f.write("set terminal png\n")
        f.write('set output "Tc.png"\n')
        f.write("plot 'Tc.dat'\n")
