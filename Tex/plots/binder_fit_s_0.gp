set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output "binder_fit_s_0.eps"

set key left bottom Left

set xlabel "{/Italic T}"
set ylabel "{/Italic g}"
set xtics 0.01

set size square

plot "data/binderFit16_s_0.000.dat" u 1:2 w l lt 1 lc rgb "red" notitle,\
     "data/binderFit32_s_0.000.dat" u 1:2 w l lt 1 lc rgb "green" notitle,\
     "data/binderFit64_s_0.000.dat" u 1:2 w l lt 1 lc rgb "blue" notitle,\
     "data/binderFit128_s_0.000.dat" u 1:2 w l lt 1 lc rgb "violet" notitle,\
     "data/binder16_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 1 lc rgb "red" title "L = 16",\
     "data/binder32_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 1lc rgb "green" title "L = 32",\
     "data/binder64_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 1lc rgb "blue" title "L = 64",\
     "data/binder128_s_0.000.dat" u 1:2:3 w ye lt 1 ps 2 pt 1lc rgb "violet" title "L = 128"

set xlabel 'Temperatur {/Italic T}'
set ylabel 'Binder cumulant {/Italic g}'
