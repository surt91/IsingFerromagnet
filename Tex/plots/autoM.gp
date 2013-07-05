set terminal postscript eps enhanced color lw 2 30
set output 'autoM.eps'

set xlabel 't'
set ylabel '{/Italic {/Symbol c}_m(t)}'

plot "data/autoM.dat" u 1:2 w l title "autocorrelation" lt 1
