# executable testing
#make my_debugger && ./my_debugger -e dummy.o 

# attachable testing
make my_debugger 
sleep 1
./dummy.o &
echo $!
dummy_pid=$(ps aux | grep dummy | awk '{print $2}' | awk '{if(NR==1){print $1}}')
echo $dummy_pid
./my_debugger -a $dummy_pid
