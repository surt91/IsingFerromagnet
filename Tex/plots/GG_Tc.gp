set terminal postscript eps enhanced color blacktext lw 2 26
set output 'GG_Tc.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T}'
set xr [-0.1:1.3]

set key title 'GG' samplen 1

set size square

plot 'data/GG_Tc.dat' w ye ps 2 pt 6 lc 3 title "{/Italic T_c}",\
     "" w l lt 3 lc 3 notitle smooth csplines
