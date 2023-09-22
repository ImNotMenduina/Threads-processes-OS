#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h> 
#include <sys/syscall.h>

void * routine(void* arg){
    int* threadId = (int*)arg;
    printf("hello from thread %d\n", *threadId);
    sleep(3);
    printf("Ending thread\n");
}

int main(int argc, char* argv[])
{
    pthread_t t1, t2;
    int idt1 = 1;
    int idt2 = 2;
    if(pthread_create (&t1, NULL, &routine, (void*)&idt1) != 0)
        return 1;
    if(pthread_create (&t2, NULL, &routine, (void*)&idt2) != 0)
        return 2;    
    if(pthread_join(t1, NULL) != 0)//wait for thread t1
        return 3;;
    if(pthread_join(t2, NULL) != 0)//wait for thread t2
        return 4;;
    return 0;
}

