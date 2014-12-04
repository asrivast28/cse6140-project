set term png size 800,600
set output 'gr202-LS1-qrtd.png'

set grid

set xlabel "Run time [seconds]"

set ylabel "P(solve)"

set key bottom right

plot "qrtd.dat" using 2:1 title '11.0' with linespoints, \
     "qrtd.dat" using 3:1 title '10.5' with linespoints, \
     "qrtd.dat" using 4:1 title '10.0' with linespoints, \
     "qrtd.dat" using 5:1 title '9.5' with linespoints, \
     "qrtd.dat" using 6:1 title '9.0' with linespoints, \
     "qrtd.dat" using 7:1 title '8.5' with linespoints, \
     "qrtd.dat" using 8:1 title '8.0' with linespoints

# eof
