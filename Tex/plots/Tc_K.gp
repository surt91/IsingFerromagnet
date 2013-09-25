set terminal postscript eps enhanced color blacktext lw 2 26
set output 'Tc_K.eps'

set xlabel '{/Italic K}'
set ylabel '{/Italic T}'

set yrange [1.2:3.3]

set size square

plot "< paste data/RNG_deg_L32.dat data/RNG_Tc.dat" u 2:5:3:6 w xyerrorbars ps 2 pt 4 title "RNG",\
     "< paste data/GG_deg_L32.dat data/GG_Tc.dat" u 2:5:3:6 w xyerrorbars ps 2 pt 6 lc 3 title "GG"
