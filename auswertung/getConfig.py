# -*- coding: utf-8 -*-

from numpy import arange

def getConfig():
    """! getConfig()

    Diese Funktion gibt die Parameter für die Simulationsaufrufe aus
    """

    sigmas = (0.00, 0.03, 0.05, 0.08, 0.10, 0.12, 0.15, 0.20, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2)

    equilibrium             = {16:20*2,  32:50*2,  64:50*2, 128:100*2, 256:300*2}
    #~ equilibrium             = {16:0,  32:0,  64:0, 128:0, 256:0}
    autokorr                = {16: 3*2,  32: 5*2,  64: 7*2, 128: 12*2, 256: 16*2}
    realisierungUnordnung   = {16:100,   32:100,   64:100,  128:100,   256:100}
    sweeps                  = {16:10000, 32:10000, 64:5000, 128:5000,  256:5000}
    #~ sweeps                  = {16:0, 32:0, 64:0, 128:0,  256:0}

    ####################################################################
    # minimal:
    #~ t16=",".join([str(i) for i in arange(0.1,2.0,0.15)]+[str(i) for i in arange(2.0,3.3,0.2)])
    #~ t32l=",".join([str(i) for i in arange(0.1,0.9,0.14)]+[str(i) for i in arange(0.9,3.3,0.09)])
    #~ t32h=",".join([str(i) for i in arange(0.1,0.9,0.14)]+[str(i) for i in arange(0.9,3.3,0.09)])
    #~ t64l=",".join([str(i) for i in arange(0.1,0.9,0.15)]+[str(i) for i in arange(0.9,2.30,0.03)]+[str(i) for i in arange(2.30,3.3,0.05)]) #passend fur 64
    #~ t64h=",".join([str(i) for i in arange(0.1,0.8,0.08)]+[str(i) for i in arange(0.8,1.50,0.02)]+[str(i) for i in arange(1.5,2.30,0.035)]+[str(i) for i in arange(2.30,3.3,0.07)]) #passend fur 64
    #~ t128l=",".join([str(i) for i in arange(0.1,0.8,0.1)]+[str(i) for i in arange(0.8,1.42,0.03)]+[str(i) for i in arange(1.42,2.08,0.02)]+[str(i) for i in arange(2.08,2.44,0.01)]+[str(i) for i in arange(2.44,3.3,0.03)]) #passend fur 128
    #~ t128m=",".join([str(i) for i in arange(0.1,0.82,0.05)]+[str(i) for i in arange(0.82,1.42,0.01)]+[str(i) for i in arange(1.42,2.08,0.015)]+[str(i) for i in arange(2.08,2.44,0.02)]+[str(i) for i in arange(2.44,3.3,0.03)]) #passend fur 128
    #~ t128h=",".join([str(i) for i in arange(0.1,0.82,0.05)]+[str(i) for i in arange(0.82,1.42,0.01)]+[str(i) for i in arange(1.42,2.08,0.015)]+[str(i) for i in arange(2.08,2.44,0.03)]+[str(i) for i in arange(2.44,3.3,0.035)]) #passend fur 128
    #~ t256l=",".join([str(i) for i in arange(0.1,0.55,0.1)]+[str(i) for i in arange(0.55,1.5,0.06)]+[str(i) for i in arange(1.5,1.8,0.008)]+[str(i) for i in arange(1.8,2.08,0.01)]+[str(i) for i in arange(2.08,2.56,0.007)]+[str(i) for i in arange(2.56,3.3,0.017)]) #passend fur 256
    #~ t256m=",".join([str(i) for i in arange(0.1,0.55,0.1)]+[str(i) for i in arange(0.55,1.5,0.006)]+[str(i) for i in arange(1.5,1.8,0.004)]+[str(i) for i in arange(1.8,2.08,0.01)]+[str(i) for i in arange(2.08,2.44,0.007)]+[str(i) for i in arange(2.44,3.3,0.017)]) #passend fur 256
    #~ t256h=",".join([str(i) for i in arange(0.1,0.55,0.06)]+[str(i) for i in arange(0.55,1.0,0.009)]+[str(i) for i in arange(1.0,1.5,0.006)]+[str(i) for i in arange(1.5,1.8,0.01)]+[str(i) for i in arange(1.8,2.08,0.010)]+[str(i) for i in arange(2.08,2.44,0.01)]+[str(i) for i in arange(2.44,3.3,0.02)]) #passend fur 256
    ####################################################################
    # RNG:
    #~ t16l=",".join([str(i) for i in arange(0.1,1.8,0.15)]+[str(i) for i in arange(1.8,2.4,0.02)]+[str(i) for i in arange(2.4,3.3,0.2)])
    #~ t16m=",".join([str(i) for i in arange(0.1,1.4,0.15)]+[str(i) for i in arange(1.4,2.0,0.02)]+[str(i) for i in arange(2.0,3.3,0.2)])
    #~ t16h=",".join([str(i) for i in arange(0.1,1.1,0.15)]+[str(i) for i in arange(1.1,1.5,0.02)]+[str(i) for i in arange(1.5,3.3,0.2)])
    #~ t32l=",".join([str(i) for i in arange(0.1,0.9,0.14)]+[str(i) for i in arange(0.9,1.8,0.09)]+[str(i) for i in arange(1.8,2.4,0.02)]+[str(i) for i in arange(2.4,3.3,0.09)])
    #~ t32m=",".join([str(i) for i in arange(0.1,0.9,0.14)]+[str(i) for i in arange(0.9,1.4,0.09)]+[str(i) for i in arange(1.4,2.0,0.02)]+[str(i) for i in arange(2.0,3.3,0.09)])
    #~ t32h=",".join([str(i) for i in arange(0.1,0.9,0.14)]+[str(i) for i in arange(0.9,1.1,0.09)]+[str(i) for i in arange(1.1,1.5,0.02)]+[str(i) for i in arange(1.5,3.3,0.09)])
    #~ t64l=",".join([str(i) for i in arange(0.1,0.9,0.15)]+[str(i) for i in arange(0.9,2.30,0.03)]+[str(i) for i in arange(2.30,3.3,0.05)]) #passend fur 64
    #~ t64m=",".join([str(i) for i in arange(0.1,0.9,0.15)]+[str(i) for i in arange(0.9,2.30,0.03)]+[str(i) for i in arange(2.30,3.3,0.05)]) #passend fur 64
    #~ t64h=",".join([str(i) for i in arange(0.1,0.8,0.08)]+[str(i) for i in arange(0.8,1.50,0.02)]+[str(i) for i in arange(1.5,2.30,0.035)]+[str(i) for i in arange(2.30,3.3,0.07)]) #passend fur 64
    #~ t128l=",".join([str(i) for i in arange(0.1,0.8,0.1)]+[str(i) for i in arange(0.8,1.42,0.03)]+[str(i) for i in arange(1.42,2.08,0.02)]+[str(i) for i in arange(2.08,2.44,0.01)]+[str(i) for i in arange(2.44,3.3,0.03)]) #passend fur 128
    #~ t128m=",".join([str(i) for i in arange(0.1,0.82,0.05)]+[str(i) for i in arange(0.82,1.42,0.01)]+[str(i) for i in arange(1.42,2.08,0.015)]+[str(i) for i in arange(2.08,2.44,0.02)]+[str(i) for i in arange(2.44,3.3,0.03)]) #passend fur 128
    #~ t128h=",".join([str(i) for i in arange(0.1,0.82,0.05)]+[str(i) for i in arange(0.82,1.42,0.01)]+[str(i) for i in arange(1.42,2.08,0.015)]+[str(i) for i in arange(2.08,2.44,0.03)]+[str(i) for i in arange(2.44,3.3,0.035)]) #passend fur 128
    #~ t256l=",".join([str(i) for i in arange(0.1,0.55,0.1)]+[str(i) for i in arange(0.55,1.5,0.06)]+[str(i) for i in arange(1.5,1.8,0.008)]+[str(i) for i in arange(1.8,2.08,0.01)]+[str(i) for i in arange(2.08,2.56,0.007)]+[str(i) for i in arange(2.56,3.3,0.017)]) #passend fur 256
    #~ t256m=",".join([str(i) for i in arange(0.1,0.55,0.1)]+[str(i) for i in arange(0.55,1.5,0.006)]+[str(i) for i in arange(1.5,1.8,0.004)]+[str(i) for i in arange(1.8,2.08,0.01)]+[str(i) for i in arange(2.08,2.44,0.007)]+[str(i) for i in arange(2.44,3.3,0.017)]) #passend fur 256
    #~ t256h=",".join([str(i) for i in arange(0.1,0.55,0.06)]+[str(i) for i in arange(0.55,1.0,0.009)]+[str(i) for i in arange(1.0,1.5,0.006)]+[str(i) for i in arange(1.5,1.8,0.01)]+[str(i) for i in arange(1.8,2.08,0.010)]+[str(i) for i in arange(2.08,2.44,0.01)]+[str(i) for i in arange(2.44,3.3,0.02)]) #passend fur 256
#~
    #~ c16  = [(s,  realisierungUnordnung[16],  sweeps[16],  16,  t16l,  equilibrium[16],  autokorr[16]) for s in sigmas if 0.0<=s<0.2] \
         #~ + [(s,  realisierungUnordnung[16],  sweeps[16],  16,  t16m,  equilibrium[16],  autokorr[16]) for s in sigmas if 0.2<=s<0.3] \
         #~ + [(s,  realisierungUnordnung[16],  sweeps[16],  16,  t16h,  equilibrium[16],  autokorr[16]) for s in sigmas if 0.3<=s<1.3]
    #~ c32  = [(s,  realisierungUnordnung[32],  sweeps[32],  32,  t32l,  equilibrium[32],  autokorr[32]) for s in sigmas if 0.0<=s<0.2] \
         #~ + [(s,  realisierungUnordnung[32],  sweeps[32],  32,  t32m,  equilibrium[32],  autokorr[32]) for s in sigmas if 0.2<=s<0.3] \
         #~ + [(s,  realisierungUnordnung[32],  sweeps[32],  32,  t32h,  equilibrium[32],  autokorr[32]) for s in sigmas if 0.3<=s<1.3]
    #~ c64  = [(s,  realisierungUnordnung[64],  sweeps[64],  64,  t64l,  equilibrium[64],  autokorr[64]) for s in sigmas if 0.0<=s<0.2] \
         #~ + [(s,  realisierungUnordnung[64],  sweeps[64],  64,  t64m,  equilibrium[64],  autokorr[64]) for s in sigmas if 0.2<=s<0.3] \
         #~ + [(s,  realisierungUnordnung[64],  sweeps[64],  64,  t64h,  equilibrium[64],  autokorr[64]) for s in sigmas if 0.3<=s<1.3]
    #~ c128 = [(s, realisierungUnordnung[128], sweeps[128], 128, t128l, equilibrium[128], autokorr[128]) for s in sigmas if 0.0<=s<0.2] \
         #~ + [(s, realisierungUnordnung[128], sweeps[128], 128, t128m, equilibrium[128], autokorr[128]) for s in sigmas if 0.2<=s<0.3] \
         #~ + [(s, realisierungUnordnung[128], sweeps[128], 128, t128h, equilibrium[128], autokorr[128]) for s in sigmas if 0.3<=s<1.3]
    #~ c256 = [(s, realisierungUnordnung[256], sweeps[256], 256, t256l, equilibrium[256], autokorr[256]) for s in sigmas if 0.0<=s<0.2] \
         #~ + [(s, realisierungUnordnung[256], sweeps[256], 256, t256m, equilibrium[256], autokorr[256]) for s in sigmas if 0.2<=s<0.3] \
         #~ + [(s, realisierungUnordnung[256], sweeps[256], 256, t256h, equilibrium[256], autokorr[256]) for s in sigmas if 0.3<=s<1.3]

    ####################################################################
    # GG:
    t16l=",".join([str(i) for i in arange(0.1,2.0,0.15)]+[str(i) for i in arange(2.0,2.4,0.02)]+[str(i) for i in arange(2.4,3.0,0.2)]+[str(i) for i in arange(3.0,3.3,0.2)])
    t16m=",".join([str(i) for i in arange(0.1,2.0,0.15)]+[str(i) for i in arange(2.0,2.4,0.2)]+[str(i) for i in arange(2.4,3.0,0.02)]+[str(i) for i in arange(3.0,3.3,0.2)])
    t16h=",".join([str(i) for i in arange(0.1,2.0,0.15)]+[str(i) for i in arange(2.0,2.5,0.02)]+[str(i) for i in arange(2.5,3.3,0.2)])
    t32l=",".join([str(i) for i in arange(0.1,0.9,0.14)]+[str(i) for i in arange(0.9,2.0,0.09)]+[str(i) for i in arange(2.0,2.4,0.02)]+[str(i) for i in arange(2.4,3.3,0.2)])
    t32m=",".join([str(i) for i in arange(0.1,0.9,0.14)]+[str(i) for i in arange(0.9,2.4,0.09)]+[str(i) for i in arange(2.4,3.0,0.02)]+[str(i) for i in arange(3.0,3.3,0.2)])
    t32h=",".join([str(i) for i in arange(0.1,0.9,0.14)]+[str(i) for i in arange(0.9,2.0,0.09)]+[str(i) for i in arange(2.0,2.5,0.02)]+[str(i) for i in arange(2.5,3.3,0.2)])
    t64l=",".join([str(i) for i in arange(0.1,0.9,0.15)]+[str(i) for i in arange(0.9,2.30,0.03)]+[str(i) for i in arange(2.30,3.3,0.05)]) #passend fur 64
    t64m=",".join([str(i) for i in arange(0.1,0.9,0.15)]+[str(i) for i in arange(0.9,2.30,0.03)]+[str(i) for i in arange(2.30,3.3,0.05)]) #passend fur 64
    t64h=",".join([str(i) for i in arange(0.1,0.8,0.08)]+[str(i) for i in arange(0.8,1.50,0.02)]+[str(i) for i in arange(1.5,2.30,0.035)]+[str(i) for i in arange(2.30,3.3,0.07)]) #passend fur 64
    t128l=",".join([str(i) for i in arange(0.1,0.8,0.1)]+[str(i) for i in arange(0.8,1.42,0.03)]+[str(i) for i in arange(1.42,2.08,0.02)]+[str(i) for i in arange(2.08,2.44,0.01)]+[str(i) for i in arange(2.44,3.3,0.03)]) #passend fur 128
    t128m=",".join([str(i) for i in arange(0.1,0.82,0.05)]+[str(i) for i in arange(0.82,1.42,0.01)]+[str(i) for i in arange(1.42,2.08,0.015)]+[str(i) for i in arange(2.08,2.44,0.02)]+[str(i) for i in arange(2.44,3.3,0.03)]) #passend fur 128
    t128h=",".join([str(i) for i in arange(0.1,0.82,0.05)]+[str(i) for i in arange(0.82,1.42,0.01)]+[str(i) for i in arange(1.42,2.08,0.015)]+[str(i) for i in arange(2.08,2.44,0.03)]+[str(i) for i in arange(2.44,3.3,0.035)]) #passend fur 128
    t256l=",".join([str(i) for i in arange(0.1,0.55,0.1)]+[str(i) for i in arange(0.55,1.5,0.06)]+[str(i) for i in arange(1.5,1.8,0.008)]+[str(i) for i in arange(1.8,2.08,0.01)]+[str(i) for i in arange(2.08,2.56,0.007)]+[str(i) for i in arange(2.56,3.3,0.017)]) #passend fur 256
    t256m=",".join([str(i) for i in arange(0.1,0.55,0.1)]+[str(i) for i in arange(0.55,1.5,0.006)]+[str(i) for i in arange(1.5,1.8,0.004)]+[str(i) for i in arange(1.8,2.08,0.01)]+[str(i) for i in arange(2.08,2.44,0.007)]+[str(i) for i in arange(2.44,3.3,0.017)]) #passend fur 256
    t256h=",".join([str(i) for i in arange(0.1,0.55,0.06)]+[str(i) for i in arange(0.55,1.0,0.009)]+[str(i) for i in arange(1.0,1.5,0.006)]+[str(i) for i in arange(1.5,1.8,0.01)]+[str(i) for i in arange(1.8,2.08,0.010)]+[str(i) for i in arange(2.08,2.44,0.01)]+[str(i) for i in arange(2.44,3.3,0.02)]) #passend fur 256

    c16  = [(s,  realisierungUnordnung[16],  sweeps[16],  16,  t16l,  equilibrium[16],  autokorr[16]) for s in sigmas if s==0] \
         + [(s,  realisierungUnordnung[16],  sweeps[16],  16,  t16m,  equilibrium[16],  autokorr[16]) for s in sigmas if 0.0<s<0.4] \
         + [(s,  realisierungUnordnung[16],  sweeps[16],  16,  t16h,  equilibrium[16],  autokorr[16]) for s in sigmas if 0.4<=s<1.3]
    c32  = [(s,  realisierungUnordnung[32],  sweeps[32],  32,  t32l,  equilibrium[32],  autokorr[32]) for s in sigmas if s==0.0] \
         + [(s,  realisierungUnordnung[32],  sweeps[32],  32,  t32m,  equilibrium[32],  autokorr[32]) for s in sigmas if 0.0<=s<0.4] \
         + [(s,  realisierungUnordnung[32],  sweeps[32],  32,  t32h,  equilibrium[32],  autokorr[32]) for s in sigmas if 0.4<=s<1.3]
    c64  = [(s,  realisierungUnordnung[64],  sweeps[64],  64,  t64l,  equilibrium[64],  autokorr[64]) for s in sigmas if s==0.0] \
         + [(s,  realisierungUnordnung[64],  sweeps[64],  64,  t64m,  equilibrium[64],  autokorr[64]) for s in sigmas if 0.0<=s<0.4] \
         + [(s,  realisierungUnordnung[64],  sweeps[64],  64,  t64h,  equilibrium[64],  autokorr[64]) for s in sigmas if 0.4<=s<1.3]
    c128 = [(s, realisierungUnordnung[128], sweeps[128], 128, t128l, equilibrium[128], autokorr[128]) for s in sigmas if s==0.0] \
         + [(s, realisierungUnordnung[128], sweeps[128], 128, t128m, equilibrium[128], autokorr[128]) for s in sigmas if 0.0<=s<0.4] \
         + [(s, realisierungUnordnung[128], sweeps[128], 128, t128h, equilibrium[128], autokorr[128]) for s in sigmas if 0.4<=s<1.3]
    c256 = [(s, realisierungUnordnung[256], sweeps[256], 256, t256l, equilibrium[256], autokorr[256]) for s in sigmas if s==0.0] \
         + [(s, realisierungUnordnung[256], sweeps[256], 256, t256m, equilibrium[256], autokorr[256]) for s in sigmas if 0.0<=s<0.4] \
         + [(s, realisierungUnordnung[256], sweeps[256], 256, t256h, equilibrium[256], autokorr[256]) for s in sigmas if 0.4<=s<1.3]

    #configs = c16 + c32 + c64 + c128 + c256
    #configs = c128 + c64 + c32 + c16
    configs = c16 + c32
    #configs = c32
    #configs = c64
    #configs = c128
    #configs = c256
    #configs = c256 + c128 + c64

    return configs

