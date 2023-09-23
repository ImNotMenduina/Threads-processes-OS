#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<math.h>
#define TAM_ARRAY 10

struct Inf
{
    int threadTotal;
    int threadId;
    int begin;
    int interval;
};
typedef struct Inf INF_THREAD;

int v[] = {29, 1, 51, 22, 54, 24, 5, 10, 9, 2};
int x; 
int is_x = 0;

void * findX(void* threadInf)
{
    INF_THREAD inf = *(INF_THREAD*)threadInf;

    int i = 0;
    for(i = 0; i < inf.interval; i++)
    {
        if(v[inf.begin + i] == x)
        {
            printf("Element [%d] found\nIndex [%d]\nThread ID [%d]\n",x ,inf.begin + i, inf.threadId);
            is_x = 1;
        }   
    }
    if((TAM_ARRAY % inf.threadTotal > 0) && (inf.threadId == inf.threadTotal - 1))
    {
        for(int j = inf.begin + i; j < TAM_ARRAY; j++)
        {   
            if(v[j] == x)
            {
                printf("Element [%d] found\nIndex [%d]\nThread ID [%d]\n",x ,inf.begin + i, inf.threadId);
                is_x = 1;
            }   
        }
    }   
}   

int main(int argc, char* argv[])
{   
    printf("N Threads : "); 
    int n;
    scanf("%d", &n);

    printf("Find Number : ");
    scanf("%d", &x);

    pthread_t id[n];

    int interval = TAM_ARRAY/n;

    for(int i=0; i<n; i++)
    {
        if(!is_x)
        {
            INF_THREAD* inf = (INF_THREAD*)malloc(sizeof(INF_THREAD));
            //it gives for each thread an subarray to work
            inf->threadTotal = n;
            inf->threadId = i;
            inf->begin = i * interval;
            inf->interval = interval;
            pthread_create(&id[i], NULL, &findX, (void*)inf);
        }
        else
        {    
            break;
        }
    }
    for(int i=0; i<n; i++)
    {
        pthread_join(id[i], NULL);
    } 
    return 0;
}