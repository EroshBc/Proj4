//
//  oss.c
//  Proj4
//
//  Created by Erosh Boralugodage on 3/21/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_PROCESS 18;
// Define the clock
typedef struct{
    unsigned int clock_sec;
    unsigned int clock_nan;
}Clock_t;

int main(int argc, const char * argv[]) {
    // generate a unique key for shared memory
    key_t key = ftok("shared_mem_key", "R");\
    
    // create shared memory
    int shmid = shmget(key, sizeof(Clock_t), IPC_CREAT|0666);
    
    if(shmid<0){
        perror("shmget");
        exit(1);
    }
    
    //attached shared memory segment
    Clock_t* shmem = (Clock_t*) shmat(shmid, NULL,0);
    if(shmem ==(Clock_t*)-1){
        perror("shmat");
        exit(1);
    }
    
    //initialized simulated system clock
    shmem->clock_sec = 0;
    shmem->clock_nan = 0;
    
    
    
    //Detach from the shared mem segment
    shmdt(shmem);
    
    
    return 0;
}

