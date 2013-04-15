#! /usr/bin/env python
# -*- coding: utf-8 -*-

import csv
from numpy import mean, var, sqrt

#~ from scipy.integrate import trapz

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

    ## @var T
    # Temperatur (Liste)
    ## @var N
    # Anzahl Sweeps (Liste)
    ## @var M
    # Magnetisierung pro Spin (Liste)
    ## @var E
    # Energie (Liste)
    ## @var tau
    # Autokorrelationszeit

    def __init__(self, filename):
        """! Construktor

            Bei der Initialisierung wird eine Datei eingelesen und die
            Spalten in Listen gespeichert.
            Die Datei sollte aus drei Spalten bestehen. In der ersten
            steht die Nummer des Sweeps, in der zweiten die Energien
            und in der dritten die Magnetisierung pro Spin.
        """
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
        """! Autokorreltationsfunktion

            Diese Funktion berechnet dei Autokorrelationsfunktion bei der
            Verschiebung um t. Berechnung erfolgt naiv per Definition
            in O(n) Laufzeit.
        """
        m = self.M
        t_max = len(m)
        eins = sum([m[i] * m[i+t] for i in range(0,t_max-t)])
        zwei = sum([m[i] for i in range(0,t_max-t)]) * sum([m[i+t] for i in range(0,t_max-t)])/(t_max-t)
        return abs(eins - zwei)

    def getAutocorrTime(self):
        """! Berechnet die Autokorrelationszeit \f$ \tau \f$

            Die Berechnung erfolgt durch Integration (hier sollte eine
            Summation genau genug sein) der Autokorrelationsfunktion.
            vgl. \cite newman1999monte S. 62 (3.20)
        """
        m = self.M
        x0 = self.X(0)
        autocorr = [self.X(t) for t in range(0,len(m))]
        #~ self.tau = trapz(autocorr)/x0
        self.tau = sum(autocorr)/x0
        #~ for i in autocorr:
            #~ print i
        return self.tau

    def getBinder(self,m=False):
        """! Berechnet die Binder Kumulante

            vgl. \cite katzgraber2011introduction S. 12 (19)
        """
        if not m:
            m = self.M
        m4 = mean([i**4 for i in m])
        m2 = mean([i**2 for i in m])

        g=(3-m4/(m2**2))/2

        return g

    def getErrorJackknife(self,estimator,data):
        """! berechnet eine Unsicherheit der Binderkumulante

            Dabei wird die Jackknife Methode benutzt.
            vgl \cite young2012everything S. 12
        """
        m = data
        g = [0 for i in range(len(m))]
        for i in range(len(m)):
            m_tmp = [j for j in m]
            del m_tmp[i]

            g[i] = estimator(m_tmp)

        # Jackknife Estimate
        #mean(g)
        # Jackknife Error
        return ( sqrt(len(m)-1) * sqrt(mean([i**2 for i in g]) - mean(g)**2) )

    def getMeanM(self):
        return mean([abs(i) for i in self.M])
    def getMeanE(self):
        return mean(self.E)
    def getVarM(self):
        return var([abs(i) for i in self.M])
    def getVarE(self):
        return var(self.E)
