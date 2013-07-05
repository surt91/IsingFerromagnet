set terminal postscript eps enhanced color lw 2 20
set output 'GG_Tc.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T}'

plot 'data/GG_Tc.dat' w ye
