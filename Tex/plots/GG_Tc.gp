set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'GG_Tc.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T}'

set size square

plot 'data/GG_Tc.dat' w ye ps 2 pt 1 title "{/Italic T_c}"
