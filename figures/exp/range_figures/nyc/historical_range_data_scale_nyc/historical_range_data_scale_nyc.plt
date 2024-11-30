
set term postscript enhanced eps 32
set output "historical_range_data_scale_nyc.eps"
set style data histogram

set tmargin 1
set rmargin 1
set lmargin 5
set bmargin 3

set xrange [-0.5:4.5]
set yrange [1:1000]  noreverse nowriteback

# set title "NYC" font 'Calibri, 30' offset 0,-0.7,0

#set xtics font 'Calibri-Bold, 35' scale 0 offset 0, 0 ("Argoverse" 2.5)
set ytics font 'Calibri, 30'

set xtics font 'Calibri, 30' ( "20"  0, "40" 1, "60" 2, "80" 3, "100" 4)

set xlabel offset 0, 0.5 "Trajectory Data Size (%)" font  'Calibri, 33'

#set xlabel "topK"  font  'Calibri-Bold,26' 
set ylabel offset 1.4, 0 "Time Cost (ms)"   font  'Calibri,33' 
set logscale y
set format y "10^{%L}"
set key top left horizontal font  'Calibri,30' spacing 1 maxrow 1

set style data histogram
set style fill pattern 3 border -1

set size 1.2, 1.1

set datafile missing '-'

plot 'historical_range_data_scale_nyc.dat' using 2 title columnheader (1),  'historical_range_data_scale_nyc.dat' using 3  title columnheader (2), 'historical_range_data_scale_nyc.dat' using 4 title columnheader (3)
set output