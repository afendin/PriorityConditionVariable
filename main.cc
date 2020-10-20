#include <stdlib.h>
#include <stdio.h>
#include "PCV.h"
#include "3rdparty/thread.h"

using namespace std;

#define NTHREADS 50


Lock lock;
PCV pcv;

void waitWithPriority(int);
void printArray(int*);

int main()
{
    thread_t workers[NTHREADS];
    int priorityArray[NTHREADS];

    // Create threads that run test function
    for (int i = 0; i < NTHREADS; ++i) {
        int priority = random() % 100;
        priorityArray[i] = priority;
        thread_create(&workers[i], waitWithPriority, priority);
    }

    // Wait couple secs for threads to do its job
    sleep(2);

    // Print contents of priorities first to match later
    // which threads are waking up
    printArray(priorityArray);
    printf("Signal lock.\n");
    pcv.signal(&lock);
    sleep(1);
    printf("Signal lock.\n");
    pcv.signal(&lock);
    sleep(1);
    printf("Broadcast 40.\n");
    pcv.broadcast(&lock, 40);
    sleep(1);
    printf("Broadcast 4.\n");
    pcv.broadcast(&lock, 4);

    sleep(2);

    return 0;
}

// Simple test functions that waits with different priorities on the same lock
void waitWithPriority(int priority)
{
    lock.acquire();
    pcv.wait(&lock, priority);
    printf("Lock woke up with priority: %d.\n", priority);
    lock.release();
}

void printArray(int *priorityArray)
{
    printf("priority array: ");
    for (int i = 0; i < NTHREADS; ++i) {
        printf("%d,", priorityArray[i]);
    }
    printf("\n");
}