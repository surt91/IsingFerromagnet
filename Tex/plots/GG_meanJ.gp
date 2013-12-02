set terminal postscript eps enhanced color blacktext lw 2 26
set output 'GG_meanJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic {/Symbol \341}J{/Symbol \361}}'

set key title 'GG' samplen 1

set format xy "%.1f"

set size square

plot "data/GG_meanJ_L16.dat" w ye lt 1 lc 4 pt 4 ps 2 title "L = 16",\
     "data/GG_meanJ_L32.dat" w ye lt 1 lc 3 pt 6 ps 2 title "L = 32",\
     "data/GG_meanJ_L16.dat" w l  lt 3 lc 4 notitle smooth csplines,\
     "data/GG_meanJ_L32.dat" w l  lt 3 lc 3 notitle smooth csplines
