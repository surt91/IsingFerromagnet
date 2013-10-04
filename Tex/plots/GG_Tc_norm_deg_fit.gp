set terminal postscript eps enhanced color blacktext lw 2 26
set output 'GG_Tc_norm_deg_fit.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T / K^{1.30}}'

set xr [-0.1:1.3]
set yr [0.345:0.352]
set size square

set key title 'GG' samplen 1
set key spacing 5

plot "< paste data/GG_Tc.dat data/GG_deg_L32.dat" u 1:($2/$5**1.3):($3/$5**1.3+1.3*$2/$5**2.3*$6) w ye ps 2 pt 6 lc 3 title "{/Italic T_c / K^{1.30}}",\
     "" u 1:($2/$5**1.3) w l lt 3 lc 3 notitle smooth csplines
