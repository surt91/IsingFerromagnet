set terminal postscript eps enhanced color lw 2 30
set output 'equiE.eps'

set xlabel 'Number of sweeps {/Italic N}'
set ylabel 'Energy per site {/Italic E}'

set xtics 50

plot "data/equiE.dat" u 1:2 w l title "random" lt 1,\
     "data/equiE.dat" u 1:3 w l title "all up" lt 1 lc 2
