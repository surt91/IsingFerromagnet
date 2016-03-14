set terminal epslatex standalone size 5.6cm,5.6cm font 10
set output 'K_sigma_inset_both.tex'

set multiplot

#set size nosquare

set lmargin at screen 0.18
set rmargin at screen 0.98
set bmargin at screen 0.18
set tmargin at screen 0.96

set xlabel "$\\sigma$"
set ylabel '$K$' offset 2.2,-0.7

set xr [-0.1:1.3]
set yr [2.5:7.1]

set xtics 0.4
set ytics 1

set format x "%.1f"
set format y "%.0f"

set key samplen 1 top left

plot "data/RNG_deg_L32.dat" w ye lt 1 lc 1 pt 4 title "RNG", \
     "data/GG_deg_L32.dat" w ye lt 1 lc 3 pt 6 title "GG"


set size nosquare
set lmargin at screen 0.58
set rmargin at screen 0.98
set bmargin at screen 0.58
set tmargin at screen 0.96

set object 1 rectangle from graph 0,0 to graph 1,1 behind fc rgb "#ffffff"

set xlabel '$\ln K$' offset -0.2,1.1
set ylabel '$\ln T_c$' offset 3.7,-0.6

set key title samplen 1. left top Left reverse

set xrange [3.9:5.25]
set yrange [2:3.4]

set format y "$%.0f$"
set format x "$%.0f$"

set xtics ('$\ln 4$' 4, '$\ln 5$' 5)
set ytics ('$\ln 2$' 2, '$\ln 3$' 3)

set log xy

#set size square

set fit errorvariables

#~ set label 3 "Fit to $aK^b$" at graph 0.65,0.27 left

a0=1; b0=1;
f0(x) = a0*x**b0
a2=1; b2=1;
f2(x) = a2*x**b2

xmin=4.001
xmax=6

fit [xmin:xmax]f2(x) "< paste data/GG_deg_L32.dat data/GG_Tc.dat" u 2:5:6 via a2,b2
fit [xmin:xmax]f0(x) "< paste data/GG_deg_L32_A0.dat data/GG_Tc_A0.dat" u 2:5:6 via a0,b0
#set label 2 sprintf("\\shortstack[r]{$b_{GG} = %.3f(%.0f) \\ \\chi_\\mathrm{red}^2=%.1f$}", b2, b2_err*1e3/FIT_STDFIT, FIT_STDFIT**2) at graph 0.15,0.07 left

#set arrow from xmin,2 to xmax,2 nohead
#set arrow from xmin,2 to xmin,2+0.1 nohead
#set arrow from xmax,2 to xmax,2+0.1 nohead
#set label 5 at xmin,2-0.15 "GG fit"

plot "< paste data/GG_deg_L32_A0.dat data/GG_Tc_A0.dat" u 2:5:3:6 w xyerrorbars pt 8 lt 1 lc 2 t "$J_{ij}=1$", f0(x) lt 1 lc 2 not, \
     "< paste data/GG_deg_L32.dat data/GG_Tc.dat" u 2:5:3:6 w xyerrorbars pt 10 lt 1 lc 4 not, f2(x) lt 1 lc 4 not

unset multiplot
