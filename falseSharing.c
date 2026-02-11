#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

#define ITERATIONS 500000000

typedef struct {
    long a;
    long b;
} Counter;

Counter *c;

void *thread1(void *arg)
{
    for (long i = 0; i < ITERATIONS; i++)
    {
        c->a++;
    }
    return NULL;
}

void *thread2(void *arg)
{
    for (long i = 0; i < ITERATIONS; i++)
    {
        c->b++;
    }
    return NULL;
}

int main()
{

    c= (Counter*)malloc(sizeof(Counter));
    c->a=0;
    c->b=0;
    pthread_t p1, p2;

    clock_t start = clock();
    if(pthread_create(&p1,NULL,&thread1,NULL)!=0)
    {
        printf("Thread 1 cannot be created\n");
        return 0;
    }
    if (pthread_create(&p2, NULL, &thread2, NULL) != 0)
    {
        printf("Thread 2 cannot be created\n");
        return 0;
    }
    if (pthread_join(p1, NULL) != 0)
    {
        printf("Thread 1 cannot be joined\n");
        return 3;
    }
    if (pthread_join(p2, NULL) != 0)
    {
        printf("Thread 2 cannot be joined\n");
        return 4;
    }
    clock_t end = clock();

    double timetaken = (((double)(end - start)))/ CLOCKS_PER_SEC;

    printf("a = %d b = %d\n",c->a,c->b);

    printf("Time taken : %lf s\n",timetaken);
    return 0;
}