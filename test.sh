#!/bin/bash
if [[ $# -ne 1 ]]; then
  echo "usage 1 argument : scheduling algoritm"
  exit
fi
if [ $1 == "0" ]; then
file="fifo.csv"
elif [ $1  == "1" ]; then
  file="sjf.csv"
elif [ $1 == "2" ]; then
  file="srt.csv"
else
  echo "wrong argument"
  exit
fi
  #statements
if [ -f $file ];then
  rm $file
  touch $file
fi



for (( i = 1; i < 1000; i++ )); do
  standart_deviation=$(bc <<< $i*0.05) #fixed percentage of total_cpu_time
  arrival_time_limit=$(bc <<< $i*0.1) #fixed percentage of total_cpu_time
  standart_deviation=$(printf "%.0f" "$standart_deviation") #rounding it
  arrival_time_limit=$(printf "%.0f" "$arrival_time_limit")
  doveratt=$(./pa2 100 $arrival_time_limit "$i" $standart_deviation $1 |tail -n 1| grep -Eo '[0-9.]{1,99}')

  echo "$doveratt," >> $file
done
