set terminal png
set output '../data/data_T_2.29_L_32.png'
plot '../data/data_T_2.29_L_32_up.dat' using 1:3 w l, '../data/data_T_2.29_L_32_rand.dat' using 1:3 w l;
