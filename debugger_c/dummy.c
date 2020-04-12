#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    int i = 100; 
    while(i--){
        printf("Counter  ; %d\n",i);
        sleep(1);
        
    }

    exit(1);

}