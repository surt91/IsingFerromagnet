set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 's_1_meanM.eps'

set key samplen 1. left bottom Left                 # customize key
set xl "{/Italic (T-T_c) L^{1/{/Symbol n}}}"
set yl "{/Italic <|m|>}"  # set x,y labels

set xr [1.5:2.8]

set size square

p "data/ScalanaMean_M_S_1.00_L_32.dat"  u 1:2:3  w ye lc 2 ps 2 pt 6 t "{/Italic L} = 32" \
, "data/ScalanaMean_M_S_1.00_L_64.dat"  u 1:2:3  w ye lc 3 ps 2 pt 8 t "{/Italic L} = 64" \
, "data/ScalanaMean_M_S_1.00_L_128.dat"  u 1:2:3  w ye lc 4 ps 2 pt 10 t "{/Italic L} = 128"
