set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'equiE.eps'

set xlabel 'Number of sweeps {/Italic N_s}'
set ylabel 'Energy per site {/Italic E}'

set xtics 50

set size square

plot "data/equiE.dat" u 1:2 w l title "random" lt 1,\
     "data/equiE.dat" u 1:3 w l title "all up" lt 1 lc 3
