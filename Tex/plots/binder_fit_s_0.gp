set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output "binder_fit_s_0.eps"

set key left bottom Left

set xlabel "{/Italic T}"
set ylabel "{/Italic g}"
set xtics 0.05
set xr [2.2:2.35]

set size square

plot "data/binderFit16_s_0.000.dat" u 1:2 w l lt 1 lc 1 notitle,\
     "data/binderFit32_s_0.000.dat" u 1:2 w l lt 1 lc 2 notitle,\
     "data/binderFit64_s_0.000.dat" u 1:2 w l lt 1 lc 3 notitle,\
     "data/binderFit128_s_0.000.dat" u 1:2 w l lt 1 lc 4 notitle,\
     "data/binder16_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 1 lc 1 title "{/Italic L} = 16",\
     "data/binder32_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 1 lc 2 title "{/Italic L} = 32",\
     "data/binder64_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 1 lc 3 title "{/Italic L} = 64",\
     "data/binder128_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 1 lc 4 title "{/Italic L} = 128"

set xlabel 'Temperatur {/Italic T}'
set ylabel 'Binder cumulant {/Italic g}'
