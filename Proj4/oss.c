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

#define MAX_PROCESS 18

// Define the Process control board

typedef struct {
    int pid;    // Process ID
    int total_cpu_time;   // Total CPU time used
    int total_time_in_system;   // Total time in the system
    int local_pid;   // Local simulated PID
} pcb_t;

// Define the clock
typedef struct{
    unsigned int clock_sec;
    unsigned int clock_nan;
    pcb_t process_table[MAX_PROCESS];
}Clock_t;


int main(int argc, const char * argv[]) {
    int i;
    
    // generate a unique key for shared memory
    key_t key = ftok("shared_mem_key", 15150);
    
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
    
    //initialize the process control table
    for(i=0; i<MAX_PROCESS; i++){
        shmem->process_table[i].pid = -1; // Set PID to -1 to indicate an unused process control block
        shmem->process_table[i].total_cpu_time = 0;
        shmem->process_table[i].total_time_in_system = 0;
        shmem->process_table[i].local_pid = i+1; // Set the local simulated PID
    }
    
    
    //Detach from the shared mem segment
    shmdt(shmem);
    
    
    return 0;
}

