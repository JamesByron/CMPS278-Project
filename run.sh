#!/bin/bash
gcc -mavx2 -g -o code code.c
sleep 2
echo 2 control
./code 2 16 0 >> output/data2_control
echo 2 test
./code 2 16 >> output/data2
sleep 1
echo 4 control
./code 4 25 0 >> output/data4_control
echo 4 test
sleep 1
./code 4 25 >> output/data4
sleep 1
echo 6 control
./code 6 32 0 >> output/data6_control
echo 6 test
sleep 1
./code 6 32 >> output/data6
sleep 1
echo 8 control
./code 8 36 0 >> output/data8_control
echo 8 test
./code 8 36 >> output/data8
sleep 2
echo 10 control
./code 10 50 0 >> output/data10_control
echo 10 test
./code 10 50 >> output/data10
echo done