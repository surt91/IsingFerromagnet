set terminal postscript eps enhanced color lw 2 30
set output 'GG_Tc.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T}'

plot 'data/GG_Tc.dat' w ye ps 2 pt 1
