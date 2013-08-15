set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'RNG_meanJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic {/Symbol \341}J{/Symbol \361}}'

set size square

plot "data/RNG_meanJ_L16.dat" w ye title "L = 16" pt 4 ps 2,\
     "data/RNG_meanJ_L32.dat" w ye title "L = 32" pt 6 ps 2
