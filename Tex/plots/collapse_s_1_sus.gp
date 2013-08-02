set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'collapse_s_1_sus.eps'

set key samplen 1. center bottom Left                 # customize key
set xl "{/Italic (T-T_c) L^{1/{/Symbol n}}}"
set yl "{/Italic {/Symbol c}} L^{-{/Symbol g}/{/Symbol n}}"  # set x,y labels
xc=1.2123
a=1/1.013
b=1.758/a                                         # set scaling parameters
set label 1 sprintf("{/Italic T_c}= %.3f\n{/Italic {/Symbol n}} = %.3f\n{/Italic {/Symbol g}} = %.3f", xc, 1/a, b*a) at graph 0.95,0.8 right # list scaling parameters
sx(x,L)=(x-xc)*L**a; sy(y,L)=y*L**-b                 # def scaling assumption
set xr [-5:15]

set size square

p "data/ScalanaSusceptibility_S_1.00_L_32.dat"  u (sx($1,32)):(sy($2,32)):(sy($3,32))  w ye lc 2 ps 2 pt 6 t "{/Italic L} = 32" \
, "data/ScalanaSusceptibility_S_1.00_L_64.dat"  u (sx($1,64)):(sy($2,64)):(sy($3,64))  w ye lc 3 ps 2 pt 8 t "{/Italic L} = 64" \
, "data/ScalanaSusceptibility_S_1.00_L_128.dat"  u (sx($1,128)):(sy($2,128)):(sy($3,128))  w ye lc 4 ps 2 pt 10 t "{/Italic L} = 128"
