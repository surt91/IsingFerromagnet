set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'RNG_Tc.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T}'
set xr [-0.1:1.3]

set size square

plot 'data/RNG_Tc.dat' w ye ps 2 pt 4 title "{/Italic T_c}"
