#!/bin/bash

make test 
sleep 2 

if [ $1 == 'exec' ]
then 
    echo "./my_debugger -e dummy "
    ./my_debugger -e dummy     
elif [ $1 == 'attach' ]
then 
    ./dummy.o &
    echo $!
    dummy_pid=$(ps aux | grep dummy | awk '{print $2}' | awk '{if(NR==1){print $1}}')
    echo $dummy_pid
    echo "./my_debugger -a "$dummy_pid
    ./my_debugger -a $dummy_pid
fi 

