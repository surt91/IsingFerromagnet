#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import sqlite3
from scipy.optimize import fsolve
import numpy
from subprocess import call
import os
import logging
from itertools import combinations
from numpy import mean, std

import warnings
warnings.simplefilter('ignore', numpy.RankWarning)
numpy.seterr(all='ignore')

logging.basicConfig(level=logging.INFO,
                format='%(asctime)s -- %(levelname)s :: %(message)s',
                datefmt='%d.%m.%YT%H:%M:%S')

class BinderIntersections():

    def __init__(self, directory, database="data.db", deg=4 ):
        self.deg = deg
        self.directory = directory
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
            p1 = numpy.polyfit(x1, y1, self.deg, w=dy1)
            p2 = numpy.polyfit(x2, y2, self.deg, w=dy2)
        except TypeError: # Numpy Version ist zu alt, fitte ohne Gewichte
            p1 = numpy.polyfit(x1, y1, self.deg)
            p2 = numpy.polyfit(x2, y2, self.deg)
        except:
            logging.warning("fitting was not successful")
            return 0

        if write:
            self.writeForGnuplot(p1, p2, x1, x2, y1, y2, dy1, dy2, L1, L2, sigma)

        x = self.findIntersection(lambda z : numpy.polyval(p1,z), lambda z : numpy.polyval(p2,z), (self.up + self.down)/2)
        y = numpy.polyval(p1,x)
        return x,y

    def writeForGnuplot(self, p1, p2, x1, x2, y1, y2, dy1, dy2, L1, L2, sigma):
        X = numpy.arange(self.down, self.up, (self.up - self.down)/500)
        with open(os.path.join(self.directory,"tmpFit{0}_s_{1:.3f}.dat".format(L1,sigma)), "w") as f:
            for x in X:
                f.write(" ".join([str(x), str(numpy.polyval(p1,x))]))
                f.write("\n")

        with open(os.path.join(self.directory,"tmpFit{0}_s_{1:.3f}.dat".format(L2,sigma)), "w") as f:
            for x in X:
                f.write(" ".join([str(x), str(numpy.polyval(p2,x))]))
                f.write("\n")

        with open(os.path.join(self.directory,"tmp{0}_s_{1:.3f}.dat".format(L1,sigma)), "w") as f:
            for [x,y,dy] in zip(map(str,x1), map(str,y1), map(str,dy1)):
                f.write(" ".join([x, y, dy]))
                f.write("\n")

        with open(os.path.join(self.directory,"tmp{0}_s_{1:.3f}.dat".format(L2,sigma)), "w") as f:
            for [x,y,dy] in zip(map(str,x2), map(str,y2), map(str,dy2)):
                f.write(" ".join([x, y, dy]))
                f.write("\n")

        with open(os.path.join(self.directory,"plotAll_s_{0:.3f}.gp".format(sigma)), "w") as f:
            f.write("set terminal png\n")
            f.write('set output "out_s_{0:.3f}.png"\n'.format(sigma))
            f.write('plot "tmpFit16_s_{0:.3f}.dat" u 1:2 w l lc rgb "red" title "Fit: L = 16", \
                          "tmpFit32_s_{0:.3f}.dat" u 1:2 w l lc rgb "green" title "Fit: L = 32", \
                          "tmpFit64_s_{0:.3f}.dat" u 1:2 w l lc rgb "blue" title "Fit: L = 64", \
                          "tmpFit128_s_{0:.3f}.dat" u 1:2 w l lc rgb "violet" title "Fit: L = 128", \
                          "tmp16_s_{0:.3f}.dat" u 1:2:3 w ye lc rgb "red" title "L = 16", \
                          "tmp32_s_{0:.3f}.dat" u 1:2:3 w ye lc rgb "green" title "L = 32", \
                          "tmp64_s_{0:.3f}.dat" u 1:2:3 w ye lc rgb "blue" title "L = 64", \
                          "tmp128_s_{0:.3f}.dat" u 1:2:3 w ye lc rgb "violet" title "L = 128"\n'.format(sigma))
            f.write("set xlabel 'Temperatur'\n")
            f.write("set ylabel 'Binder'\n")
            f.write("set key right\n")

if __name__ == "__main__":

    def writeAndPlot(graphType):
        sizes = [16,32,64,128]

        logging.info("possible intersections: " + str([x for x in combinations(sizes, 2)]))
        sigmas    = (0.00, 0.03, 0.05, 0.08, 0.10, 0.12, 0.15, 0.20, 0.25, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00, 1.10, 1.20)

        x = {}
        y = {}

        if graphType == "GG":
            directory = "../dataGG/binderIntersect/"
            db = "dataGG.db"
            downs   = (2.25, 2.80, 2.80, 2.80, 2.85, 2.85, 2.85, 2.80, 2.60, 2.48, 2.25, 2.20, 2.15, 2.12, 2.10, 2.08, 2.08, 2.08, 2.08)
            ups     = (2.30, 2.94, 2.95, 2.95, 2.95, 3.00, 3.00, 2.90, 2.75, 2.60, 2.40, 2.30, 2.25, 2.22, 2.22, 2.20, 2.20, 2.20, 2.20)

        else:
            directory = "../dataRNG/binderIntersect/"
            db = "dataRNG.db"
            downs  = (2.25, 2.25, 2.25, 2.23, 2.18, 2.07, 1.85, 1.60, 1.45, 1.38, 1.30, 1.24, 1.22, 1.20, 1.18, 1.17, 1.18, 1.18, 1.18)
            ups    = (2.30, 2.30, 2.30, 2.30, 2.25, 2.15, 1.95, 1.68, 1.53, 1.45, 1.38, 1.32, 1.30, 1.28, 1.26, 1.26, 1.24, 1.24, 1.24)


        if not os.path.exists(directory):
            os.makedirs(directory)
        a = BinderIntersections(directory, db)
        for [s, down, up] in zip(sigmas, downs, ups):
            tmpIntersects = [a.getIntersection(s, L1, L2, down=down, up=up) for [L1,L2] in combinations(sizes, 2)]
            x.update({s:[i[0] for i in tmpIntersects]})
            y.update({s:[i[1] for i in tmpIntersects]})

        with open(os.path.join(directory,"Tc.dat"), "w") as f:
            for s in sigmas:
                f.write("{0} {1} {2}\n".format(s, mean(x[s]), std(x[s])))
        with open(os.path.join(directory,"TcG.dat"), "w") as f:
            for s in sigmas:
                f.write("{0} {1} {2}\n".format(s, mean(y[s]), std(y[s])))

        with open(os.path.join(directory,"plotTc.gp"), "w") as f:
            f.write("set terminal png\n")
            f.write('set output "Tc.png"\n')
            f.write("plot 'Tc.dat' u 1:2:3 w ye \n")
            f.write("set xlabel 'sigma'\n")
            f.write("set ylabel 'T'\n")
            f.write("set key right\n")

        with open(os.path.join(directory,"plotTcG.gp"), "w") as f:
            f.write("set terminal png\n")
            f.write('set output "TcG.png"\n')
            f.write("plot 'TcG.dat' u 1:2:3 w ye\n")
            f.write("set xlabel 'sigma'\n")
            f.write("set ylabel 'g(T)'\n")
            f.write("set key right\n")

    writeAndPlot("GG")
    writeAndPlot("RNG")
