alg_c=12
x_base = 3
x_tic_base = 6
xtic_width = 12
dfile="historical_knn_datasize_nyc.dat"
epsfile="historical_knn_datasize_nyc.eps"
box_width = 3
height=1

set term postscript enhanced eps 32
set output epsfile
set datafile separator ","

set tmargin 1
set rmargin 1
set lmargin 5.2


set xrange [0:60]
set yrange [1:905000]  noreverse nowriteback

set ytics font 'Calibri, 30'

set xtics font 'Calibri, 30' ( "20" x_tic_base,  "40" x_tic_base+xtic_width, "60" x_tic_base+2*xtic_width, "80" x_tic_base+3*xtic_width, "100" x_tic_base+4*xtic_width)


set ylabel offset 1.1, 0 "Time Cost (ms)"   font  'Calibri,33'
set logscale y
set format y "10^{%L}"


set xlabel offset 0, 0.5 "Trajectory Data Size (%)" font  'Calibri, 33''


set key top left horizontal font  'Calibri,29' spacing 1 maxrow 1


set size 1.1, 1.0
set key  vertical maxrows 2

set boxwidth 3


#set style fill solid border -1
#plot dfile using (($1)-1)*alg_c+x_base:(($3))  title "STS+LOGC"   with boxes  lc rgb '#0000FF' lt 1 lw 2,\
#     dfile using (($1)-1)*alg_c+x_base+box_width:(($4))  title "STS+LORS" with boxes lc rgb '#FFA500' lt 1 lw 2,\
#	 dfile using (($1)-1)*alg_c+x_base+2*box_width:(($5))  title "TS+LOGC" with boxes lc rgb '#008000' lt 1 lw 2,\
#	 dfile using (($1)-1)*alg_c+x_base+3*box_width:(($6))  title "LOGC" with boxes lc rgb '#FF0000' lt 1 lw 2

plot dfile using (($1)-1)*alg_c+x_base:(($3))  title "Ours"   with boxes fill pattern 3 lt 1 lw 0.5,\
     dfile using (($1)-1)*alg_c+x_base+box_width:(($4))  title "Torch" with boxes fill pattern 4 lt 1 lw 0.5,\
	 dfile using (($1)-1)*alg_c+x_base+2*box_width:(($5))  title "MobilityDB" with boxes fill pattern 0 lt 1 lw 0.5