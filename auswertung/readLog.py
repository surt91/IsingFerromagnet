#! /usr/bin/env python2
# -*- coding: utf-8 -*-

import os

T=[]
A=[]
S=[]
L=[]
with open("log.txt", 'r') as f:
    for line in f:
        if "sigma =" in line:
            S.append(float(line.partition("sigma = ")[2].strip()))
        if "L     =" in line:
            L.append(float(line.partition("L     = ")[2].strip()))
        if line.startswith("T: "):
            T.append(list(map(float,line.partition("T: ")[2].split("      ")[:-1])))
        if line.startswith("A: "):
            A.append(list(map(float,line.partition("A:      ")[2].split("      ")[:-1])))
            #~ if min(A[-1]) < 0.3:
                #~ print T[-1]
                #~ print A[-1]

    num = len(T)/(len(set(S))*len(set(L)))

    t = [[0 for y in range(len(set(L)))] for x in range(len(set(S)))]
    a = [[0 for y in range(len(set(L)))] for x in range(len(set(S)))]
    for [i,s] in enumerate(sorted(list(set(S)))):
        for [j,l] in enumerate(sorted(list(set(L)))):
            t[i][j] = [T[n] for n in range(len(T)) if S[n] == s and L[n] == l]
            a[i][j] = [A[n] for n in range(len(T)) if S[n] == s and L[n] == l]

    aOut = [[[0 for x in a[i][j][0]] for j in range(len(set(L)))] for i in range(len(set(S)))]
    tOut = [[[0 for x in a[i][j][0]] for j in range(len(set(L)))] for i in range(len(set(S)))]
    for [i,s] in enumerate(sorted(list(set(S)))):
        for [j,l] in enumerate(sorted(list(set(L)))):
            for x in a[i][j]:
                for n,y in enumerate(x):
                    aOut[i][j][n] += y

    for [i,s] in enumerate(sorted(list(set(S)))):
        for [j,l] in enumerate(sorted(list(set(L)))):
            tOut[i][j] = t[i][j][0]
            for n in range(len(aOut[i][j])):
                aOut[i][j][n] /= num


    for [j,l] in enumerate(sorted(list(set(L)))):
        name = "L_"+str(l)
        directory = "../data/temps/"
        if not os.path.exists(directory):
            os.makedirs(directory)

        f = open(os.path.join(directory,name+".gp"), "w")
        #~ f.write("set terminal  postscript eps enhanced\n")
        #~ f.write("set output '{0}'\n".format(name+".eps"))
        f.write("set terminal png\n")
        f.write("set output '{0}'\n".format(name+".png"))
        f.write("set xlabel 'Temperatur'\n")
        f.write("set ylabel '{0}'\n".format("Akzeptanz"))
        f.write("set key right\n")

        f.write("plot ")
        tmpStr = ""
        for [i,s] in enumerate(sorted(list(set(S)))):
            tmpStr+=("'{0}' using 1:{1} w l title {2}, ".format(name+".dat", i+2, "'sigma" +" = {0}'".format(s)))
        f.write(tmpStr[:-2])
        f.close()

        f = open(os.path.join(directory,name+".dat"), "w")
        for [n,tx] in enumerate(tOut[0][j][:-1]):
            f.write("{0}".format(tx))
            for [i,s] in enumerate(sorted(list(set(S)))):
                # Spalten
                f.write(" {0}  ".format(aOut[i][j][n]))
            f.write("\n")
