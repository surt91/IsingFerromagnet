set terminal postscript eps enhanced color blacktext lw 2 26
set output 'Tc_J.eps'

set xlabel '{/Italic {/Symbol \341} {/Symbol S}_{{/Symbol \341}i,j{/Symbol \361}} J_{ij} {/Symbol \361}}'
set ylabel '{/Italic T}'

set yrange [1.2:3.3]

set format xy "%.1f"

set key samplen 1

set size square

plot "< paste data/RNG_sumJ_L32.dat data/RNG_Tc.dat" u 2:5:3:6 w xyerrorbars ps 2 pt 4 title "RNG",\
     "< paste data/GG_sumJ_L32.dat data/GG_Tc.dat" u 2:5:3:6 w xyerrorbars ps 2 pt 6  lt 1 lc 3 title "GG"
