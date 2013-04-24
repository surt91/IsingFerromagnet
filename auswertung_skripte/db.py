#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import os
import sqlite3
import zlib

import array
from numpy import mean, var

from reader import *

class Database():
    def __init__(self, dbPath = "data.db", dataPath = "../data"):
        self.dbPath = dbPath

        if not os.path.isfile(self.dbPath):
            # Generate new Database in memory
            self.conn = sqlite3.connect(":memory:")
            self.createNewDatabase(dataPath)
            self.calculateNewDatabase()
            # Copy Database to file
            print "copy Database to {0}".format(self.dbPath)
            new_db = sqlite3.connect(self.dbPath)
            query = "".join(line for line in self.conn.iterdump() if "calculated_data" in line)
            new_db.executescript(query)
            print "  copied"
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
            c.execute('SELECT binder FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"_{0}".format(s)+".dat", x)

    def writeMeanForGnuplot(self, name):
        c = self.conn.cursor()
        for s in self.getSigmas():
            c.execute('SELECT meanM FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"M_{0}".format(s)+".dat", x)
        for s in self.getSigmas():
            c.execute('SELECT meanE FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"E_{0}".format(s)+".dat", x)
    def writeVarForGnuplot(self, name):
        c = self.conn.cursor()
        for s in self.getSigmas():
            c.execute('SELECT varM FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"M_{0}".format(s)+".dat", x)
        for s in self.getSigmas():
            c.execute('SELECT varE FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC', (s,))
            x = c.fetchall()
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"E_{0}".format(s)+".dat", x)

    def writeFileForGnuplot(self, name, x):
        Ls = sorted(list(set(self.getLs())))
        Ts = sorted(list(set(self.getTs())))
        numL = len(set(Ls))
        numT = len(set(Ts))

        f = open("data/"+name, "w")
        f.write("# L: ")
        for l in Ls:
            f.write(" {0}".format(l))

        # Zeilen
        for i in range(numT):
            f.write("\n{0}".format(Ts[i]))
            # Spalten
            for j in range(numL):
                f.write(" {0}".format(x[j+i*numL]))

    def createNewDatabase(self, dataPath):
        print("create new Database: '{0}'".format(self.dbPath))

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
        print("calculating")
        self.conn.execute("""CREATE TABLE calculated_data
            (sigma real, L integer, T real, binder real, meanM real, meanE real, varM real, varE real)""")

        sigmas = self.getSigmas()
        Ls = self.getLs()
        Ts = self.getTs()
        rows=[]
        for s in sigmas:
            print "sigma: ", s
            for L in Ls:
                print "L: ", L
                for T in Ts:
                    binder = self.getAverageM(self.getBinder, s, L, T)
                    mM = self.getAverageM(mean, s, L, T)
                    mE = self.getAverageE(mean, s, L, T)
                    vM = self.getAverageM(var, s, L, T)
                    vE = self.getAverageE(var, s, L, T)
                    rows.append((s, L, T, binder, mM, mE, vM, vE))
        self.conn.executemany('INSERT INTO calculated_data VALUES (?,?,?,?,?,?,?,?)', rows)
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
        return mean([self.getExpectationM(f, sigma, L, T, x) for x in self.getXs()])
    def getAverageE(self, f, sigma, L, T):
        return mean([self.getExpectationE(f, sigma, L, T, x) for x in self.getXs()])

if __name__ == '__main__':
    a=Database()
