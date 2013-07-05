set terminal postscript eps enhanced color blacktext lw 2 solid 26
set output 'collapse_s_0.eps'

set key samplen 1. left bottom Left                 # customize key
set xl "{/Italic (T-T_c) L^{1/{/Symbol n}}}"
set yl "{/Italic y L}"    # set x,y labels
xc=2.267979
a=1.006524
b=0                                                 # set scaling parameters
set label 1 sprintf("T_c= %.3f\n{/Symbol n} = %.3f", xc, 1/a) at graph 0.95,0.8 right # list scaling parameters
sx(x,L)=(x-xc)*L**a; sy(y,L)=y*L**b                 # def scaling assumption
set xr [-5:15]

set size square

p "data/ScalanaBinder_S_0.00_L_16.dat"  u (sx($1,16)): (sy($2,16))  w p t "L=16" \
, "data/ScalanaBinder_S_0.00_L_32.dat"  u (sx($1,32)): (sy($2,32))  w p t "L=32" \
, "data/ScalanaBinder_S_0.00_L_64.dat"  u (sx($1,64)): (sy($2,64))  w p t "L=64" \
, "data/ScalanaBinder_S_0.00_L_128.dat"  u (sx($1,128)): (sy($2,128))  w p t "L=128"
