set terminal postscript enhanced monochrome dashed
set output 'qrtd.eps'

set grid

set xlabel "Run time [seconds]"

set ylabel "P(solve)"

set key bottom right

plot "qrtd.dat" using 4:1 title '13.0' with linespoints, \
     "qrtd.dat" using 6:1 title '12.0' with linespoints, \
     "qrtd.dat" using 8:1 title '11.0' with linespoints, \
     "qrtd.dat" using 10:1 title '10.0' with linespoints, \
     "qrtd.dat" using 12:1 title '9.0' with linespoints, \
     "qrtd.dat" using 14:1 title '8.0' with linespoints, \
     "qrtd.dat" using 16:1 title '7.0' with linespoints

# eof
