alg_c=9
x_base = 3
x_tic_base = 4.5
xtic_width = 9
dfile="realtime_range_window_syd_new.dat"
epsfile="realtime_range_window_syd_new.eps"
box_width = 3
height=1

set term postscript enhanced eps 32
set output epsfile
set datafile separator ","

set tmargin 1
set rmargin 1
set lmargin 5.2
set bmargin 2.8

# set style histogram rowstacked

set xrange [0:45]
set yrange [1:200]  noreverse nowriteback

#set ytics offset 0.8,0 font "Calibri, 32" offset 0.5,0,0 
set ytics font 'Calibri, 30'

#set xtics scale 0 offset 0, 0.5 font "Calibri, 32" ( "Read Heavy" x_tic_base,  "Write Heavy " x_tic_base+xtic_width, "Write Only" x_tic_base+2*xtic_width)
set xtics font 'Calibri, 31' ( "1{/Symbol \264}1" x_tic_base,  "2{/Symbol \264}2" x_tic_base+xtic_width, "3{/Symbol \264}3" x_tic_base+2*xtic_width, "4{/Symbol \264}4" x_tic_base+3*xtic_width, "5{/Symbol \264}5" x_tic_base+4*xtic_width)

#set ylabel offset 2, 0 "CPU Cost (s)"   font  "Calibri-Bold, 34" 
set ylabel offset 1.1, 0 "Time Cost (ms)"   font  'Calibri,33' 
set logscale y
set format y "10^{%L}"

#set xlabel  offset 0, 1 "Workload Type"   font  "Calibri-Bold, 34" 
set xlabel offset 0, 0.5 "Spatial Range (km^2)" font  'Calibri, 33'

# set key  top left horizontal   font 'Calibri,30' spacing 0.8 Left
# set size 3, 1
#set key  horizontal   font 'Calibri,30' spacing 0.8 Right
#set key horizontal font 'Calibri,29' spacing 0.8 Right
set key top left horizontal font  'Calibri,30' spacing 1 maxrow 1

#set key at 45, 3400

set size 1.1, 1.0
set key  vertical maxrows 1

set boxwidth 3

plot dfile using (($1)-1)*alg_c+x_base:(($2))  title "Ours"   with boxes fill pattern 3 lt 1 lw 0.5,\
     dfile using (($1)-1)*alg_c+x_base+box_width:(($3))  title "Dragoon" with boxes fill pattern 4 lt 1 lw 0.5