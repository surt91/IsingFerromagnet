#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import os
import sqlite3
import zlib
import logging

import array
from numpy import mean, var
from numpy import std

from reader import *

logging.basicConfig(level=logging.INFO,
                format='%(asctime)s -- %(levelname)s :: %(message)s',
                datefmt='%d.%m.%YT%H:%M:%S')

class Database():
    def __init__(self, dbPath = "data.db", dataPath = "../data"):
        self.dbPath = dbPath

        if not os.path.isfile(self.dbPath):
            # Generate new Database in memory
            self.conn = sqlite3.connect(":memory:")
            self.createNewDatabase(dataPath)
            self.calculateNewDatabase()
            # Copy Database to file
            logging.info("copy Database to {0}".format(self.dbPath))
            new_db = sqlite3.connect(self.dbPath)
            query = "".join(line for line in self.conn.iterdump() if "calculated_data" in line)
            new_db.executescript(query)
            logging.info("  copied")
            self.conn.close()

        # lade Datenbank und hole sie in den Speicher
        self.conn = sqlite3.connect(self.dbPath)

        self.writeBinderForGnuplot("Binder")
        self.writeMeanForGnuplot("Mean")
        self.writeVarForGnuplot("Var")

        self.conn.close()

    @staticmethod
    def getVal(x):
        a = array.array('d')
        a.fromstring(zlib.decompress(x))
        return a

    @staticmethod
    def setVal(x):
        a = array.array('d')
        a.extend(x)
        return buffer(zlib.compress(a.tostring()))

    @staticmethod
    def getBinder(m):
        """! Berechnet die Binder Kumulante

            vgl. \cite katzgraber2011introduction S. 12 (19)
        """
        m4 = mean([i**4 for i in m])
        m2 = mean([i**2 for i in m])

        g=(3-m4/(m2**2))/2

        return g

    def writeBinderForGnuplot(self, name):
        c = self.conn.cursor()
        for s in self.getSigmas():
            c.execute('SELECT binder,binderErr FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            dx = [i[1] for i in x]
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"_{0}".format(s)+".dat", x, dx)

    def writeMeanForGnuplot(self, name):
        c = self.conn.cursor()
        for s in self.getSigmas():
            c.execute('SELECT meanM,meanMErr FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            dx = [i[1] for i in x]
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"_M_{0}".format(s)+".dat", x, dx)
        for s in self.getSigmas():
            c.execute('SELECT meanE,meanEErr FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            dx = [i[1] for i in x]
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"_E_{0}".format(s)+".dat", x, dx)
    def writeVarForGnuplot(self, name):
        c = self.conn.cursor()
        for s in self.getSigmas():
            c.execute('SELECT varM,varMErr FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            dx = [i[1] for i in x]
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"_M_{0}".format(s)+".dat", x, dx)
        for s in self.getSigmas():
            c.execute('SELECT varE,varEErr FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            dx = [i[1] for i in x]
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"_E_{0}".format(s)+".dat", x, dx)

    def writeFileForGnuplot(self, name, x, dx):
        Ls = sorted(list(set(self.getLs())))
        Ts = sorted(list(set(self.getTs())))
        numL = len(set(Ls))
        numT = len(set(Ts))

        f = open("../data/"+name, "w")
        f.write("# In der ersten Spalte steht die Temperatur in den Spalten daneben stehen Werte und Fehlerwerte für jeweils ein L.")
        f.write("# L: ")
        for l in Ls:
            f.write(" {0}".format(l))

        # Zeilen
        for i in range(numT):
            f.write("\n{0}  ".format(Ts[i]))
            # Spalten
            for j in range(numL):
                f.write(" {0}".format(x[j+i*numL]))
                f.write(" {0}  ".format(dx[j+i*numL]))
        f.close()
        f = open("../data/"+name.replace(".dat",".gp"), "w")
        f.write("set terminal png\n")
        f.write("set output '{0}'\n".format(name.replace(".dat",".png")))
        f.write("set xlabel 'Temperatur'\n")
        f.write("set ylabel '{0}'\n".format(name.partition("_")[0]))
        f.write("set key right\n")

        f.write("plot ")
        tmpStr = ""
        for [i,l] in enumerate(Ls):
            tmpStr+=("'{0}' using 1:{1}:{2} w yerrorbar title {3}, ".format(name, 2*i+2, 2*i+3, "'L = {0}'".format(l)))
        f.write(tmpStr[:-2])
        f.close()

    def createNewDatabase(self, dataPath):
        logging.info("create new Database: '{0}'".format(self.dbPath))

        self.conn.execute("""CREATE TABLE rawdata
            (n blob, sigma real, L integer, x integer, T real, M blob, E blob)""")

        for f in os.listdir(dataPath):
            if not ".dat" in f:
                continue

            r = output_reader(os.path.join(dataPath,f))
            t = [(self.setVal(r.N), r.sigma, r.L, r.x, r.T[i], self.setVal(r.M[i]), self.setVal(r.E[i])) for i in range(len(r.T))]
            self.conn.executemany('INSERT INTO rawdata VALUES (?,?,?,?,?,?,?)', t)
            self.conn.commit()

    def calculateNewDatabase(self):
        logging.info("calculating")
        self.conn.execute("""CREATE TABLE calculated_data
            (sigma real, L integer, T real, binder real, binderErr real, meanM real, meanMErr real, meanE real, meanEErr real, varM real, varMErr real, varE real, varEErr real)""")

        sigmas = self.getSigmas()
        Ls = self.getLs()
        Ts = self.getTs()
        rows=[]
        for s in sigmas:
            logging.info("sigma: %f" % s)
            for L in Ls:
                logging.info("L: %d" % L)
                for T in Ts:
                    # Ist es korrekt hier einfach den Standardfehler des Mittelwerts zu nehmen?
                    binder, binderErr = self.getAverageM(self.getBinder, s, L, T)
                    mM, mMErr = self.getAverageM(mean, s, L, T)
                    mE, mEErr = self.getAverageE(mean, s, L, T)
                    vM, vMErr = self.getAverageM(var, s, L, T)
                    vE, vEErr = self.getAverageE(var, s, L, T)
                    rows.append((s, L, T, binder, binderErr, mM, mMErr, mE, mEErr, vM, vMErr, vE, vEErr))
        self.conn.executemany('INSERT INTO calculated_data VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)', rows)
        self.conn.commit()

    def getXs(self):
        c = self.conn.cursor()
        try:
            c.execute('SELECT x FROM rawdata')
        except :
            c.execute('SELECT x FROM calculated_data')
        x = c.fetchall()
        x = (i[0] for i in x)
        return list(set(x))
    def getTs(self):
        c = self.conn.cursor()
        try:
            c.execute('SELECT T FROM rawdata')
        except :
            c.execute('SELECT T FROM calculated_data')
        x = c.fetchall()
        x = (i[0] for i in x)
        return list(set(x))
    def getLs(self):
        c = self.conn.cursor()
        try:
            c.execute('SELECT L FROM rawdata')
        except :
            c.execute('SELECT L FROM calculated_data')
        x = c.fetchall()
        x = (i[0] for i in x)
        return list(set(x))
    def getSigmas(self):
        c = self.conn.cursor()
        try:
            c.execute('SELECT sigma FROM rawdata')
        except :
            c.execute('SELECT sigma FROM calculated_data')
        x = c.fetchall()
        x = (i[0] for i in x)
        return list(set(x))

    def getMForSigmaLT(self, sigma, L, T, x):
        c = self.conn.cursor()
        c.execute('SELECT M FROM rawdata WHERE sigma = ? AND L = ? AND T = ? AND x = ?', (sigma, L, T, x))
        try:
            M = self.getVal(c.fetchone()[0])
        except TypeError:
            M = [0]
        c.close()
        return M
    def getEForSigmaLT(self, sigma, L, T, x):
        c = self.conn.cursor()
        c.execute('SELECT E FROM rawdata WHERE sigma = ? AND L = ? AND T = ? AND x = ?', (sigma, L, T, x))
        try:
            E = self.getVal(c.fetchone()[0])
        except TypeError:
            E = [0]
        c.close()
        return E

    def getExpectationM(self, f, sigma, L, T, x):
        return f(list(map(abs,self.getMForSigmaLT(sigma, L, T, x))))
    def getExpectationE(self, f, sigma, L, T, x):
        return f(self.getEForSigmaLT(sigma, L, T, x))

    def getAverageM(self, f, sigma, L, T):
        lst = [self.getExpectationM(f, sigma, L, T, x) for x in self.getXs()]
        return mean(lst), std(lst)
    def getAverageE(self, f, sigma, L, T):
        lst = [self.getExpectationE(f, sigma, L, T, x) for x in self.getXs()]
        return mean(lst), std(lst)

if __name__ == '__main__':
    a=Database()
