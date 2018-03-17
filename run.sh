#!/bin/bash
gcc -mavx2 -g -o code code.c
sleep 5
echo 1
./code >> output/data1
sleep 5
echo 2
./code >> output/data2
sleep 5
echo 3
./code >> output/data3
sleep 5
echo 4
./code >> output/data4
sleep 5
echo 5
./code >> output/data5
sleep 5
echo 6
./code >> output/data6
sleep 5
echo 7
./code >> output/data7
sleep 5
echo 8
./code >> output/data8
sleep 5
echo 9
./code >> output/data9
sleep 5
echo 10
./code >> output/data10
echo done