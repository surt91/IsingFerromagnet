set terminal epslatex standalone size 5.6cm,5.6cm font 10
set output "Tc_both.tex"

set size nosquare
set lmargin at screen 0.18
set rmargin at screen 0.98
set bmargin at screen 0.18
set tmargin at screen 0.96

set xlabel "$\\sigma$"
set ylabel "$T_c$" offset 2.2,1
set xr [-0.1:1.3]
set yr [1.1:3.0]

set xtics 0.4
set ytics 0.4

set format xy "$%.1f$"

#set size square

set key samplen 1

plot 'data/RNG_Tc.dat' w ye lt 1 lc 1 pt 4 title "RNG",\
     'data/GG_Tc.dat' w ye pt 6 lc 3 title "GG"
