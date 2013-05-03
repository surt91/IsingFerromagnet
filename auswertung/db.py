#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import os
import sqlite3
import zlib
import logging

import array
from numpy import mean, var
from numpy import std
import numpy

from reader import *

logging.basicConfig(level=logging.INFO,
                format='%(asctime)s -- %(levelname)s :: %(message)s',
                datefmt='%d.%m.%YT%H:%M:%S')

class Database():
    def __init__(self, dbPath = "data.db", dataPath = "../data"):
        """! Constructor

            Erstellt eine SQLite Datenbank aus den Daten in dataPath, wenn
            sie noch nicht exisitert, ansonsten liest es sie und gibt
            Dateien zum Plotten wichtiger Messgrößen aus.
        """
        self.dbPath = dbPath

        if not os.path.isfile(self.dbPath):
            # Generate new Database in memory
            self.conn = sqlite3.connect(":memory:")
            self.createNewDatabase(dataPath)
            self.calculateNewDatabase()
            # Copy Database to file
            logging.info("copy Database to {0}".format(self.dbPath))
            new_db = sqlite3.connect(self.dbPath)
            self.conn.execute("""DROP TABLE rawdata""")
            self.conn.execute("""VACUUM""")
            query = "".join(line for line in self.conn.iterdump())
            new_db.executescript(query)
            logging.info("  copied")
            self.conn.close()

        # lade Datenbank und hole sie in den Speicher
        self.conn = sqlite3.connect(self.dbPath)

        self.writeBinderForGnuplot()
        self.writeMeanForGnuplot()
        self.writeVarForGnuplot()
        self.writeAutoForGnuplot()

        self.conn.close()

    @staticmethod
    def getVal(x):
        """! wandelt den gezippten String der Werte aus der Datenbank in
        ein Array zurück """
        a = array.array('d')
        a.fromstring(zlib.decompress(x))
        return a

    @staticmethod
    def setVal(x):
        """! wandelt ein Array mit Werten in einen gezppten String zum
        Speichern in der Datenbank um"""
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

    @staticmethod
    def getAutocorrTime(m):
        """! getAutocorrTime()

            Berechnet die Autokorrelationszeit \f$ \tau \f$
            durch Integration der Autokorrelationsfunktion

            Berechnung erfolgt per FFT und Faltung. O(nlog(n))
            vgl. http://stackoverflow.com/questions/643699/how-can-i-use-numpy-correlate-to-do-autocorrelation

            Die Berechnung erfolgt durch Integration (hier sollte eine
            Summation genau genug sein) der Autokorrelationsfunktion.
            vgl. \cite newman1999monte S. 62 (3.20)
        """
        auto = numpy.correlate(m, m, mode='full')[len(m)-1:]
        x0 = auto[0]
        tau = sum(auto)/x0
        return tau

    def writeForGnuplot(self, name, val, valErr):
        """! Sammelt die Werte die geplottet werden sollen und gibt sie
        an die Funktion weiter, die die Datein Schreibt """
        c = self.conn.cursor()
        for s in self.getDifferent("sigma"):
            if valErr:
                c.execute('SELECT {0},{1} FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC'.format(val, valErr), (s,))
            else:
                c.execute('SELECT {0} FROM calculated_data WHERE sigma = ? ORDER BY T ASC, L ASC'.format(val), (s,))
            x = c.fetchall()
            if valErr:
                dx = [i[1] for i in x]
            else:
                dx = [0 for i in x]
            x = [i[0] for i in x]

            self.writeFileForGnuplot(name+"_{0}".format(s)+".dat", x, dx)

    def writeBinderForGnuplot(self):
        self.writeForGnuplot("Binder", "binder", "binderErr")
    def writeMeanForGnuplot(self):
        self.writeForGnuplot("Mean_M", "meanM", "meanMErr")
        self.writeForGnuplot("Mean_E", "meanE", "meanEErr")
    def writeVarForGnuplot(self):
        self.writeForGnuplot("Var_M", "varM", "varMErr")
        self.writeForGnuplot("Var_E", "varE", "varEErr")
    def writeAutoForGnuplot(self):
        self.writeForGnuplot("Auto", "auto", None)

    def writeFileForGnuplot(self, name, x, dx):
        """! Schreibt die Daten- und Skriptdateien, die die Gnuplot plots
        erstellen """
        directory = "../data/out/"
        if not os.path.exists(directory):
            os.makedirs(directory)

        Ls = sorted(self.getDifferent("L"))
        Ts = sorted(self.getDifferent("T"))
        numL = len(Ls)
        numT = len(Ts)

        f = open(os.path.join(directory,name), "w")
        f.write("# Je drei Spalten beschreiben ein L: Temperatur, Wert, Fehler\n")
        f.write("# L: ")
        for l in Ls:
            f.write(" {0}".format(l))
        f.write("\n")

        # Zeilen
        for i in range(numT):
            f.write("{0}".format(Ts[i]))
            # Spalten
            for j in range(numL):
                f.write(" {0}".format(x[j+i*numL]))
                f.write(" {0}  ".format(dx[j+i*numL]))
            f.write("\n")
        f.close()
        f = open(os.path.join(directory,name.replace(".dat",".gp")), "w")
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
        """! Liest die Datendatein aus und sortiert ihre Inhalte in die
        Datenbank ein """
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
        self.conn.execute("""CREATE INDEX idx_ex1 ON rawdata(sigma,L,T)""")

    def calculateNewDatabase(self):
        """! Liest die Rohdaten aus der Datenbank und berechnet daraus
        die Mittelwerte der Erwartungswerte """
        logging.info("calculating")
        self.conn.execute("""CREATE TABLE calculated_data
            (sigma real, L integer, T real, binder real, binderErr real, meanM real, meanMErr real, meanE real, meanEErr real, varM real, varMErr real, varE real, varEErr real, auto real)""")

        sigmas = self.getDifferent("sigma")
        Ls = self.getDifferent("L")
        Ts = self.getDifferent("T")
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
                    auto = self.getAverageM(self.getAutocorrTime, s, L, T, signed=True)[0]
                    rows.append((s, L, T, binder, binderErr, mM, mMErr, mE, mEErr, vM, vMErr, vE, vEErr, auto))
        self.conn.executemany('INSERT INTO calculated_data VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)', rows)
        self.conn.commit()

    def getDifferent(self, val):
        """! Gibt eine Liste aus, in der alle vorkommenden Werte der
        angefragten Spalte vorkommen """
        c = self.conn.cursor()
        try:
            c.execute('SELECT DISTINCT {0} FROM rawdata'.format(val))
        except :
            c.execute('SELECT DISTINCT {0} FROM calculated_data'.format(val))
        x = c.fetchall()
        x = [i[0] for i in x]
        return x

    def getAverageM(self, f, sigma, L, T, signed = False):
        """! Liefert den über verschiedene Realisierungen gemittelten
        Erwartungswert von f(|M|) aus: mean(<f(|M|)>) """
        c = self.conn.cursor()
        c.execute('SELECT M FROM rawdata WHERE sigma = ? AND L = ? AND T = ?', (sigma, L, T))
        lst = c.fetchall()
        c.close()
        # Berechne die Erwartungswerte
        if signed:
            M = [f(list(self.getVal(i[0]))) for i in lst]
        else:
            M = [f(list(map(abs,self.getVal(i[0])))) for i in lst]

        # berechne den Mittelwert der Erwartungswerte
        return mean(M), std(M)
    def getAverageE(self, f, sigma, L, T):
        """! Liefert den über verschiedene Realisierungen gemittelten
        Erwartungswert von f(E) aus: mean(<f(E)>) """
        c = self.conn.cursor()
        c.execute('SELECT E FROM rawdata WHERE sigma = ? AND L = ? AND T = ?', (sigma, L, T))
        E = [f(self.getVal(i[0])) for i in c.fetchall()]
        c.close()
        return mean(E), std(E)

if __name__ == '__main__':
    a=Database()
