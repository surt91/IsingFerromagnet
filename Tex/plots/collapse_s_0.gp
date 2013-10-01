set terminal postscript eps enhanced color blacktext lw 2 26
set output 'collapse_s_0.eps'

set key samplen 1. left bottom Left                 # customize key
set xl "{/Italic (T-T_c) L^{1/{/Symbol n}}}"
set yl "{/Italic g}"                                # set x,y labels
xc=2.2688
a=1/0.997
b=0                                                 # set scaling parameters
set label 1 sprintf("{/Italic T_c}= %.3f\n{/Italic {/Symbol n}} = %.3f", xc, 1/a) at graph 0.95,0.8 right # list scaling parameters
sx(x,L)=(x-xc)*L**a; sy(y,L)=y*L**b                 # def scaling assumption
set xr [-5:15]

set size square

p "data/ScalanaBinder_S_0.00_L_32.dat"  u (sx($1,32)):(sy($2,32)):(sy($3,32))    w ye lt 1 lc 2 ps 2 pt 6 t "{/Italic L} = 32" \
, "data/ScalanaBinder_S_0.00_L_64.dat"  u (sx($1,64)):(sy($2,64)):(sy($3,64))    w ye lt 1 lc 3 ps 2 pt 8 t "{/Italic L} = 64" \
, "data/ScalanaBinder_S_0.00_L_128.dat" u (sx($1,128)):(sy($2,128)):(sy($3,128)) w ye lt 1 lc 4 ps 2 pt 10 t "{/Italic L} = 128" \
, "data/ScalanaBinder_S_0.00_L_32.dat"  u (sx($1,32)):(sy($2,32)):(sy($3,32))    w l  lt 3 lc 2 notitle smooth csplines \
, "data/ScalanaBinder_S_0.00_L_64.dat"  u (sx($1,64)):(sy($2,64)):(sy($3,64))    w l  lt 3 lc 3 notitle smooth csplines \
, "data/ScalanaBinder_S_0.00_L_128.dat" u (sx($1,128)):(sy($2,128)):(sy($3,128)) w l  lt 3 lc 4 notitle smooth csplines
