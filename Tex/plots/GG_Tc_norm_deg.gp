set terminal postscript eps enhanced color blacktext lw 2 26
set output 'GG_Tc_norm_deg.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T / K}'

set xr [-0.1:1.3]
set size square
set format x "%.1f"
set format y "%.2f"

set key title 'GG' samplen 1

plot "< paste data/GG_Tc.dat data/GG_deg_L32.dat" u 1:($2/$5):($3/$5+$2/$5**2*$6) w ye ps 2 pt 6 lc 3 title "{/Italic T_c / K}",\
     "" u 1:($2/$5) w l lt 3 lc 3 notitle smooth csplines
