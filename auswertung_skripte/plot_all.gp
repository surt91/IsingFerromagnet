set terminal png
set output 'plot_binder.png'

plot 'binder.dat' using 1:2 w p, 'binder.dat' using 1:3 w p, 'binder.dat' using 1:4 w p

set terminal png
set output 'plot_mean.png'

plot 'mean.dat' using 1:2 w p, 'mean.dat' using 1:3 w p, 'mean.dat' using 1:4 w p

set terminal png
set output 'plot_var.png'

plot 'var.dat' using 1:2 w p, 'var.dat' using 1:3 w p, 'var.dat' using 1:4 w p
