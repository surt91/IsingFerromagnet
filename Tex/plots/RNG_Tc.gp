set terminal postscript eps enhanced color blacktext lw 2 26
set output 'RNG_Tc.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T}'
set xr [-0.1:1.3]
set yr [1.1:2.4]

set format xy "%.1f"

set size square

set key title 'RNG' samplen 1

plot 'data/RNG_Tc.dat' w ye lt 1 lc 1 ps 2 pt 4 title "{/Italic T_c}",\
     "" w l lt 3 lc 1 notitle smooth csplines
