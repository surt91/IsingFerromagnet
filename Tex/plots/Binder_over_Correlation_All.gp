set terminal postscript eps enhanced color blacktext lw 1 32
set output 'Binder_over_Correlation_All.eps'

set size 2,2

set multiplot

set xlabel '{/Italic {/Symbol x}/L}'
set ylabel '{/Italic g}'

set xr [0:10]
set yr [0.92:1.005]

set ytics 0.01

set key samplen 1. Left reverse at 4,0.96

set arrow from 1.5,0.97 to 1.5,1.00 lw 1 front nohead
set arrow from 4,0.97 to 4,1.00 lw 1 front nohead
set arrow from 1.5,0.97 to 4,0.97 lw 1 front nohead
set arrow from 1.5,1.00 to 4,1.00 lw 1 front nohead

set format y "%.2f"

plot 'data/GG_Binder_over_Correlation_Length_L_16.dat'   using 1: 2: 3: 4 w xyerrorbar pt  4 ps 1 lt 1 lc rgb "#009999" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_16.dat'   using 5: 6: 7: 8 w xyerrorbar pt  4 ps 1 lt 1 lc rgb "#00AAFF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_16.dat'   using 9:10:11:12 w xyerrorbar pt  4 ps 1 lt 1 lc rgb "#0000FF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_32.dat'   using 1: 2: 3: 4 w xyerrorbar pt 12 ps 1 lt 1 lc rgb "#009999" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_32.dat'   using 5: 6: 7: 8 w xyerrorbar pt 12 ps 1 lt 1 lc rgb "#00AAFF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_32.dat'   using 9:10:11:12 w xyerrorbar pt 12 ps 1 lt 1 lc rgb "#0000FF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_64.dat'   using 1: 2: 3: 4 w xyerrorbar pt  8 ps 1 lt 1 lc rgb "#009999" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_64.dat'   using 5: 6: 7: 8 w xyerrorbar pt  8 ps 1 lt 1 lc rgb "#00AAFF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_64.dat'   using 9:10:11:12 w xyerrorbar pt  8 ps 1 lt 1 lc rgb "#0000FF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_128.dat'  using 1: 2: 3: 4 w xyerrorbar pt 10 ps 1 lt 1 lc rgb "#009999" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_128.dat'  using 5: 6: 7: 8 w xyerrorbar pt 10 ps 1 lt 1 lc rgb "#00AAFF" notitle, \
     'data/GG_Binder_over_Correlation_Length_L_128.dat'  using 9:10:11:12 w xyerrorbar pt 10 ps 1 lt 1 lc rgb "#0000FF" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 1: 2: 3: 4 w xyerrorbar pt  4 ps 1 lt 1 lc rgb "#999900" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 5: 6: 7: 8 w xyerrorbar pt  4 ps 1 lt 1 lc rgb "#FFAA00" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 9:10:11:12 w xyerrorbar pt  4 ps 1 lt 1 lc rgb "#FF0000" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 1: 2: 3: 4 w xyerrorbar pt 12 ps 1 lt 1 lc rgb "#999900" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 5: 6: 7: 8 w xyerrorbar pt 12 ps 1 lt 1 lc rgb "#FFAA00" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 9:10:11:12 w xyerrorbar pt 12 ps 1 lt 1 lc rgb "#FF0000" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 1: 2: 3: 4 w xyerrorbar pt  8 ps 1 lt 1 lc rgb "#999900" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 5: 6: 7: 8 w xyerrorbar pt  8 ps 1 lt 1 lc rgb "#FFAA00" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 9:10:11:12 w xyerrorbar pt  8 ps 1 lt 1 lc rgb "#FF0000" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 1: 2: 3: 4 w xyerrorbar pt 10 ps 1 lt 1 lc rgb "#999900" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 5: 6: 7: 8 w xyerrorbar pt 10 ps 1 lt 1 lc rgb "#FFAA00" notitle, \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 9:10:11:12 w xyerrorbar pt 10 ps 1 lt 1 lc rgb "#FF0000" notitle, \
     'data/dummy.dat' w xyerrorbar pt  4 ps 1 lt 1 lc rgb "#000000" title "{/Italic L} = 16", \
     'data/dummy.dat' w xyerrorbar pt 12 ps 1 lt 1 lc rgb "#000000" title "{/Italic L} = 32", \
     'data/dummy.dat' w xyerrorbar pt  8 ps 1 lt 1 lc rgb "#000000" title "{/Italic L} = 64", \
     'data/dummy.dat' w xyerrorbar pt 10 ps 1 lt 1 lc rgb "#000000" title "{/Italic L} = 128", \
     'data/dummy.dat' w xyerrorbar pt  6 ps 1 lt 1 lc rgb "#009999" title "GG, {/Symbol s} = 0.0", \
     'data/dummy.dat' w xyerrorbar pt  6 ps 1 lt 1 lc rgb "#00AAFF" title "GG, {/Symbol s} = 0.5", \
     'data/dummy.dat' w xyerrorbar pt  6 ps 1 lt 1 lc rgb "#0000FF" title "GG, {/Symbol s} = 1.0", \
     'data/dummy.dat' w xyerrorbar pt  6 ps 1 lt 1 lc rgb "#999900" title "RNG, {/Symbol s} = 0.0", \
     'data/dummy.dat' w xyerrorbar pt  6 ps 1 lt 1 lc rgb "#FFAA00" title "RNG, {/Symbol s} = 0.5", \
     'data/dummy.dat' w xyerrorbar pt  6 ps 1 lt 1 lc rgb "#FF0000" title "RNG, {/Symbol s} = 1.0"

set size 1.10,1.5
set origin 0.85,0.2

unset xl
unset yl
set xr [1.5:4]
set yr [0.97:1.00]
set format x "%.1f"

unset key
unset arrow

replot

unset multiplot
