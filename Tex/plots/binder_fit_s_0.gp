set terminal postscript eps enhanced color lw 2 20
set output "binder_fit_s_0.eps"

set key left bottom Left

plot "data/binderFit16_s_0.000.dat" u 1:2 w l lt 1 lc rgb "red" title "Fit: L = 16",\
     "data/binderFit32_s_0.000.dat" u 1:2 w l lt 1 lc rgb "green" title "Fit: L = 32",\
     "data/binderFit64_s_0.000.dat" u 1:2 w l lt 1 lc rgb "blue" title "Fit: L = 64",\
     "data/binderFit128_s_0.000.dat" u 1:2 w l lt 1 lc rgb "violet" title "Fit: L = 128",\
     "data/binder16_s_0.000.dat" u 1:2:3 w ye lt 1 lc rgb "red" title "L = 16",\
     "data/binder32_s_0.000.dat" u 1:2:3 w ye lt 1 lc rgb "green" title "L = 32",\
     "data/binder64_s_0.000.dat" u 1:2:3 w ye lt 1 lc rgb "blue" title "L = 64",\
     "data/binder128_s_0.000.dat" u 1:2:3 w ye lt 1 lc rgb "violet" title "L = 128"

set xlabel 'Temperatur {/Italic T}'
set ylabel 'Binder cumulant {/Italic g}'
