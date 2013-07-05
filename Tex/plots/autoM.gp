set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'autoM.eps'

set xlabel '{/Italic t}'
set ylabel '{/Italic {/Symbol c}_m(t)}'
set xrange [0:1200]
set xtic 400

set size square

plot "data/autoM.dat" u 1:2 w l title "autocorrelation" lt 1
