set terminal dumb

plot 'data/Specific_Heat_0.dat' using 1:2 w l notitle smooth csplines
ymax16=GPVAL_DATA_Y_MAX

plot 'data/Specific_Heat_0.dat' using 1:4 w l notitle smooth csplines
ymax32=GPVAL_DATA_Y_MAX

plot 'data/Specific_Heat_0.dat' using 1:6 w l notitle smooth csplines
ymax64=GPVAL_DATA_Y_MAX

plot 'data/Specific_Heat_0.dat' using 1:8 w l notitle smooth csplines
ymax128=GPVAL_DATA_Y_MAX

set print "data/tmp.dat"
print  16,  ymax16
print  32,  ymax32
print  64,  ymax64
print 128, ymax128

set terminal postscript eps enhanced color blacktext lw 2 26
set output 'Specific_Heat_0_log.eps'

set xlabel '{/Italic L}'
set ylabel '{/Italic c}'

set xtics (16,32,64,128)

set xr [12:136]

set size square

set key samplen 1. left Left

f(x) = a*log(x)
f2(x) = b*x**d

fit f(x) "data/tmp.dat" u 1:2 via a
fit f2(x) "data/tmp.dat" u 1:2 via b,d

plot "data/tmp.dat" u 1:2 w p title "max({/Italic c})",\
     f(x) lc 3 lt 3 title "{/Italic a} log({/Italic L})",\
     f2(x) lc 2 lt 3 title "{/Italic b} {/Italic L^d"
