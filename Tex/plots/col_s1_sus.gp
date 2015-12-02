set terminal epslatex standalone size 7.8cm,7.8cm font 10
set output 'col_s1_sus.tex'

set size nosquare
set lmargin at screen 0.18
set rmargin at screen 0.98
set bmargin at screen 0.18
set tmargin at screen 0.96

set multiplot

set key samplen 1. at 8,0.02
set xl "$(T-T_c) L^{1/\\nu}$"
set yl "$\\chi L^{-\\gamma / \\nu}$" offset 1.5,0
xc=1.2125
a=1/1.010
b=1.764/a                                         # set scaling parameters
#~ set label 1 sprintf("{/Italic T_c}= %.3f\n{/Italic {/Symbol n}} = %.3f\n{/Italic {/Symbol g}} = %.3f", xc, 1/a, b*a) at graph 0.95,0.8 right # list scaling parameters
sx(x,L)=(x-xc)*L**a
sy(y,L)=y*L**-b                 # def scaling assumption
set xr [-5:20]

set format y "$%.2f$"

#set size square

#~ p "data/ScalanaSusceptibility_S_1.00_L_16.dat"  u (sx($1,16)):(sy($2,16)):(sy($3,16))    w ye lt 1 lc 1 pt  4 t "$L = \\phantom{1}16$"
p "data/ScalanaSusceptibility_S_1.00_L_32.dat"  u (sx($1,32)):(sy($2,32)):(sy($3,32))    w ye lt 1 lc 2 pt  6 t "$L = \\phantom{1}32$" \
, "data/ScalanaSusceptibility_S_1.00_L_64.dat"  u (sx($1,64)):(sy($2,64)):(sy($3,64))    w ye lt 1 lc 3 pt  8 t "$L = \\phantom{1}64$" \
, "data/ScalanaSusceptibility_S_1.00_L_128.dat" u (sx($1,128)):(sy($2,128)):(sy($3,128)) w ye lt 1 lc 4 pt 10 t "$L = 128$"

set origin 0.45,0.41
set size 0.53,0.55
unset lmargin
unset bmargin

set object 1 rectangle from graph 0,0 to graph 1,1 behind fc rgb "#ffffff"

unset key # samplen 1. top right

set xl "$T$" offset 0,0
set yl "$\\chi$"

set format x ""
set format y ""

set xr [1.15:1.35]

p "data/ScalanaSusceptibility_S_1.00_L_128.dat" u 1:2:3  w ye lt 1 lc 4 pt 10 t "$128$" \
, "data/ScalanaSusceptibility_S_1.00_L_64.dat"  u 1:2:3  w ye lt 1 lc 3 pt  8 t "$\\phantom{1}64$" \
, "data/ScalanaSusceptibility_S_1.00_L_32.dat"  u 1:2:3  w ye lt 1 lc 2 pt  6 t "$\\phantom{1}32$" \
#~ , "data/ScalanaSusceptibility_S_1.00_L_16.dat"  u 1:2:3  w ye lt 1 lc 1 pt  4 t "$L = \\phantom{1}16$"

unset multiplot
