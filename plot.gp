set terminal postscript eps color enhanced lw 2
set output 'data/plot_1.00.eps'

plot 'data/data_T_1.00_rand.dat' using 1:3 w l, "data/data_T_1.00_up.dat" using 1:3 w l;
