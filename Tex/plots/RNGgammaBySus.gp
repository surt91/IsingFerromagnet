set terminal postscript eps enhanced color blacktext lw 2 26
set output 'RNGgammaBySus.eps'

set logscale xy

set xlabel '{/Italic L}'
set ylabel '{/Italic {/Symbol c}}_{max}'

set logscale x 2

set xr [11.3:170.0]
set yr [3:1000]

set format x "%.1f"

set size square

set fit errorvariables

set key samplen 1. bottom right Right spacing 1.5
set key title 'RNG' samplen 1

f1(x)=a1*x**b1
f2(x)=a2*x**b2
fit f1(x) "data/RNGSusceptibility_Max_s_0.0.dat" u 1:2:3 via a1,b1
set label 1 sprintf("{/Symbol g/n} = %.3f(%.0f)", b1, b1_err*1e3/FIT_STDFIT) at graph 0.45,0.35 left rotate by 30

fit f2(x) "data/RNGSusceptibility_Max_s_1.0.dat" u 1:2:3 via a2,b2
set label 2 sprintf("{/Symbol g/n} = %.3f(%.0f)", b2, b2_err*1e3/FIT_STDFIT) at graph 0.25,0.6 left rotate by 30

plot "data/RNGSusceptibility_Max_s_1.0.dat" u 1:2:3 w ye pt 6 ps 2 lt 1 lc 3 t "{/Italic {/Symbol c}}_{max} at {/Symbol s} = 1.0", f2(x) lt 3 lc 3 notitle, \
     "data/RNGSusceptibility_Max_s_0.0.dat" u 1:2:3 w ye pt 4 ps 2 lt 1 lc 1 t "{/Italic {/Symbol c}}_{max} at {/Symbol s} = 0.0", f1(x) lt 3 lc 1 notitle
