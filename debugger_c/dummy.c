#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    int i ; 
    for(int i = 0 ; i < 10; ++i){
        printf("Counter  ; %d\n",i);
        sleep(4);
    
    }

    return 0 ; 


}