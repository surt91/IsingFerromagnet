set terminal epslatex standalone size 8.2cm,6cm font 10
set output 'Binder_over_Correlation_All.tex'

set size nosquare
set lmargin at screen 0.15
set rmargin at screen 0.98
set bmargin at screen 0.18
set tmargin at screen 0.96

set xlabel "$\\xi / L$"
set ylabel "$g$" offset 1.5,0

set xr [0:6]
set yr [0.94:1.005]

set ytics 0.01

set key samplen 1. bottom right

set format y "$%.2f$"
set format x "$%.0f$"

plot 'data/GG_Binder_over_Correlation_Length_L_16.dat'   using 1: 2: 3: 4 w xyerrorbar pt  4 ps 0.5 lt 1 lc rgb "#009999" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_16.dat'   using 5: 6: 7: 8 w xyerrorbar pt  4 ps 0.5 lt 1 lc rgb "#00AAFF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_16.dat'   using 9:10:11:12 w xyerrorbar pt  4 ps 0.5 lt 1 lc rgb "#0000FF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_32.dat'   using 1: 2: 3: 4 w xyerrorbar pt 12 ps 0.5 lt 1 lc rgb "#009999" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_32.dat'   using 5: 6: 7: 8 w xyerrorbar pt 12 ps 0.5 lt 1 lc rgb "#00AAFF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_32.dat'   using 9:10:11:12 w xyerrorbar pt 12 ps 0.5 lt 1 lc rgb "#0000FF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_64.dat'   using 1: 2: 3: 4 w xyerrorbar pt  8 ps 0.5 lt 1 lc rgb "#009999" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_64.dat'   using 5: 6: 7: 8 w xyerrorbar pt  8 ps 0.5 lt 1 lc rgb "#00AAFF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_64.dat'   using 9:10:11:12 w xyerrorbar pt  8 ps 0.5 lt 1 lc rgb "#0000FF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_128.dat'  using 1: 2: 3: 4 w xyerrorbar pt 10 ps 0.5 lt 1 lc rgb "#009999" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_128.dat'  using 5: 6: 7: 8 w xyerrorbar pt 10 ps 0.5 lt 1 lc rgb "#00AAFF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_128.dat'  using 9:10:11:12 w xyerrorbar pt 10 ps 0.5 lt 1 lc rgb "#0000FF" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 1: 2: 3: 4 w xyerrorbar pt  4 ps 0.5 lt 1 lc rgb "#999900" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 5: 6: 7: 8 w xyerrorbar pt  4 ps 0.5 lt 1 lc rgb "#FFAA00" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 9:10:11:12 w xyerrorbar pt  4 ps 0.5 lt 1 lc rgb "#FF0000" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 1: 2: 3: 4 w xyerrorbar pt 12 ps 0.5 lt 1 lc rgb "#999900" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 5: 6: 7: 8 w xyerrorbar pt 12 ps 0.5 lt 1 lc rgb "#FFAA00" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 9:10:11:12 w xyerrorbar pt 12 ps 0.5 lt 1 lc rgb "#FF0000" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 1: 2: 3: 4 w xyerrorbar pt  8 ps 0.5 lt 1 lc rgb "#999900" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 5: 6: 7: 8 w xyerrorbar pt  8 ps 0.5 lt 1 lc rgb "#FFAA00" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 9:10:11:12 w xyerrorbar pt  8 ps 0.5 lt 1 lc rgb "#FF0000" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 1: 2: 3: 4 w xyerrorbar pt 10 ps 0.5 lt 1 lc rgb "#999900" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 5: 6: 7: 8 w xyerrorbar pt 10 ps 0.5 lt 1 lc rgb "#FFAA00" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 9:10:11:12 w xyerrorbar pt 10 ps 0.5 lt 1 lc rgb "#FF0000" notitle, \
     'data/dummy.dat' w p pt  4 ps 1 lt 1 lc rgb "#000000" title "$L = \\phantom{1}16$", \
     'data/dummy.dat' w p pt 12 ps 1 lt 1 lc rgb "#000000" title "$L = \\phantom{1}32$", \
     'data/dummy.dat' w p pt  8 ps 1 lt 1 lc rgb "#000000" title "$L = \\phantom{1}64$", \
     'data/dummy.dat' w p pt 10 ps 1 lt 1 lc rgb "#000000" title "$L = 128$", \
     'data/dummy.dat' w p pt  6 ps 1 lt 1 lc rgb "#009999" title "$\\mathrm{GG}, \\sigma = 0.0$", \
     'data/dummy.dat' w p pt  6 ps 1 lt 1 lc rgb "#00AAFF" title "$\\mathrm{GG}, \\sigma = 0.5$", \
     'data/dummy.dat' w p pt  6 ps 1 lt 1 lc rgb "#0000FF" title "$\\mathrm{GG}, \\sigma = 1.0$", \
     'data/dummy.dat' w p pt  6 ps 1 lt 1 lc rgb "#999900" title "$\\mathrm{RNG}, \\sigma = 0.0$", \
     'data/dummy.dat' w p pt  6 ps 1 lt 1 lc rgb "#FFAA00" title "$\\mathrm{RNG}, \\sigma = 0.5$", \
     'data/dummy.dat' w p pt  6 ps 1 lt 1 lc rgb "#FF0000" title "$\\mathrm{RNG}, \\sigma = 1.0$"
