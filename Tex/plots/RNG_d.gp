set terminal postscript eps enhanced color blacktext lw 2 26
set output 'RNG_d.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic d}'

set xr [-0.1:1.3]

set format x "%.1f"
set format y "%.2f"

set size square
set key title 'RNG' samplen 1

plot "data/RNG_d_L16.dat" w ye lt 1 lc 5 pt 4 ps 2 title "L = 16",\
     "data/RNG_d_L32.dat" w ye lt 1 lc 1 pt 6 ps 2 title "L = 32",\
     "data/RNG_d_L16.dat" w l  lt 3 lc 5 notitle smooth csplines,\
     "data/RNG_d_L32.dat" w l  lt 3 lc 1 notitle smooth csplines
