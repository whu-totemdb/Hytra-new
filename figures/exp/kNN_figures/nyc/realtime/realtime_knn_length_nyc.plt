alg_c=9
x_base = 3
x_tic_base = 4.5
xtic_width = 9
dfile="realtime_knn_length_nyc.dat"
epsfile="realtime_knn_length_nyc.eps"
box_width = 3
height=1

set term postscript enhanced eps 32
set output epsfile
set datafile separator ","

set tmargin 1
set rmargin 1
set lmargin 5.2
# set bmargin 3

# set style histogram rowstacked

set xrange [0:45]
set yrange [1:120]  noreverse nowriteback

#set ytics offset 0.8,0 font "Calibri, 32" offset 0.5,0,0 
set ytics font 'Calibri, 30'

#set xtics scale 0 offset 0, 0.5 font "Calibri, 32" ( "Read Heavy" x_tic_base,  "Write Heavy " x_tic_base+xtic_width, "Write Only" x_tic_base+2*xtic_width)
set xtics font 'Calibri, 30' ( "5" x_tic_base,  "10" x_tic_base+xtic_width, "15" x_tic_base+2*xtic_width, "20" x_tic_base+3*xtic_width, "25" x_tic_base+4*xtic_width)

#set ylabel offset 2, 0 "CPU Cost (s)"   font  "Calibri-Bold, 34" 
set ylabel offset 1.1, 0 "Time Cost (ms)"   font  'Calibri,33'
set logscale y
set format y "10^{%L}"

#set xlabel  offset 0, 1 "Workload Type"   font  "Calibri-Bold, 34" 
set xlabel offset 0, 0.5 "Time Window (min)" font  'Calibri, 33''

# set key  top left horizontal   font 'Calibri,30' spacing 0.8 Left
# set size 3, 1
#set key  horizontal   font 'Calibri,30' spacing 0.8 Right
#set key horizontal font 'Calibri,29' spacing 0.8 Right
set key top left horizontal font  'Calibri,29' spacing 1 maxrow 1

#set key at 45, 3400

set size 1.1, 1.0
set key  vertical maxrows 1

set boxwidth 3

plot dfile using (($1)-1)*alg_c+x_base:(($3))  title "Ours"   with boxes fill pattern 3 lt 1 lw 0.5,\
     dfile using (($1)-1)*alg_c+x_base+box_width:(($4))  title "Torch" with boxes fill pattern 4 lt 1 lw 0.5