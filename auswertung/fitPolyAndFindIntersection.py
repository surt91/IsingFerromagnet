#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import sqlite3
from scipy.optimize import fsolve
import numpy
from subprocess import call
import os

import warnings
warnings.simplefilter('ignore', numpy.RankWarning)

class BinderIntersections():

    def __init__(self, database="data.db", deg=4, down=2.15, up=2.4):
        self.deg = deg

        self.conn = sqlite3.connect(database)

    def getBinderForSigmaAndL(self, sigma, L):
        c = self.conn.cursor()
        c.execute('SELECT T,binder FROM calculated_data WHERE sigma = ?\
                    AND L = ? AND T BETWEEN ? AND ? ORDER BY T ASC',
                                         (sigma, L, self.down, self.up))

        x = c.fetchall()

        y = [i[1] for i in x if i[1]]
        x = [i[0] for i in x if i[1]]
        return x, y

    def findIntersection(self, fun1, fun2, x0):
        return fsolve(lambda x : fun1(x) - fun2(x), x0)

    def getIntersection(self, sigma, L1, L2, write=True, down=2.15, up=2.4):
        self.down = down
        self.up = up

        x1,y1 = self.getBinderForSigmaAndL(sigma, L1)
        x2,y2 = self.getBinderForSigmaAndL(sigma, L2)
        try:
            p1 = numpy.polyfit(x1, y1, self.deg)
            p2 = numpy.polyfit(x2, y2, self.deg)
        except:
            return 0

        if write:
            self.writeForGnuplot(p1, p2, x1, x2, y1, y2, L1, L2, sigma)

        result = self.findIntersection(lambda x : numpy.polyval(p1,x), lambda x : numpy.polyval(p2,x), (self.up + self.down)/2)
        return result

    def writeForGnuplot(self, p1, p2, x1, x2, y1, y2, L1, L2, sigma):
        directory = "../data/binderIntersect/"
        if not os.path.exists(directory):
            os.makedirs(directory)

        X = numpy.arange(self.down, self.up, (self.up - self.down)/500)
        f = open(os.path.join(directory,"tmpFit{0}_s_{1}.dat".format(L1,sigma)), "w")
        for x in X:
            f.write(" ".join([str(x), str(numpy.polyval(p1,x))]))
            f.write("\n")
        f.close()

        f = open(os.path.join(directory,"tmpFit{0}_s_{1}.dat".format(L2,sigma)), "w")
        for x in X:
            f.write(" ".join([str(x), str(numpy.polyval(p2,x))]))
            f.write("\n")
        f.close()

        f = open(os.path.join(directory,"tmp{0}_s_{1}.dat".format(L1,sigma)), "w")
        for [x,y] in zip(map(str,x1),map(str,y1)):
            f.write(" ".join([x, y]))
            f.write("\n")
        f.close()

        f = open(os.path.join(directory,"tmp{0}_s_{1}.dat".format(L2,sigma)), "w")
        for [x,y] in zip(map(str,x2),map(str,y2)):
            f.write(" ".join([x, y]))
            f.write("\n")
        f.close()

        f = open(os.path.join(directory,"plotAll_s_{0}.gp".format(sigma)), "w")
        f.write("set terminal png\n")
        f.write('set output "out_s_{0}.png"\n'.format(sigma))
        f.write('plot "tmpFit16_s_{0}.dat" u 1:2 w l lc rgb "red", "tmpFit32_s_{0}.dat" u 1:2 w l lc rgb "green", "tmpFit64_s_{0}.dat" u 1:2 w l lc rgb "blue", "tmp16_s_{0}.dat" u 1:2 w p lc rgb "red", "tmp32_s_{0}.dat" u 1:2 w p lc rgb "green", "tmp64_s_{0}.dat" u 1:2 w p lc rgb "blue"\n'.format(sigma))
        f.close()


if __name__ == "__main__":
    a = BinderIntersections()
    for [s, down, up] in zip(   (0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2),
                                (2.10, 2.10, 2.10, 1.50, 1.50, 1.20, 1.35,1.25,1.2, 1.2, 1.15,1.15,1.15, 1.15, 1.15, 1.15),
                                (2.40, 2.40, 2.30, 2.00, 1.70, 1.80, 1.5, 1.4, 1.4, 1.4, 1.4, 1.3, 1.3, 1.3, 1.3, 1.3)):
        print s, a.getIntersection(s, 16, 32, down=down, up=up)
        print s, a.getIntersection(s, 32, 64, down=down, up=up)
        print s, a.getIntersection(s, 64, 16, down=down, up=up)
