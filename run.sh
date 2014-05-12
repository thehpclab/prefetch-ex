#!/bin/bash

#set it to icc if you have intel compiler toolkit
CC=gcc 



$CC random_prefetch.c -std=c99 -o rand -O0
$CC sequential_prefetch.c -std=c99 -o seq -O0

SIZE=51200000
START=1
END=4

for(( i=$START; i<=$END; i*=2 ))
do
echo -e "\n\n\n Size: $SIZE, Prefetch distance: $i" &>>rand-result.txt
echo -e "\n\n\n Size: $SIZE, Prefetch distance: $i" &>>seq-result.txt

./rand $SIZE $i &>>rand-result.txt
./seq $SIZE $i &>>seq-result.txt
done
