set terminal postscript eps enhanced color blacktext lw 2 26
set output 'RNG_Tc_norm_deg_fit.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T / K^{1.38}}'

set xr [-0.1:1.3]
set size square

set format x "%.1f"
set format y "%.3f"

set key title 'RNG' samplen 1
set key spacing 5

plot "< paste data/RNG_Tc.dat data/RNG_deg_L32.dat" u 1:($2/$5**1.38):($3/$5**1.3+1.3*$2/$5**2.3*$6) w ye lt 1 lc 1 ps 2 pt 4 title "{/Italic T_c / K^{1.38}}",\
     "" u 1:($2/$5**1.38):($3/$5) w l lt 3 lc 1 notitle smooth csplines
