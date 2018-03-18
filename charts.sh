#!/bin/bash
cd output	
#rm *.png
gnuplot <<EOF
set title "Time to Merge 2 Arrays"
set ylabel "Time (us)"
set xlabel "Number of Keys"
set key top left
set terminal png
set logscale y
set output '2.png'
plot '2.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'2.tsv' using 1:3 w linespoints t 'qsort' lw 2
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
set terminal png
set logscale y
set output '4.png'
plot '4.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'4.tsv' using 1:3 w linespoints t 'qsort' lw 2
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
set terminal png
set logscale y
set output '6.png'
plot '6.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'6.tsv' using 1:3 w linespoints t 'qsort' lw 2
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
set terminal png 
set logscale y
set output '8.png'
plot '8.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'8.tsv' using 1:3 w linespoints t 'qsort' lw 2
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
set terminal png
set logscale y
set output '10.png'
plot '10.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'10.tsv' using 1:3 w linespoints t 'qsort' lw 2
set terminal postscript eps solid size 3.5,3 enhanced color font 'Helvetica,15' linewidth 2
set output '../paper/10.eps'
plot '10.tsv' using 1:2 w linespoints t 'SIMD' lw 2,\
'10.tsv' using 1:3 w linespoints t 'qsort' lw 2
EOF
cd ..