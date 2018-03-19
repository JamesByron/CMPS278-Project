#!/bin/bash
python3 dataProcessing.py data2
python3 dataProcessing.py data4
python3 dataProcessing.py data6
python3 dataProcessing.py data8
python3 dataProcessing.py data10
cd output	
rm *.png
sleep 1
gnuplot <<EOF
set title "Time to Merge 2 Arrays"
set ylabel "Time (us)"
set xlabel "Number of Keys"
set key top left
set terminal png size 1024,1024
set logscale y
set output '2.png'
plot 'data2.tsv' using 1:2:3:4 t 'SIMD' w yerrorlines ps 0 lw 2,\
'data2_control.tsv' using 1:2:3:4 t 'qsort' w yerrorlines ps 0 lw 2
set terminal postscript eps solid size 3.5,3 enhanced color font 'Helvetica,15' linewidth 2
set output '../paper/2.eps'
plot '2.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'2.tsv' using 1:3 w linespoints t 'qsort' lw 2
EOF
gnuplot <<EOF
set title "Time to Merge 4 Arrays"
set ylabel "Time (us)"
set xlabel "Number of Keys"
set key top left
set terminal png size 1024,1024
set logscale y
set output '4.png'
plot 'data4.tsv' using 1:2:3:4 t 'SIMD' w yerrorlines ps 0 lw 2,\
'data4_control.tsv' using 1:2:3:4 t 'qsort' w yerrorlines ps 0 lw 2
set terminal postscript eps solid size 3.5,3 enhanced color font 'Helvetica,15' linewidth 2
set output '../paper/4.eps'
plot '4.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'4.tsv' using 1:3 w linespoints t 'qsort' lw 2
EOF
gnuplot <<EOF
set title "Time to Merge 6 Arrays"
set ylabel "Time (us)"
set xlabel "Number of Keys"
set key top left
set terminal png size 1024,1024
set logscale y
set output '6.png'
plot 'data6.tsv' using 1:2:3:4 t 'SIMD' w yerrorlines ps 0 lw 2,\
'data6_control.tsv' using 1:2:3:4 t 'qsort' w yerrorlines ps 0 lw 2
set terminal postscript eps solid size 3.5,3 enhanced color font 'Helvetica,15' linewidth 2
set output '../paper/6.eps'
plot '6.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'6.tsv' using 1:3 w linespoints t 'qsort' lw 2
EOF
gnuplot <<EOF
set title "Time to Merge 8 Arrays"
set ylabel "Time (us)"
set xlabel "Number of Keys"
set key top left
set terminal png size 1024,1024 
set logscale y
set output '8.png'
plot 'data8.tsv' using 1:2:3:4 t 'SIMD' w yerrorlines ps 0 lw 2,\
'data8_control.tsv' using 1:2:3:4 t 'qsort' w yerrorlines ps 0 lw 2
set terminal postscript eps solid size 3.5,3 enhanced color font 'Helvetica,15' linewidth 2
set output '../paper/8.eps'
plot '8.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'8.tsv' using 1:3 w linespoints t 'qsort' lw 2
EOF
gnuplot <<EOF
set title "Time to Merge 10 Arrays"
set ylabel "Time (us)"
set xlabel "Number of Keys"
set key top left
set terminal png size 1024,1024
set logscale y
set output '10.png'
plot 'data10.tsv' using 1:2:3:4 t 'SIMD' w yerrorlines ps 0 lw 2,\
'data10_control.tsv' using 1:2:3:4 t 'qsort' w yerrorlines ps 0 lw 2
set terminal postscript eps solid size 3.5,3 enhanced color font 'Helvetica,15' linewidth 2
set output '../paper/10.eps'
plot '10.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'10.tsv' using 1:3 w linespoints t 'qsort' lw 2
EOF
cd ..