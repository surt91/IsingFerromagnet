set terminal postscript eps enhanced color blacktext lw 2 26
set output 'RNG_TcG.eps'

set xlabel '{/Symbol s}'
set ylabel '{/Italic g}'

set xr [-0.1:1.3]
set size square

f(x)=0.9160

plot 'data/RNG_TcG.dat' u 1:2:3 w ye pt 4 ps 1 lc 1 title "{/Italic g_c}",\
     f(x) w l lt 3 lc 1 notitle