set terminal epslatex standalone size 5.6cm,5.6cm font 10
set output 'TcK.tex'

set size nosquare
set lmargin at screen 0.18
set rmargin at screen 0.98
set bmargin at screen 0.18
set tmargin at screen 0.96

set xlabel '$K$'
set ylabel '$T$' offset 2.2,0

set key samplen 1. left top Left

set xrange [2.5:5.2]
set yrange [1.1:3.0]

set format y "$%.1f$"
set format x "$%.0f$"

set xtics (1,2,3,4,5,6)
set ytics (1.2,1.6,2.0,2.4,2.8,3.2)

set log xy

#set size square

set fit errorvariables

#~ set label 3 "Fit to $aK^b$" at graph 0.65,0.27 left

a1=1; b1=1;
a2=1; b2=1;
f1(x) = a1*x**b1
f2(x) = a2*x**b2

xmin=2.8
xmax=3.9

fit [xmin:xmax]f1(x) "< paste data/RNG_deg_L32.dat data/RNG_Tc.dat" u 2:5:6 via a1,b1
#~ set label 1 sprintf("\\shortstack[r]{$b_{RNG} = %.3f(%.0f) \\ \\chi_\\mathrm{red}^2=%.1f$}", b1, b1_err*1e3/FIT_STDFIT, FIT_STDFIT**2) at graph 0.20,0.17 left

set arrow from xmin,2.4 to xmax,2.4 nohead
set arrow from xmin,2.4 to xmin,2.4-0.1 nohead
set arrow from xmax,2.4 to xmax,2.4-0.1 nohead
set label 4 at xmin,2.4+0.15 "RNG fit"


xmin=4.001
xmax=5

fit [xmin:xmax]f2(x) "< paste data/GG_deg_L32.dat data/GG_Tc.dat" u 2:5:6 via a2,b2
#~ set label 2 sprintf("\\shortstack[r]{$b_{GG} = %.3f(%.0f) \\ \\chi_\\mathrm{red}^2=%.1f$}", b2, b2_err*1e3/FIT_STDFIT, FIT_STDFIT**2) at graph 0.20,0.07 left

set arrow from xmin,2 to xmax,2 nohead
set arrow from xmin,2 to xmin,2+0.1 nohead
set arrow from xmax,2 to xmax,2+0.1 nohead
set label 5 at xmin,2-0.15 "GG fit"


plot "< paste data/RNG_deg_L32.dat data/RNG_Tc.dat" u 2:5:3:6 w xyerrorbars pt 4 not, f1(x) lt 3 lc 1 notitle,\
     "< paste data/GG_deg_L32.dat data/GG_Tc.dat" u 2:5:3:6 w xyerrorbars pt 6 lt 1 lc 3 not, f2(x) lt 3 lc 3 notitle
