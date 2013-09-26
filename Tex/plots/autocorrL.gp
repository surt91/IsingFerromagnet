set terminal postscript eps enhanced color blacktext lw 2 26
set output 'autocorrL.eps'

set xlabel '{/Italic L}'
set ylabel '{/Italic {/Symbol t}}'
set logscale x 2
#set format x "2^{%L}"
set xrange [11.3:362.0]
set logscale y
set format y "10^{%L}"
set size square

set key left Left

set fit errorvariables

f(x)=a*x**b
fit f(x) "data/autocorrL2.dat" u 1:2:3 via a,b

set label 1 sprintf("{/Italic a} = %.2f(%.0f)\n{/Italic z'} = %.2f(%.0f)\n{/Symbol c}^2 = %.1f", a, a_err*1e2/FIT_STDFIT, b, b_err*1e2/FIT_STDFIT, FIT_STDFIT**2) at graph 0.05,0.7 left

plot "data/autocorrL2.dat" u 1:2:3 w ye pt 4 ps 2 t "{/Symbol t}",\
     f(x) w l lc 1 lt 3 t "{/Italic aL^{z'}}"
