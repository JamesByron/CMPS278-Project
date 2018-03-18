#!/bin/bash
gcc -mavx2 -g -o code code.c
sleep 5
echo 2
./code 2 150 >> output/data2
sleep 5
echo 4
./code 4 80 >> output/data4
sleep 5
echo 6
./code 6 60 >> output/data6
sleep 5
echo 8
./code 8 >> output/data8
sleep 5
echo 10
./code >> output/data10
echo done