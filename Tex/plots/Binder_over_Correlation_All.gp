set terminal postscript eps enhanced color blacktext lw 2 26
set output 'Binder_over_Correlation_All.eps'

set key samplen 1. right bottom
set xlabel '{/Italic {/Symbol x}/L}'
set ylabel '{/Italic g}'

set size square

set xr [0:10]
set yr [0.92:1.01]

plot 'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 1: 2: 3: 4 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#FF0000" title '{/Symbol s} = 0.0, {/Italic L} = 16', \
     'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 5: 6: 7: 8 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#FF7700" title '{/Symbol s} = 0.5, {/Italic L} = 16', \
     'data/RNG_Binder_over_Correlation_Length_L_16.dat'  using 9:10:11:12 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#FF5555" title '{/Symbol s} = 1.0, {/Italic L} = 16', \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 1: 2: 3: 4 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#CC0000" title '{/Symbol s} = 0.0, {/Italic L} = 32', \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 5: 6: 7: 8 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#CC7700" title '{/Symbol s} = 0.5, {/Italic L} = 32', \
     'data/RNG_Binder_over_Correlation_Length_L_32.dat'  using 9:10:11:12 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#CC5555" title '{/Symbol s} = 1.0, {/Italic L} = 32', \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 1: 2: 3: 4 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#AA0000" title '{/Symbol s} = 0.0, {/Italic L} = 64', \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 5: 6: 7: 8 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#AA7700" title '{/Symbol s} = 0.5, {/Italic L} = 64', \
     'data/RNG_Binder_over_Correlation_Length_L_64.dat'  using 9:10:11:12 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#AA5555" title '{/Symbol s} = 1.0, {/Italic L} = 64', \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 1: 2: 3: 4 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#990000" title '{/Symbol s} = 0.0, {/Italic L} = 128', \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 5: 6: 7: 8 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#997700" title '{/Symbol s} = 0.5, {/Italic L} = 128', \
     'data/RNG_Binder_over_Correlation_Length_L_128.dat' using 9:10:11:12 w xyerrorbar pt 6 ps 1 lt 1 lc rgb "#995555" title '{/Symbol s} = 1.0, {/Italic L} = 128'
