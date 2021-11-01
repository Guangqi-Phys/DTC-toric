set size ratio 0.8

aqua = "#00FFFF"; azure = "#F0FFFF"
aliceblue = "#F0F8FF"

set key

set style fill solid 1.00 border 0

set xlabel "T" font ",16"

set ylabel "<Logical_Z>" font ",16"
# set ylabel offset -2,0

set xtics font ",10"
set ytics font ",10"

set xrange[0:500]
set yrange[-1:1]


set title "" font ""

plot 'data.dat' using 1:2 pt 22  ,'data.dat' using 1:3 pt 22 