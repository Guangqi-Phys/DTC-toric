set term pdfcairo enhanced font "Arial,16" 
set size ratio 0.8 
set output "wave.pdf"

set multiplot layout 2,2 \
              margins 0.2,1,0.2,0.9 \
              spacing 0.0,0.04
			  
			  


# plot 1

unset key

set style fill solid 1.00 border 0

set ylabel "|{/Symbol y}|^2" 
#set xlabel "z (nm)" 
unset xlabel

#set xtics -100,50,100 
set ytics 0,0.02,0.06
unset xtics
set xrange[-50:50]
set yrange[0:0.06]



plot "wavesK0.00En0.1717.dat" using 1:2 with lines lc 2 lw 3,\
"wavesK0.00En0.2179.dat" using 1:2 with lines lc 3 lw 3


# plot 2
unset key
set style fill solid 1.00 border 0

#set ylabel "|{/Symbol y}|^2" 
#set xlabel "z (nm)" 
unset xlabel
unset ylabel

#set xtics -100,50,100 
set ytics 0,0.02,0.06
unset xtics
#unset ytics

set xrange[-50:50]
set yrange[0:0.06]

plot "wavesK0.00En0.2538.dat" using 1:2 with lines lc 4 lw 3, \
"wavesK0.00En0.2795.dat" using 1:2 with lines lc 5 lw 3,\
"wavesK0.00En0.2949.dat" using 1:2 with lines lc 6 lw 3


# plot 3
unset key
set style fill solid 1.00 border 0

set ylabel "|{/Symbol y}|^2" 

set xlabel "z (nm)" 

set xtics -100,50,100 
set ytics 0,0.02,0.06

set xrange[-50:50]
set yrange[0:0.06]


plot "wavesK0.28En0.1125.dat" using 1:2 with lines lc rgb "#E16614" lw 3,\
"wavesK0.28En0.1967.dat" using 1:2 with lines lc 7 lw 3 ,\

# plot 4
unset key
set style fill solid 1.00 border 0

#set ylabel "|{/Symbol y}|^2" 
set xlabel "z (nm)" 
unset ylabel

set xtics -100,50,100 
set ytics 0,0.02,0.06
#unset ytics

set xrange[-50:50]
set yrange[0:0.06]


plot "wavesK0.28En0.2867.dat" using 1:2 with lines lc 8 lw 3,\
"wavesK0.28En0.4126.dat" using 1:2 with lines lc 9 lw 3

unset multiplot
set out