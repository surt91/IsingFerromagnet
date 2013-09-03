set terminal postscript eps enhanced color blacktext lw 2 26
set output 'Mean_M_L_128.eps'

set xlabel '{/Italic T}'
set ylabel '{/Italic {/Symbol \341}|m|{/Symbol \361}}'
set xrange [0.5:3.0]

set size square

unset key

set arrow from 2.75,0.6 to 0.75,0.4
set label "{/Symbol s}" at 2.5,0.65

plot 'data/Mean_M_L_128.dat' using 1:2:3   w yerrorbar title '{/Symbol s} = 0.00', \
     'data/Mean_M_L_128.dat' using 1:4:5   w yerrorbar title '{/Symbol s} = 0.03', \
     'data/Mean_M_L_128.dat' using 1:6:7   w yerrorbar title '{/Symbol s} = 0.05', \
     'data/Mean_M_L_128.dat' using 1:8:9   w yerrorbar title '{/Symbol s} = 0.08', \
     'data/Mean_M_L_128.dat' using 1:10:11 w yerrorbar title '{/Symbol s} = 0.10', \
     'data/Mean_M_L_128.dat' using 1:12:13 w yerrorbar title '{/Symbol s} = 0.12', \
     'data/Mean_M_L_128.dat' using 1:14:15 w yerrorbar title '{/Symbol s} = 0.15', \
     'data/Mean_M_L_128.dat' using 1:16:17 w yerrorbar title '{/Symbol s} = 0.20', \
     'data/Mean_M_L_128.dat' using 1:18:19 w yerrorbar title '{/Symbol s} = 0.25', \
     'data/Mean_M_L_128.dat' using 1:20:21 w yerrorbar title '{/Symbol s} = 0.30', \
     'data/Mean_M_L_128.dat' using 1:22:23 w yerrorbar title '{/Symbol s} = 0.40', \
     'data/Mean_M_L_128.dat' using 1:24:25 w yerrorbar title '{/Symbol s} = 0.50', \
     'data/Mean_M_L_128.dat' using 1:26:27 w yerrorbar title '{/Symbol s} = 0.60', \
     'data/Mean_M_L_128.dat' using 1:28:29 w yerrorbar title '{/Symbol s} = 0.70', \
     'data/Mean_M_L_128.dat' using 1:30:31 w yerrorbar title '{/Symbol s} = 0.80', \
     'data/Mean_M_L_128.dat' using 1:32:33 w yerrorbar title '{/Symbol s} = 0.90', \
     'data/Mean_M_L_128.dat' using 1:34:35 w yerrorbar title '{/Symbol s} = 1.00', \
     'data/Mean_M_L_128.dat' using 1:36:37 w yerrorbar title '{/Symbol s} = 1.10', \
     'data/Mean_M_L_128.dat' using 1:38:39 w yerrorbar title '{/Symbol s} = 1.20', \
     \
     'data/Mean_M_L_128.dat' using 1:2:3   w l lt 3 lc  1 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:4:5   w l lt 3 lc  2 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:6:7   w l lt 3 lc  3 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:8:9   w l lt 3 lc  4 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:10:11 w l lt 3 lc  5 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:12:13 w l lt 3 lc  6 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:14:15 w l lt 3 lc  7 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:16:17 w l lt 3 lc  8 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:18:19 w l lt 3 lc  9 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:20:21 w l lt 3 lc 10 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:22:23 w l lt 3 lc 11 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:24:25 w l lt 3 lc 12 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:26:27 w l lt 3 lc 13 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:28:29 w l lt 3 lc 14 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:30:31 w l lt 3 lc 15 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:32:33 w l lt 3 lc 16 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:34:35 w l lt 3 lc 17 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:36:37 w l lt 3 lc 18 notitle smooth csplines, \
     'data/Mean_M_L_128.dat' using 1:38:39 w l lt 3 lc 19 notitle smooth csplines
