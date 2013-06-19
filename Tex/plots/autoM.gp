set terminal postscript eps enhanced color blacktext lw 2 26
set output 'autoM.eps'

set xlabel '{/Italic t}'
set ylabel '{/Italic {/Symbol c}_m(t)}'
set xrange [0:800]
set xtic 400
set yr [0.09:1]
set logscale y
set format y "10^{%L}"
set size square

f(x)=exp(-x/a)
fit f(x) "data/autoM.dat" via a

plot "data/autoM.dat" u 1:2 w l title "autocorrelation" lt 1, \
     f(x) w l lc 1 lt 3 notitle
