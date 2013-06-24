set terminal postscript eps enhanced color blacktext lw 2 26
set output 'meanM.eps'

set xlabel '{/Italic T}'
set ylabel '{/Italic <|m|>}'
set xrange [1.5:3.2]

set size square

set key right

plot 'data/Mean_M_0_00.dat' using 1:2 w p ps 2 lt 1 lc 1 pt 1 title '{/Italic L} = 16',\
     'data/Mean_M_0_00.dat' using 1:2 w l lt 3 lc 1 notitle smooth csplines,\
     'data/Mean_M_0_00.dat' using 1:4 w p ps 2 lt 1 lc 2 pt 2 title '{/Italic L} = 32',\
     'data/Mean_M_0_00.dat' using 1:4 w l lt 3 lc 2 notitle smooth csplines,\
     'data/Mean_M_0_00.dat' using 1:6 w p ps 2 lt 1 lc 3 pt 3 title '{/Italic L} = 64',\
     'data/Mean_M_0_00.dat' using 1:6 w l lt 3 lc 3 notitle smooth csplines,\
     'data/Mean_M_0_00.dat' using 1:8 w p ps 2 lt 1 lc 4 pt 4 title '{/Italic L} = 128',\
     'data/Mean_M_0_00.dat' using 1:8 w l lt 3 lc 4 notitle smooth csplines,\
     'data/Mean_M_0_00.dat' using 1:($1<2.269 ? $8 : 0) w l lt 1 lc 7 lw 3 title '{/Italic L} {/Symbol \256 \245}'
