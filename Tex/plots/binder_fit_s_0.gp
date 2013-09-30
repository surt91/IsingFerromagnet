set terminal postscript eps enhanced color blacktext lw 2 26
set output "binder_fit_s_0.eps"

set key samplen 1. left bottom Left

set xlabel "{/Italic T}"
set ylabel "{/Italic g}"

set xtics 0.1
set xtics add ("T_c" 2.2691)
set xr [2.2:2.4]
set yr [0:1]

set size square
set arrow from 2.2691,0 to 2.2691,1 nohead

plot "data/binderFit16_s_0.000.dat" u 1:2 w l lt 2 lc 1 notitle,\
     "data/binderFit32_s_0.000.dat" u 1:2 w l lt 2 lc 2 notitle,\
     "data/binderFit64_s_0.000.dat" u 1:2 w l lt 2 lc 3 notitle,\
     "data/binderFit128_s_0.000.dat" u 1:2 w l lt 2 lc 4 notitle,\
     "data/binder16_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 4 lc 1 title "{/Italic L} = 16",\
     "data/binder32_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 6 lc 2 title "{/Italic L} = 32",\
     "data/binder64_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 8 lc 3 title "{/Italic L} = 64",\
     "data/binder128_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 10 lc 4 title "{/Italic L} = 128"

set xlabel 'Temperatur {/Italic T}'
set ylabel 'Binder cumulant {/Italic g}'
