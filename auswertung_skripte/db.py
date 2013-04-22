#!/usr/bin/env python2
# -*- coding: UTF-8 -*-

import os
import sqlite3
import zlib

import array
from numpy import mean

from reader import *

class Database():
    def __init__(self, dbPath = "data.db", dataPath = "../data"):
        self.dbPath = dbPath

        if not os.path.isfile(self.dbPath):
            self.createNewDatabase(dataPath)

        self.conn = sqlite3.connect(self.dbPath)

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

    def createNewDatabase(self, dataPath):
        print("create new Database: '{0}'".format(self.dbPath))

        conn = sqlite3.connect(self.dbPath)
        conn.execute("""CREATE TABLE rawdata
            (n blob, sigma real, L integer, x integer, T real, M blob, E blob)""")


        for f in os.listdir(dataPath):
            if not ".dat" in f:
                continue

            r = output_reader(os.path.join(dataPath,f))
            t = [(self.setVal(r.N), r.sigma, r.L, r.x, r.T[i], self.setVal(r.M[i]), self.setVal(r.E[i])) for i in range(len(r.T))]
            conn.executemany('INSERT INTO rawdata VALUES (?,?,?,?,?,?,?)', t)
        conn.commit()
        conn. close()

    def addRows(self, dataLst):
        """dataLst ist eine liste von Datentupeln. bsp:
         [(r.N, r.sigma, r.L, r.x, r.T[i], r.M[i], r.E[i]),
          (r.N, r.sigma, r.L, r.x, r.T[i], r.M[i], r.E[i])]
        """
        self.conn.executemany('INSERT INTO rawdata VALUES (?,?,?,?,?,?,?)', dataLst)
        self.conn.commit()

    def getXs(self):
        c = self.conn.cursor()
        c.execute('SELECT x FROM rawdata')
        x = c.fetchall()
        x = (i[0] for i in x)
        return list(set(x))
    def getTs(self):
        c = self.conn.cursor()
        c.execute('SELECT T FROM rawdata')
        x = c.fetchall()
        x = (i[0] for i in x)
        return list(set(x))
    def getLs(self):
        c = self.conn.cursor()
        c.execute('SELECT L FROM rawdata')
        x = c.fetchall()
        x = (i[0] for i in x)
        return list(set(x))
    def getSigmas(self):
        c = self.conn.cursor()
        c.execute('SELECT sigma FROM rawdata')
        x = c.fetchall()
        x = (i[0] for i in x)
        return list(set(x))

    def getMForSigmaLT(self, sigma, L, T, x):
        c = self.conn.cursor()
        c.execute('SELECT M FROM rawdata WHERE sigma = ? AND L = ? AND T = ? AND x = ?', (sigma, L, T, x))
        M = self.getVal(c.fetchone()[0])
        c.close()
        return M
    def getEForSigmaLT(self, sigma, L, T, x):
        c = self.conn.cursor()
        c.execute('SELECT E FROM rawdata WHERE sigma = ? AND L = ? AND T = ? AND x = ?', (sigma, L, T, x))
        E = self.getVal(c.fetchone()[0])
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
