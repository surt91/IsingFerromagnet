set terminal postscript eps enhanced color blacktext lw 2 26
set output "binder_s_0.eps"

set key samplen 1. left bottom Left

set xlabel "{/Italic T}"
set ylabel "{/Italic g}"
#set xtics 0.05
set xr [1.6:3.2]
set yr [0:1]

set size square

plot "data/binder_0.00.dat" u 1:2:3 w ye lt 1 ps 2 pt  4 lc 1 title "{/Italic L} = 16",\
     "data/binder_0.00.dat" u 1:4:5 w ye lt 1 ps 2 pt  6 lc 2 title "{/Italic L} = 32",\
     "data/binder_0.00.dat" u 1:6:7 w ye lt 1 ps 2 pt  8 lc 3 title "{/Italic L} = 64",\
     "data/binder_0.00.dat" u 1:8:9 w ye lt 1 ps 2 pt 10 lc 4 title "{/Italic L} = 128",\
     "data/binder_0.00.dat" u 1:2:3 w l  lt 3 lc 1 notitle smooth csplines,\
     "data/binder_0.00.dat" u 1:4:5 w l  lt 3 lc 2 notitle smooth csplines,\
     "data/binder_0.00.dat" u 1:6:7 w l  lt 3 lc 3 notitle smooth csplines,\
     "data/binder_0.00.dat" u 1:8:9 w l  lt 3 lc 4 notitle smooth csplines

set xlabel 'Temperatur {/Italic T}'
set ylabel 'Binder cumulant {/Italic g}'
