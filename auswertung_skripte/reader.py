#! /usr/bin/env python
# -*- coding: utf-8 -*-

import csv
from numpy import mean, var, sqrt, std

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

            self.T = "".join(header).rpartition("T=")[2]
            self.T = [float(i) for i in self.T.split(",")[0:-1]]

            # Zwischenspeichern, da Einträge im Reader verbraucht werden
            alle = [i for i in reader]

            self.N = [int(i[0]) for i in alle]
            self.E = [[float(j[2*i+1]) for j in alle] for i in range(len(self.T))]
            self.M = [[float(j[2*i+2]) for j in alle] for i in range(len(self.T))]

            print "finished reading"

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
