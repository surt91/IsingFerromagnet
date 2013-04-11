#! /usr/bin/env python

import csv

class output_reader():

    def __init__(self, filename):
        with open(filename, 'rb') as csvfile:
            reader = csv.reader(csvfile, delimiter=' ')
            header = reader.next()

            self.T = header[0].rpartition("T=")[2]

            alle = [[int(i[0]), float(i[1]), float(i[2])] for i in reader]

            self.N = [i[0] for i in alle]
            self.E = [i[1] for i in alle]
            self.M = [i[2] for i in alle]
