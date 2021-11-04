set size ratio 0.8

aqua = "#00FFFF"; azure = "#F0FFFF"
aliceblue = "#F0F8FF"

set key font ",18"

set grid ytics mytics  # draw lines for each ytics and mytics
set grid xtics mytics
set mytics 1           # set the spacing for the mytics
set grid               # enable the grid

set style fill solid 1.00 border 0

set xlabel "t/T" font ",18"

set ylabel "<Logical_{Z}>" font ",18"
# set ylabel offset -2,0

set xtics font ",10"
set ytics font ",10"

set xrange[0:1000]
set yrange[-1:1]



set title "1 sample" font ",24"

plot 'data_ns=1_nt=1000.dat' using 1:2 with lines lc 1 lw 2 t "Without decoder",\
'data_ns=1_nt=1000.dat' using 1:3 with lines lc 2 lw 2 t "Without decoder",\
'data_decoder_thd=0.100000_ns=1_nt=1000.dat' using 1:2 with lines lc 3 lw 2 t "With decoder",\
'data_decoder_thd=0.100000_ns=1_nt=1000.dat' using 1:3 with lines lc 4 lw 2 t "With decoder",\


# set title "200 samples" font ",24"
#
# plot 'data_ns=200_nt=1000.dat' using 1:2 with lines ls 1 lw 2 t "Without decoder",\
# 'data_ns=200_nt=1000.dat' using 1:3 with lines ls 1 lw 2 t "",\
# 'data_decoder_thd=0.100000_ns=200_nt=1000.dat' using 1:2 with lines lc 2 lw 2 t "With decoder",\
# 'data_decoder_thd=0.100000_ns=200_nt=1000.dat' using 1:3 with lines lc 2 lw 2 t "",\



# set title "No decoder, Different sample number" font ",24"
#
# plot 'data_ns=1_nt=1000.dat' using 1:2 with lines ls 1 lw 2 t "1",\
# 'data_ns=1_nt=1000.dat' using 1:3 with lines ls 1 lw 2 t "",\
# 'data_ns=50_nt=1000.dat' using 1:2 with lines lc 2 lw 2 t "50",\
# 'data_ns=50_nt=1000.dat' using 1:3 with lines lc 2 lw 2 t "",\
# 'data_ns=200_nt=1000.dat' using 1:2 with lines lc 3 lw 2 t "200",\
# 'data_ns=200_nt=1000.dat' using 1:3 with lines lc 3 lw 2 t ""


# set title "Threshold = 0.10, Different sample number" font ",24"
#
# plot 'data_decoder_thd=0.100000_ns=1_nt=1000.dat' using 1:2 with lines ls 1 lw 2 t "1",\
# 'data_decoder_thd=0.100000_ns=1_nt=1000.dat' using 1:3 with lines ls 1 lw 2 t "",\
# 'data_decoder_thd=0.100000_ns=50_nt=1000.dat' using 1:2 with lines lc 2 lw 2 t "50",\
# 'data_decoder_thd=0.100000_ns=50_nt=1000.dat' using 1:3 with lines lc 2 lw 2 t "",\
# 'data_decoder_thd=0.100000_ns=200_nt=1000.dat' using 1:2 with lines lc 3 lw 2 t "200",\
# 'data_decoder_thd=0.100000_ns=200_nt=1000.dat' using 1:3 with lines lc 3 lw 2 t ""


# set title "50 samples, With decoder, Different threshold" font ",24"
#
# plot 'data_decoder_thd=0.010000_ns=50_nt=1000.dat' using 1:2 with lines ls 1 lw 2 t "threshold=0.01",\
# 'data_decoder_thd=0.010000_ns=50_nt=1000.dat' using 1:3 with lines ls 1 lw 2 t "",\
# 'data_decoder_thd=0.100000_ns=50_nt=1000.dat' using 1:2 with lines lc 2 lw 2 t "threshold = 0.10",\
# 'data_decoder_thd=0.100000_ns=50_nt=1000.dat' using 1:3 with lines lc 2 lw 2 t "",\
# 'data_decoder_thd=0.130000_ns=50_nt=1000.dat' using 1:2 with lines lc 3 lw 2 t "threshold = 0.13",\
# 'data_decoder_thd=0.130000_ns=50_nt=1000.dat' using 1:3 with lines lc 3 lw 2 t ""