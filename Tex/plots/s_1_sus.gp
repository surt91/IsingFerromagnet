set terminal postscript eps enhanced color blacktext lw 2 26
set output 's_1_sus.eps'

set key samplen 1. right top Left                 # customize key
set xl "{/Italic T}"
set yl "{/Italic {/Symbol c}}"  # set x,y labels

set xr [1:1.7]

set size square

p "data/ScalanaSusceptibility_S_1.00_L_128.dat" u 1:2:3  w ye lt 1 lc 4 ps 2 pt 10 t "{/Italic L} = 128" \
, "data/ScalanaSusceptibility_S_1.00_L_64.dat"  u 1:2:3  w ye lt 1 lc 3 ps 2 pt  8 t "{/Italic L} = 64" \
, "data/ScalanaSusceptibility_S_1.00_L_32.dat"  u 1:2:3  w ye lt 1 lc 2 ps 2 pt  6 t "{/Italic L} = 32" \
, "data/ScalanaSusceptibility_S_1.00_L_128.dat" u 1:2:3  w l  lt 3 lc 4 notitle smooth csplines \
, "data/ScalanaSusceptibility_S_1.00_L_64.dat"  u 1:2:3  w l  lt 3 lc 3 notitle smooth csplines \
, "data/ScalanaSusceptibility_S_1.00_L_32.dat"  u 1:2:3  w l  lt 3 lc 2 notitle smooth csplines
