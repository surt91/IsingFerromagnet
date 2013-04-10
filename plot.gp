set terminal postscript eps color enhanced lw 2
set output 'data/plot_1.00.eps'

plot 'data/data_T_1.00_L_128_rand.dat' using 1:3 w l, "data/data_T_1.00_L_128_up.dat" using 1:3 w l;

set terminal png
set output 'data/plot_T_1.00_L_128.png'

plot 'data/data_T_1.00_L_128_rand.dat' using 1:3 w l, "data/data_T_1.00_L_128_up.dat" using 1:3 w l;
