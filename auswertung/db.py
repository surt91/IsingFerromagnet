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

from random import choice

from reader import *

logging.basicConfig(level=logging.INFO,
                format='%(asctime)s -- %(levelname)s :: %(message)s',
                datefmt='%d.%m.%YT%H:%M:%S')

class Database():
    def __init__(self, dbPath="data.db", dbRawPath="dataRaw.db", dataPath="../data", graphType=1):
        """! Constructor

            Erstellt eine SQLite Datenbank aus den Daten in dataPath, wenn
            sie noch nicht exisitert, ansonsten liest es sie und gibt
            Dateien zum Plotten wichtiger Messgrößen aus.
        """
        self.dbPath = dbPath
        self.dbRawPath = dbRawPath
        self.graphType = graphType

        if not os.path.isfile(self.dbPath):
            try:
                os.remove(self.dbRawPath)
            except: pass
            # Generate new Raw Database
            self.connRaw = sqlite3.connect(self.dbRawPath)
            self.createNewDatabase(dataPath)

            # Generate new Database
            self.conn = sqlite3.connect(self.dbPath)
            self.calculateNewDatabase()

            # Cleanup Databse
            logging.info("  vacuum")
            self.conn.execute("""VACUUM""")

            # Close Databases
            self.connRaw.close()
            self.conn.close()

        self.conn = sqlite3.connect(self.dbPath)

        logging.info("start writing files for Gnuplot")
        logging.info("  binder")
        self.writeBinderForGnuplot()
        logging.info("  Mittelwerte E,M")
        self.writeMeanForGnuplot()
        logging.info("  Wärmekapazität, Suzeptibilität")
        self.writeVarForGnuplot()
        logging.info("  Autokorrelationszeiten")
        self.writeAutoForGnuplot()
        logging.info("  Parallel Tempering Akzeptanzraten")
        self.writeParTempForGnuplot()
        logging.info("  finished")

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
    def getMean(x,T,L):
        """! Berechnet den Mittelwert
        """
        return mean(x)

    @staticmethod
    def getSpecificHeat(e,T,L):
        """! Berechnet die spezifische Wärme

            vgl. \cite newman1999monte S. 59 (3.15)
            abgewandelt mit der Eigenschaft: var(n*e) = n^2*var(e)
        """
        N = L*L
        c = var(e)*N/T**2
        return c

    @staticmethod
    def getSusceptibility(m,T,L):
        """! Berechnet die Suszeptibilität

            vgl. \cite newman1999monte S. 59 (3.16)
        """
        N = L*L
        chi = var(m)/T*N
        return chi

    @staticmethod
    def getBinder(m,T,L):
        """! Berechnet die Binder Kumulante

            vgl. \cite katzgraber2011introduction S. 12 (19)
        """
        m4 = mean([i**4 for i in m])
        m2 = mean([i**2 for i in m])

        g=(3-m4/(m2**2))/2

        return g

    @staticmethod
    def getAutocorrTime(m,T,L):
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

    @staticmethod
    def bootstrap_variance(x, n_resample, f):
        N = range(len(x))
        h = [f([choice(x) for _ in N]) for _ in range(n_sample)]
        return var(h)

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

    def writeForGnuplot2(self, name, val, valErr):
        """! Sammelt die Werte die geplottet werden sollen und gibt sie
        an die Funktion weiter, die die Datein Schreibt """
        c = self.conn.cursor()
        for l in self.getDifferent("L"):
            if valErr:
                c.execute('SELECT {0},{1} FROM calculated_data WHERE L = ? ORDER BY T ASC, sigma ASC'.format(val, valErr), (l,))
            else:
                c.execute('SELECT {0} FROM calculated_data WHERE L = ? ORDER BY T ASC, sigma ASC'.format(val), (l,))
            x = c.fetchall()
            if valErr:
                dx = [i[1] for i in x]
            else:
                dx = [0 for i in x]
            x = [i[0] for i in x]

            self.writeFileForGnuplot2(name+"_L_{0}".format(l)+".dat", x, dx)

    def writeBinderForGnuplot(self):
        self.writeForGnuplot("Binder", "binder", "binderErr")
        self.writeForGnuplot2("Binder", "binder", "binderErr")
    def writeMeanForGnuplot(self):
        self.writeForGnuplot("Mean_M", "meanM", "meanMErr")
        self.writeForGnuplot("Mean_E", "meanE", "meanEErr")
        self.writeForGnuplot2("Mean_M", "meanM", "meanMErr")
        self.writeForGnuplot2("Mean_E", "meanE", "meanEErr")
    def writeVarForGnuplot(self):
        self.writeForGnuplot("Susceptibility", "varM", "varMErr")
        self.writeForGnuplot("Specific_Heat", "varE", "varEErr")
        self.writeForGnuplot2("Susceptibility", "varM", "varMErr")
        self.writeForGnuplot2("Specific_Heat", "varE", "varEErr")
    def writeAutoForGnuplot(self):
        self.writeForGnuplot("Autokorrelationszeit", "auto", None)
        self.writeForGnuplot2("Autokorrelationszeit", "auto", None)
    def writeParTempForGnuplot(self):
        self.writeForGnuplot("Akzeptanz", "A", None)
        self.writeForGnuplot2("Akzeptanz", "A", None)

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

    def writeFileForGnuplot2(self, name, x, dx):
        """! Schreibt die Daten- und Skriptdateien, die die Gnuplot plots
        erstellen """
        directory = "../data/out/"
        if not os.path.exists(directory):
            os.makedirs(directory)

        Ss = sorted(self.getDifferent("sigma"))
        Ts = sorted(self.getDifferent("T"))
        numS = len(Ss)
        numT = len(Ts)

        f = open(os.path.join(directory,name), "w")
        f.write("# Je drei Spalten beschreiben ein Sigma: Temperatur, Wert, Fehler\n")
        f.write("# Sigma: ")
        for s in Ss:
            f.write(" {0}".format(s))
        f.write("\n")

        # Zeilen
        for i in range(numT):
            f.write("{0}".format(Ts[i]))
            # Spalten
            for j in range(numS):
                f.write(" {0}".format(x[j+i*numS]))
                f.write(" {0}  ".format(dx[j+i*numS]))
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
        for [i,s] in enumerate(Ss):
            tmpStr+=("'{0}' using 1:{1}:{2} w yerrorbar title {3}, ".format(name, 2*i+2, 2*i+3, "'s = {0}'".format(s)))
        f.write(tmpStr[:-2])
        f.close()

    def createNewDatabase(self, dataPath):
        """! Liest die Datendatein aus und sortiert ihre Inhalte in die
        Datenbank ein """
        logging.info("create new Database: '{0}'".format(self.dbPath))

        self.connRaw.execute("""CREATE TABLE rawdata
            (n blob, sigma real, L integer, x integer, T real, M blob, E blob, A real)""")
        for f in sorted(os.listdir(dataPath)):
            if not ".dat" in f:
                continue

            r = output_reader(os.path.join(dataPath,f))
            if int(r.graphType) != int(self.graphType):
                continue
            t = [(self.setVal(r.N), r.sigma, r.L, r.x, r.T[i], self.setVal(r.M[i]), self.setVal(r.E[i]), r.A[i]) for i in range(len(r.T))]
            self.connRaw.executemany('INSERT INTO rawdata VALUES (?,?,?,?,?,?,?,?)', t)
        self.connRaw.commit()
        self.connRaw.execute("""CREATE INDEX idx_ex1 ON rawdata(sigma,L,T)""")

    def calculateNewDatabase(self):
        """! Liest die Rohdaten aus der Datenbank und berechnet daraus
        die Mittelwerte der Erwartungswerte """
        logging.info("calculating")
        self.conn.execute("""CREATE TABLE calculated_data
            (sigma real, L integer, T real, binder real, binderErr real, meanM real, meanMErr real, meanE real, meanEErr real, varM real, varMErr real, varE real, varEErr real, auto real, A real)""")

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
                    mM, mMErr = self.getAverageM(self.getMean, s, L, T)
                    mE, mEErr = self.getAverageE(self.getMean, s, L, T)
                    vM, vMErr = self.getAverageM(self.getSusceptibility, s, L, T)
                    vE, vEErr = self.getAverageE(self.getSpecificHeat, s, L, T)
                    auto = self.getAverageM(self.getAutocorrTime, s, L, T, signed=True)[0]
                    A = self.getAverageA(s, L, T)
                    rows.append((s, L, T, binder, binderErr, mM, mMErr, mE, mEErr, vM, vMErr, vE, vEErr, auto, A))
        self.conn.executemany('INSERT INTO calculated_data VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)', rows)
        self.conn.commit()

    def getDifferent(self, val):
        """! Gibt eine Liste aus, in der alle vorkommenden Werte der
        angefragten Spalte vorkommen """
        try:
            c = self.connRaw.cursor()
            c.execute('SELECT DISTINCT {0} FROM rawdata'.format(val))
        except :
            c = self.conn.cursor()
            c.execute('SELECT DISTINCT {0} FROM calculated_data'.format(val))
        x = c.fetchall()
        x = [i[0] for i in x]
        return x

    def getAverageA(self, sigma, L, T):
        """! Liefert die über verschiedene Realisierungen gemittelten
        Parallel Tempering Akzeptanzraten """
        c = self.connRaw.cursor()
        c.execute('SELECT A FROM rawdata WHERE sigma = ? AND L = ? AND T = ?', (sigma, L, T))
        A = [i[0] for i in c.fetchall()]
        c.close()
        return mean(A)

    def getAverageM(self, f, sigma, L, T, signed = False):
        """! Liefert den über verschiedene Realisierungen gemittelten
        Erwartungswert von f(|M|) aus: mean(<f(|M|)>) """
        c = self.connRaw.cursor()
        c.execute('SELECT M FROM rawdata WHERE sigma = ? AND L = ? AND T = ?', (sigma, L, T))
        lst = c.fetchall()
        c.close()
        # Berechne die Erwartungswerte
        if signed:
            M = [f(list(self.getVal(i[0])),T,L) for i in lst]
        else:
            M = [f(list(map(abs,self.getVal(i[0]))),T,L) for i in lst]

        # berechne den Mittelwert der Erwartungswerte
        return mean(M), std(M)

    def getAverageE(self, f, sigma, L, T):
        """! Liefert den über verschiedene Realisierungen gemittelten
        Erwartungswert von f(E) aus: mean(<f(E)>) """
        c = self.connRaw.cursor()
        c.execute('SELECT E FROM rawdata WHERE sigma = ? AND L = ? AND T = ?', (sigma, L, T))
        E = [f(self.getVal(i[0]),T,L) for i in c.fetchall()]
        c.close()
        return mean(E), std(E)

if __name__ == '__main__':
    a=Database( dbPath = "dataRNG.db", dataPath = "../data", graphType=1)
    #~ b=Database( dbPath = "dataGG.db", dataPath = "../data", graphType=2)
