/**
  * @file detached_threads1.c 
  * @brief This example shows how it is possible to use Posix to create Detached Threads and how to wait for their completation in an asynchronous way 
  * @note The result is similar to what could have been done in a synchronous way with join() appunto 
  * @note To see it running http://coliru.stacked-crooked.com/a/daa73e98f71abaa8 
  */

#include <stdio.h>    
#include <string.h>    
#include <stdlib.h>  
#include <unistd.h>  
#include <pthread.h>  
#include <signal.h>  

/**
  * @brief The Posix Thread Number to be created 
  */
#define PT_THREADNUM 3 

/**
  * @brief The Global Running Thread Counter 
  */
unsigned int thread_running = 0;  

/**
  * @brief The Generic Posix Thread Function Call 
  * @param value An int representing an ID 
  */
void* start_function(void* value)  
{ 
    printf("[Thread %d] Started\n", *(int*)(value)); 
    sleep(4);  
    printf("[Thread %d] Finished\n", *(int*)(value)); 
    thread_running--; 
    pthread_exit(value); 
}  
  
int main(int argc, char** argv)  
{  
    int res; 
    pthread_attr_t attr;  
    pthread_t thread1[3]; 
    int temp[3]; 
    
    //** Creating the Thread Attribute 
    res = pthread_attr_init(&attr);  
    if (res != 0) {  
        perror("Attribute init failed");  
        exit(EXIT_FAILURE);  
    }
    
    //** Setting the Detached State in the Attribute Structure 
    res = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);  
    if (res != 0) {  
        perror("Setting detached state failed");  
        exit(EXIT_FAILURE);  
    }  
    
    //** Creating the Set of Threads with the Detached State 
    //** Note: This Thread is not joinable hence another mechanism is needed to detect their finish. In this case a global counter is used 
    for(int i=0; i<3; ++i)
    {
        //** Stores the different Thread IDs 
        temp[i] = i; 
        //** Increments the Global Running Threads Counter 
        thread_running++; 
        //** Creates the Thread with the Detached State 
        res = pthread_create(&thread1[i], &attr, start_function, (void*)&temp[i]);  
        if (res != 0) {  
            perror("Creation of thread failed");  
            exit(EXIT_FAILURE);  
        }  
    }
    
    //** Waiting for the all the Threads to finish checking the Global Running Threads Counter 
    while(thread_running > 0) {  
        printf("Waiting for %d threads to finish\n", thread_running);  
        sleep(1);  
    }  
  
    printf("All threads finished\n");      
    pthread_attr_destroy(&attr);  
}  

