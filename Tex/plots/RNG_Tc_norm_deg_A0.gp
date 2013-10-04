set terminal postscript eps enhanced color blacktext lw 2 26
set output 'RNG_Tc_norm_deg_A0.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T / K}'

set xr [-0.1:1.3]
#set yr [0.42:0.58]
set size square

set key title 'RNG' samplen 1 invert

#set logscale xy

plot "< paste data/RNG_Tc.dat data/RNG_sumJ_L32.dat" u 1:($2/$5):($3/$5+$2/$5**2*$6) w ye lt 1 lc 5 ps 2 pt 8 title "{/Italic T_c / {/Symbol \341}{/Symbol S}_{{/Symbol \341}i,j{/Symbol \361}} J_{ij}{/Symbol \361}}",\
     "" u 1:($2/$5) w l lt 3 lc 5 notitle smooth csplines,\
     "< paste data/RNG_Tc_A0.dat data/RNG_deg_L32_A0.dat" u 1:($2/$5):($3/$5+$2/$5**2*$6) w ye lt 1 lc 1 ps 2 pt 4 title "{/Italic T_c / K}",\
     "" u 1:($2/$5) w l lt 3 lc 1 notitle smooth csplines
