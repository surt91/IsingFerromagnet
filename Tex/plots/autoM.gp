set terminal postscript eps enhanced color lw 2 20
set output 'autoM.eps'

set xlabel '{/Symbol t}'
set ylabel '{/Italic {/Symbol C}_m({/Symbol t})}'

plot "data/autoM.dat" u 1:2 w l title "autocorrelation" lt 1
