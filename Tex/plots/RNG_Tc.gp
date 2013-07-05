set terminal postscript eps enhanced color lw 2 30
set output 'RNG_Tc.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T}'

plot 'data/RNG_Tc.dat' w ye
