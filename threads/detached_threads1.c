#include <stdio.h>    
#include <string.h>    
#include <stdlib.h>  
#include <unistd.h>  
#include <pthread.h>  
#include <signal.h>  
  
unsigned int thread_running = 0;  

/**
  * @brief The Generic Posix Thread Function Call 
  * @param value An int representing an ID 
  */
void* start_function(void* value)  
{ 
    printf("[Thread %d] Started\n", *(int*)(value)); 
    //printf("%s is now entering the thread function.\n", (char*)value);  
    sleep(4);  
    printf("[Thread %d] Finished\n", *(int*)(value)); 
    //printf("%s is now leaving the thread function.\n", (char*)value);  
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
    
    //** Setting the Detached State 
    res = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);  
    if (res != 0) {  
        perror("Setting detached state failed");  
        exit(EXIT_FAILURE);  
    }  
    
    //** Creating the Thread with the Detached State 
    for(int i=0; i<3; ++i)
    {
        temp[i] = i; 
        thread_running++; 
        res = pthread_create(&thread1[i], &attr, start_function, (void*)&temp[i]);  
        if (res != 0) {  
            perror("Creation of thread failed");  
            exit(EXIT_FAILURE);  
        }  
    }
    
    //** Waiting for the Thread to finish 
    while(thread_running > 0) {  
        printf("Waiting for %d threads to finish\n", thread_running);  
        sleep(1);  
    }  
  
    printf("All threads finished\n");      
    pthread_attr_destroy(&attr);  
}  
