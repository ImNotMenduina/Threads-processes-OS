#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

#define PARKING_THREADS 2
#define WAITING_LIST_SIZE 10

void* producer();
void* consumer(void* car);

int N = -1, cars = -1; // SPOTS QNTTY
int waitingList = 0;
int carsInside = 0;

sem_t mutex;
sem_t parkingSpots;
sem_t carsSmph;
sem_t waitingSpots;

struct cars
{
    pthread_t thCar;
    int id;
    int spot;
};
typedef struct cars Cars;

int main()
{
    printf("Parking Spots : "); 
    scanf("%d", &N);

    printf("Cars : ");
    scanf("%d", &cars);

    sem_init(&mutex, 0, 1);
    sem_init(&carsSmph, 0, 0);
    sem_init(&parkingSpots, 0, N);
    sem_init(&waitingSpots, 0, 10);

    pthread_t thSpots[PARKING_THREADS];
    for(int i=0; i< PARKING_THREADS; i++)
        pthread_create(&thSpots[i], NULL, &producer, NULL);

    Cars* thCars = (Cars*)malloc(sizeof(Cars) * cars); 
    for(int j=0; j<cars; j++)
    {
        thCars[j].id = j;
        pthread_create(&thCars[j].thCar, NULL, &consumer, (void*)&thCars[j]);
    }
    for(int i=0; i<PARKING_THREADS; i++)
        pthread_join(thSpots[i], NULL);

    for(int i=0; i<cars; i++)
        pthread_join(thCars[i].thCar, NULL);

    sem_destroy(&mutex);
    sem_destroy(&parkingSpots);
    sem_destroy(&carsSmph);

    pthread_exit(0);
}

void* consumer(void* car)
{
    sem_wait(&mutex);
    Cars* thCar = (Cars*)car;
    if(carsInside < N)
    {
        carsInside++;
        sem_post(&carsSmph);
        sem_post(&mutex);
        sem_wait(&parkingSpots);
        //printf("car [%d] inside\n", thCar->id);
    }
    else if(waitingList < WAITING_LIST_SIZE)
    {
        waitingList++;
        sem_post(&mutex);
        sem_post(&carsSmph);
        sem_wait(&waitingSpots);
        //printf("car [%d] is waiting\n", thCar->id);

        sem_wait(&parkingSpots);
        sem_post(&waitingSpots);
        //printf("car [%d] inside\n", thCar->id);

        sem_wait(&mutex);
        waitingList--;
        carsInside++;
        sem_post(&mutex);
    }
    else
    {
        //printf("car [%d] you should GET AWAY FROM HERE!!\n", thCar->id);
        sem_post(&mutex);
    }
}

void* producer()
{
    while(1)
    {
        sem_wait(&carsSmph);

        sleep(rand()%10+1);

        sem_wait(&mutex);
        carsInside--;
        //printf("A car is leaving\n");
        sem_post(&mutex);

        sem_post(&parkingSpots);
    }
}