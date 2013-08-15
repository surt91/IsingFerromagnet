set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'GG_sumJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic {/Symbol \341} {/Symbol S}_{{/Symbol \341}i,j{/Symbol \361}} J_{ij} {/Symbol \361}}'
set xr [-0.1:1.3]
set yr [3.9:5.0]

set size square

plot "data/GG_sumJ_L16.dat" w ye title "L = 16" pt 4 ps 2,\
     "data/GG_sumJ_L32.dat" w ye title "L = 32" pt 6 ps 2
