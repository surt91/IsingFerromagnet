#! /usr/bin/env python
# -*- coding: utf-8 -*- 

import csv
from numpy import mean, var

from scipy.integrate import trapz

class output_reader():
    """! 
        Diese Klasse kümmert sich um das Auslesen einzelner Daten Dateien.

        Dabei werden Funktionen definiert, die
        
            - Die Autokorrelationszeit \f$ \tau \f$ \cite newman1999monte S. 62 (3.20)
            - Die Binder Cumulante g \cite katzgraber2011introduction S. 12 (19)
            - Den Mittelwert und die Varianz
            - Die Fehler per Bootstrap/Jackknife
            
        berechnen
    """

    def __init__(self, filename):
        with open(filename, 'rb') as csvfile:
            print "reading: ", filename
            reader = csv.reader(csvfile, delimiter=' ')
            header = reader.next()

            self.T = header[0].rpartition("T=")[2]

            alle = [[int(i[0]), float(i[1]), float(i[2])] for i in reader]

            self.N = [i[0] for i in alle]
            self.E = [i[1] for i in alle]
            self.M = [i[2] for i in alle]
            
    def X(self, t):
        m = self.M
        t_max = len(m)
        return sum([m[i] * m[i+t] for i in range(0,t_max-t)])
        
    def getAutocorrTime(self):
        m = self.M
        x0 = self.X(0)
        autocorr = [self.X(t) for t in range(0,len(m))]
        self.tau = trapz(autocorr)/x0
        
        return self.tau
        
    def getBinder(self):
        m = self.M
        m4 = mean([abs(i**4) for i in m])
        m2 = mean([abs(i**2) for i in m])

        self.g=(3-m4/(m2**2))/2

        return self.g
        
    def getMeanM(self):
        return mean(self.M)
    def getMeanE(self):
        return mean(self.E)
    def getVarM(self):
        return var(self.M)
    def getVarE(self):
        return var(self.E)
