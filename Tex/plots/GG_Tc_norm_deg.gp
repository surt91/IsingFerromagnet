set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'GG_Tc_norm_deg.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T / K}'

set xr [-0.1:1.3]
set size square

plot "< paste data/GG_Tc.dat data/GG_deg_L32.dat" u 1:($2/$5):($3/$5) w ye ps 2 pt 4 title "{/Italic T_c}"
