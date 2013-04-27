#! /usr/bin/env python2
# -*- coding: utf-8 -*-

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
    for [i,s] in enumerate(sorted(list(set(S)))):
        for [j,l] in enumerate(sorted(list(set(L)))):
            for x in a[i][j]:
                for n,y in enumerate(x):
                    aOut[i][j][n] += y

    for [i,s] in enumerate(sorted(list(set(S)))):
        for [j,l] in enumerate(sorted(list(set(L)))):
            for n in range(len(aOut[i][j])):
                aOut[i][j][n] /= num


    for [i,s] in enumerate(sorted(list(set(S)))):
        print "sigma = ", s
        for [j,l] in enumerate(sorted(list(set(L)))):
            print "L = ", l
            for [n,x] in enumerate(aOut[i][j]):
                print " {0:.2f}".format(t[i][j][0][n]),
                if x < 0.3:
                    print '\033[91m', "{0:.2f}".format(x), '\033[0m'
                else:
                    print " {0:.2f}".format(x)
            print
