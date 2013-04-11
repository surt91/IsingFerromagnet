#set terminal postscript eps color enhanced lw 2
#set output 'data/plot_1.00.eps'

set terminal png
set output 'plot_binder.png'

plot 'binder.dat' using 1:2 w p, 'binder.dat' using 1:3 w p, 'binder.dat' using 1:4 w p
