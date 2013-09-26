set terminal postscript eps enhanced color blacktext lw 2 26
set output 'Tc_K.eps'

set xlabel '{/Italic K}'
set ylabel '{/Italic T}'

set key samplen 1. left top Left

set yrange [1.1:3.0]

set size square

set fit errorvariables

a1=1; b1=1;
a2=1; b2=1;
f1(x) = a1*x**b1
f2(x) = a2*x**b2

fit f1(x) "< paste data/RNG_deg_L32.dat data/RNG_Tc.dat" u 2:5:6 via a1,b1
set label 1 sprintf("b_{RNG} = %.4f(%.0f), {/Symbol c}^2=%.0f", b1, b1_err*1e4/FIT_STDFIT, FIT_STDFIT**2) at graph 0.20,0.17 left

fit [4.001:6]f2(x) "< paste data/GG_deg_L32.dat data/GG_Tc.dat" u 2:5:6 via a2,b2
set label 2 sprintf("b_{GG} = %.4f(%.0f), {/Symbol c}^2=%.0f", b2, b2_err*1e4/FIT_STDFIT, FIT_STDFIT**2) at graph 0.20,0.07 left

plot "< paste data/RNG_deg_L32.dat data/RNG_Tc.dat" u 2:5:3:6 w xyerrorbars ps 2 pt 4 title "RNG", f1(x) lt 3 lc 1 notitle,\
     "< paste data/GG_deg_L32.dat data/GG_Tc.dat" u 2:5:3:6 w xyerrorbars ps 2 pt 6 lt 1 lc 3 title "GG", f2(x) lt 3 lc 3 notitle
