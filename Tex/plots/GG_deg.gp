set terminal postscript eps enhanced color blacktext lw 2 26
set output 'GG_deg.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic K}'

set xr [-0.1:1.3]
set yr [3.9:5.2]

set size square

plot "data/GG_deg_L16.dat" w ye lt 1 lc 1 pt 4 ps 2 title "L = 16",\
     "data/GG_deg_L32.dat" w ye lt 1 lc 2 pt 6 ps 2 title "L = 32",\
     "data/GG_deg_L16.dat" w l  lt 3 lc 1 notitle smooth csplines,\
     "data/GG_deg_L32.dat" w l  lt 3 lc 2 notitle smooth csplines
