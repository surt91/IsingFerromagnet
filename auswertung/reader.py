#! /usr/bin/env python
# -*- coding: utf-8 -*-

import csv
import logging
from numpy import mean, var, sqrt, std

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

    ## @var L
    # Kantenlänge (int)

    ## @var sigma
    # Unordnungsparameter (double)

    ## @var x
    # Unordnungsrealisierung (int)

    ## @var N
    # Anzahl Sweeps (Liste)

    ## @var M
    # Magnetisierung pro Spin (Liste)

    ## @var E
    # Energie (Liste)

    ## @var A
    # Parallel Tempering Akzeptanz (Liste)

    def __init__(self, filename):
        """! Construktor

            Bei der Initialisierung wird eine Datei eingelesen und die
            Spalten in Listen gespeichert.
            Die Datei sollte aus drei Spalten bestehen. In der ersten
            steht die Nummer des Sweeps, in der zweiten die Energien
            und in der dritten die Magnetisierung pro Spin.
        """
        if not ".dat" in filename:
            logging.info("Kein gültiger name: "+filename)
            return
        with open(filename, 'rb') as csvfile:
            logging.info("reading: " + filename)
            reader = csv.reader(csvfile, delimiter=' ')
            header = reader.next()

            self.T = " ".join(header).rpartition("T=")[2]
            self.T = [float(i) for i in self.T.split(",")[0:-1]]

            self.sigma = " ".join(header).rpartition("sigma=")[2].partition (" #")[0]
            self.x = " ".join(header).rpartition("x=")[2].partition (" #")[0]
            self.L = " ".join(header).rpartition("L=")[2].partition (" #")[0]

            # Zwischenspeichern, da Einträge im Reader verbraucht werden
            alle = [i for i in reader]
            footer = alle[-1][:-1]
            del alle[-1]

            self.N = [int(i[0]) for i in alle]
            self.E = [[float(j[2*i+1]) for j in alle] for i in range(len(self.T))]
            self.M = [[float(j[2*i+2]) for j in alle] for i in range(len(self.T))]

            self.A = [float(j) for j in footer[2:]]

            logging.info("finished reading")
