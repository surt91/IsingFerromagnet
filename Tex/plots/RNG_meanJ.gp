set terminal postscript eps enhanced color blacktext lw 2 26
set output 'RNG_meanJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic {/Symbol \341}J{/Symbol \361}}'

set format xy "%.1f"

set size square

set key title 'RNG' samplen 1

plot "data/RNG_meanJ_L16.dat" w ye lt 1 lc 5 pt 4 ps 2 title "L = 16",\
     "data/RNG_meanJ_L32.dat" w ye lt 1 lc 1 pt 6 ps 2 title "L = 32",\
     "data/RNG_meanJ_L16.dat" w l  lt 3 lc 5 notitle smooth csplines,\
     "data/RNG_meanJ_L32.dat" w l  lt 3 lc 1 notitle smooth csplines
