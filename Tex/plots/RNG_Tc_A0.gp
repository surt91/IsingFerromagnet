set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'RNG_Tc_A0.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T}'

set xr [-0.1:1.3]
#set yr [0.42:0.58]
set size square

#set logscale xy

plot "data/RNG_Tc_A0.dat" u 1:2:3 w ye ps 2 pt 4 title "{/Italic T_c}"
