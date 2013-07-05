set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'RNG_Tc_normJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T / <{/Symbol S}_{<i,j>} J_{ij}>}'

set xr [-0.1:1.3]
set yr [0.4:0.6]
set size square

plot "< paste data/RNG_Tc.dat data/RNG_sumJ_L32.dat" u 1:($2/$5):($3/$5) w ye ps 2 pt 4 title "{/Italic T_c}"
