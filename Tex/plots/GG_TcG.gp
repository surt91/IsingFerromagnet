set terminal postscript eps enhanced color blacktext lw 2 26
set output 'GG_TcG.eps'

set xlabel '{/Symbol s}'
set ylabel '{/Italic g}'

set xr [-0.1:1.3]
set yr [0.905:0.935]
set size square

set key title 'GG' samplen 1

f(x)=0.9160

plot 'data/GG_TcG.dat' u 1:2:3 w ye pt 6 ps 1 lc 3 title "{/Italic g_c}",\
     f(x) w l lt 3 lc 3 notitle
