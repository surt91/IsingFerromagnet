set terminal epslatex standalone size 5.6cm,5.6cm font 10
set output "binder.tex"

set size nosquare
set lmargin at screen 0.18
set rmargin at screen 0.98
set bmargin at screen 0.18
set tmargin at screen 0.96

#set size square

set key samplen 1. left bottom Left reverse opaque

set xlabel "$T$"
set ylabel "$g$" offset 2.2,0

set xtics 0.1
set xtics add ("$T_c$" 2.2691)
set xr [2.2:2.4]
set yr [0:1]

set format xy "$%.1f$"

#set size square
set arrow from 2.2691,0 to 2.2691,1 nohead

plot "data/binderFit16_s_0.000.dat" u 1:2 w l lt 2 lc 1 notitle,\
     "data/binderFit32_s_0.000.dat" u 1:2 w l lt 2 lc 2 notitle,\
     "data/binderFit64_s_0.000.dat" u 1:2 w l lt 2 lc 3 notitle,\
     "data/binderFit128_s_0.000.dat" u 1:2 w l lt 2 lc 4 notitle,\
     "data/binder16_s_0.000.dat"  u 1:2:3 w ye lt 1 pt  4 lc 1 title "$\\phantom{1}16$",\
     "data/binder32_s_0.000.dat"  u 1:2:3 w ye lt 1 pt  6 lc 2 title "$\\phantom{1}32$",\
     "data/binder64_s_0.000.dat"  u 1:2:3 w ye lt 1 pt  8 lc 3 title "$\\phantom{1}64$",\
     "data/binder128_s_0.000.dat" u 1:2:3 w ye lt 1 pt 10 lc 4 title "$128$"
