#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h> 
#include <sys/syscall.h>

void * routine(){
    printf("hello");
}

int main(int argc, char* argv[])
{
    pthread_t t1;
    pthread_create (&t1, NULL, &routine, NULL);
    pthread_join(t1, NULL);//wait for threads
    return 0;
}

