set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 's_0_sus.eps'

set key samplen 1. right top Left                 # customize key
set xl "{/Italic T}"
set yl "{/Italic {/Symbol c}}"  # set x,y labels

set xr [2:2.7]

set size square

p "data/ScalanaSusceptibility_S_0.00_L_128.dat"  u 1:2:3  w ye lc 4 ps 2 pt 10 t "{/Italic L} = 128" \
, "data/ScalanaSusceptibility_S_0.00_L_64.dat"  u 1:2:3  w ye lc 3 ps 2 pt 8 t "{/Italic L} = 64" \
, "data/ScalanaSusceptibility_S_0.00_L_32.dat"  u 1:2:3  w ye lc 2 ps 2 pt 6 t "{/Italic L} = 32"
