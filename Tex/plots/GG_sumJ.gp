set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'GG_sumJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic < {/Symbol S}_{<ij>} J_{ij} >}'

set size square

plot "data/RNG_sumJ_L16.dat" w ye title "L = 16" pt 1 ps 2,\
     "data/RNG_sumJ_L32.dat" w ye title "L = 32" pt 1 ps 2
