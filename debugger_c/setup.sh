#!/bin/bash

make test 
sleep 2 

if [ $1 == 'exec' ]
then 
    echo "./my_debugger.o -e dummy "
    ./my_debugger.o -e dummy     
elif [ $1 == 'attach' ]
then 
    ./dummy.o &
    echo $!
    dummy_pid=$(ps aux | grep dummy | awk '{print $2}' | awk '{if(NR==1){print $1}}')
    echo $dummy_pid
    echo "./my_debugger.o -a "$dummy_pid
    ./my_debugger.o -a $dummy_pid
fi 

