set terminal postscript eps enhanced color blacktext lw 2 26
set output 'Binder_over_Correlation_All.eps'

set key right bottom
set xlabel '{/Italic {/Symbol x}/L}'
set ylabel '{/Italic g}'

set size square

set xr [0:10]
set yr [0.9:1]

plot 'data/Binder_over_Correlation_Length_L_16.dat'  using 1: 2: 3: 4 w xyerrorbar title '{/Symbol s} = 0.0, {/Italic L} = 16', \
     'data/Binder_over_Correlation_Length_L_16.dat'  using 5: 6: 7: 8 w xyerrorbar title '{/Symbol s} = 0.5, {/Italic L} = 16', \
     'data/Binder_over_Correlation_Length_L_16.dat'  using 9:10:11:12 w xyerrorbar title '{/Symbol s} = 1.0, {/Italic L} = 16', \
     'data/Binder_over_Correlation_Length_L_32.dat'  using 1: 2: 3: 4 w xyerrorbar title '{/Symbol s} = 0.0, {/Italic L} = 32', \
     'data/Binder_over_Correlation_Length_L_32.dat'  using 5: 6: 7: 8 w xyerrorbar title '{/Symbol s} = 0.5, {/Italic L} = 32', \
     'data/Binder_over_Correlation_Length_L_32.dat'  using 9:10:11:12 w xyerrorbar title '{/Symbol s} = 1.0, {/Italic L} = 32', \
     'data/Binder_over_Correlation_Length_L_64.dat'  using 1: 2: 3: 4 w xyerrorbar title '{/Symbol s} = 0.0, {/Italic L} = 64', \
     'data/Binder_over_Correlation_Length_L_64.dat'  using 5: 6: 7: 8 w xyerrorbar title '{/Symbol s} = 0.5, {/Italic L} = 64', \
     'data/Binder_over_Correlation_Length_L_64.dat'  using 9:10:11:12 w xyerrorbar title '{/Symbol s} = 1.0, {/Italic L} = 64', \
     'data/Binder_over_Correlation_Length_L_128.dat' using 1: 2: 3: 4 w xyerrorbar title '{/Symbol s} = 0.0, {/Italic L} = 128', \
     'data/Binder_over_Correlation_Length_L_128.dat' using 5: 6: 7: 8 w xyerrorbar title '{/Symbol s} = 0.5, {/Italic L} = 128', \
     'data/Binder_over_Correlation_Length_L_128.dat' using 9:10:11:12 w xyerrorbar title '{/Symbol s} = 1.0, {/Italic L} = 128'
