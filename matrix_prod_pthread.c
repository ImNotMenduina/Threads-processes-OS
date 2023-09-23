#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct threadPackage
{
    int threadId;
    int lineInterval;
    int threadsTotal;
    int begin;
    int columnLength;
};
typedef struct threadPackage THREAD_PACK;

void* matrix_prod(void* threadInf);

int** ma;
int** mb;
int** mc;

int main(int agrc, char* argv[])
{
    const int MATRIX_LINE = atoi(argv[1]);
    const int MATRIX_COLUMN = MATRIX_LINE;

    ma = (int**)malloc(sizeof(int*) * MATRIX_LINE);
    mb = (int**)malloc(sizeof(int*) * MATRIX_LINE); 
    mc = (int**)malloc(sizeof(int*) * MATRIX_LINE);  
    //A
    for(int i=0; i<MATRIX_LINE; i++)
        ma[i] = (int*)malloc(sizeof(int) * MATRIX_COLUMN);
    //B
    for(int i=0; i<MATRIX_LINE; i++)
        mb[i] = (int*)malloc(sizeof(int) * MATRIX_COLUMN);
    //C
    for(int i=0; i<MATRIX_LINE; i++)
        mc[i] = (int*)malloc(sizeof(int) * MATRIX_COLUMN);

    //A
    for(int i=0; i<MATRIX_LINE; i++)
    {
        for(int j=0; j<MATRIX_COLUMN; j++)
        {
            ma[i][j] = rand() % 10;
        }
    }
    //B
    for(int i=0; i<MATRIX_LINE; i++)
    {
        for(int j=0; j<MATRIX_COLUMN; j++)
        {
            mb[i][j] = rand() % 10;
        }
    }
    //C
    for(int i=0; i<MATRIX_LINE; i++)
    {
        for(int j=0; j<MATRIX_COLUMN; j++)
        {
            mc[i][j] = 0;
        }
    }

    int n = 4;
    pthread_t id[n];

    int interval = MATRIX_LINE / n;

    for(int i=0; i<n; i++)
    {
        THREAD_PACK* threadInf = (THREAD_PACK*)malloc(sizeof(THREAD_PACK*));
        threadInf->threadId = i;
        threadInf->threadsTotal = n;
        threadInf->lineInterval = interval;
        threadInf->begin = interval * i;
        threadInf->columnLength = MATRIX_COLUMN;
        pthread_create(&id[i], NULL, &matrix_prod, (void*)threadInf);
    }

    for(int i=0; i<n; i++)
        pthread_join(id[i], NULL);

    free(ma);
    free(mb);
    free(mc);

    return 0;
}

void* matrix_prod(void* threadInf)
{   
    THREAD_PACK* thread = (THREAD_PACK*)threadInf;

        for(int i=0; i<thread->lineInterval; i++)
        {
            for(int j=0; j<thread->columnLength; j++)
            {
                for(int k=0; k<thread->columnLength; k++)
                {
                    mc[thread->begin +i][j] += ma[thread->begin + i][k] * mb[k][j];
                }
            }
        }
    free(thread);
}