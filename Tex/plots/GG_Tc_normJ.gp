set terminal postscript eps enhanced color blacktext lw 2 26
set output 'GG_Tc_normJ.eps'

set xlabel '{/Italic {/Symbol s}}'
set ylabel '{/Italic T / {/Symbol \341}{/Symbol S}_{{/Symbol \341}i,j{/Symbol \361}} J_{ij}{/Symbol \361}}'

set xr [-0.1:1.3]
set yr [0.5:0.6]
set size square

plot "< paste data/GG_Tc.dat data/GG_sumJ_L32.dat" u 1:($2/$5):($3/$5) w ye ps 2 pt 4 title "{/Italic T_c / {/Symbol \341}{/Symbol S}_{{/Symbol \341}i,j{/Symbol \361}} J_{ij}{/Symbol \361}}",\
     "" u 1:($2/$5):($3/$5) w l lt 3 lc 1 notitle smooth csplines
