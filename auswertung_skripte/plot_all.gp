set terminal png
set output 'plot_binder.png'

plot 'binder.dat' using 1:2:3 w yerrorbar, \
     'binder.dat' using 1:4:5 w yerrorbar, \
     'binder.dat' using 1:6:7 w yerrorbar
