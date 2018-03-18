#!/bin/bash
gcc -mavx2 -g -o code code.c
sleep 3
echo 2
./code 2 20 >> output/data2
sleep 2
echo 4
./code 4 35 >> output/data4
sleep 2
echo 6
./code 6 40 >> output/data6
sleep 2
echo 8
./code 8 45 >> output/data8
sleep 2
echo 10
./code >> output/data10
echo done