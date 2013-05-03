from numpy import arange
# -*- coding: utf-8 -*-

def getConfig():
    """! getConfig()

    Diese Funktion gibt die Parameter für die Simulationsaufrufe aus
    """
    t16=",".join([str(i) for i in arange(0.1,3.3,0.15)]) #passend fur 16 und 32
    t32=",".join([str(i) for i in arange(0.1,3.3,0.09)]) #passend fur 16 und 32
    t64=",".join([str(i) for i in arange(0.1,0.8,0.08)]+[str(i) for i in arange(0.8,2.30,0.03)]+[str(i) for i in arange(2.30,3.3,0.05)]) #passend fur 64
    t128=",".join([str(i) for i in arange(0.1,0.82,0.05)]+[str(i) for i in arange(0.82,1.42,0.01)]+[str(i) for i in arange(1.42,2.08,0.02)]+[str(i) for i in arange(2.08,2.44,0.01)]+[str(i) for i in arange(2.44,3.3,0.03)]) #passend fur 128
    t256=",".join([str(i) for i in arange(0.1,0.55,0.06)]+[str(i) for i in arange(0.55,1.5,0.006)]+[str(i) for i in arange(1.5,1.8,0.004)]+[str(i) for i in arange(1.8,2.08,0.015)]+[str(i) for i in arange(2.08,2.44,0.007)]+[str(i) for i in arange(2.44,3.3,0.017)]) #passend fur 256
    configs = [ ( 16,  t16,  20*2,  2),
                ( 32,  t32,  30*2,  2),
                ( 64,  t64,  50*2,  6),
                (128, t128, 150*2, 10),
                (256, t256, 300*2, 16)
                                       ]
    return configs
