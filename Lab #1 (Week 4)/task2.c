/*
* Student 1: Gabriel Wong Bo Xuen 34496114 gwon0021@student.monash.edu
* Student 2: Nicholas Ho Khor Pei 34496009 nhoo0025@student.monash.edu
* task2.c
* this task uses pthreads to create threads to compute prime numbers in parallel
* How to run file:
* 1. Open terminal for Lab #1 (Week 4) file directory
* 2. Compile the file using 'gcc task2.c -o a -pthread'
* 3. Run the .exe using './a'
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

// 
// IMPORTANT: CHANGE THE NUMBER OF THREADS TO CPU CORES, WORKED ON DIFFERENT DEVICES
//
#define NUM_THREADS 6 

//global variables
int n;
int above_hundred;
FILE *fp = NULL;
int *results[NUM_THREADS];
int prime_count[NUM_THREADS];

//function prototypes
int prime(int num);
void *ThreadFunc(void *pArg);
void WriteToFile(char *pFilename);

int main() {
    int i;
    double time_taken;
    struct timespec startComp, endComp;

    printf("Enter number:\n");
    scanf("%d", &n);

    //checks if we need to write into a file or not
    if (n >= 100) {
        above_hundred = 1; //c dont allow boolean unless library, 0 is false and any non-zero is true
    } else {
        above_hundred = 0;
    }

    printf("Start computation\n");
    clock_gettime(CLOCK_MONOTONIC, &startComp); 


    pthread_t tid[NUM_THREADS];
    int threadNum[NUM_THREADS];

    //fork, the threads will now run in parralel
    for (i = 0; i < NUM_THREADS; i++) {
        threadNum[i] = i;
        pthread_create(&tid[i], 0, ThreadFunc, &threadNum[i]);
    }

    //join
    for(i = 0; i < NUM_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    
    if (above_hundred == 1) {
        WriteToFile("prime_numberst2.txt"); 
        printf("Prime numbers written to file prime_numbers2.txt\n");
    } else {
        WriteToFile(NULL);
    }
    
    for (int t = 0; t < NUM_THREADS; t++) {
        if (results[t] != NULL) {
            free(results[t]);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &endComp);

    //taken from lab week 3
    time_taken = (endComp.tv_sec - startComp.tv_sec) * 1e9; 
    time_taken = (time_taken + (endComp.tv_nsec - startComp.tv_nsec)) * 1e-9; 

    printf("\nEnd computation\n");
    printf("\nTotal computational time (inc writing): %f seconds\n", time_taken);

    return 0;
}

//thread function for the threads to run parrelelly
void *ThreadFunc(void *pArg) {
    int count;
    struct timespec t_start, t_end;
    double thread_time;

    clock_gettime(CLOCK_MONOTONIC, &t_start);

    int my_rank = *((int*)pArg);

    printf("Thread %d created\n", my_rank);

    int npt = n / NUM_THREADS; // npt = numbers per thread
    int nptr = n % NUM_THREADS; // nptr = numbers per thread remainder,

    int sp = my_rank * npt;
    int ep = sp + npt;

    if (my_rank == NUM_THREADS - 1) {
        ep += nptr; //last thread gets the remainder
    }

    int *prime_array = malloc((ep - sp) * sizeof(int)); //allocates memory for each thread for its corresponding sizes

    count = 0;

    //check all numbers in this thread's range
    for (int i = sp; i < ep; i++) {
        if (prime(i)) {
            prime_array[count] = i;
            count++;
        }
    }

    results[my_rank] = prime_array; //store the threads array into their corresponding index of the global array
    prime_count[my_rank] = count; //stores the count of elements in the threads array

    clock_gettime(CLOCK_MONOTONIC, &t_end);

    thread_time = (t_end.tv_sec - t_start.tv_sec) * 1e9; 
    thread_time = (thread_time + (t_end.tv_nsec - t_start.tv_nsec)) * 1e-9; 
    printf("Thread %d finished. Computational time: %fs\n", my_rank, thread_time);
    
    return NULL;
}

int prime(int num)
{
    if ((num < 2) || ((num % 2 == 0) && (num != 2))) { //if u divide 2 and theres no remainder it means its an even number and not prime
        return 0;
    }

    // only need to check divisors up to sqrt(num)
    int sqrt_num = (int)sqrt(num);
    for (int x = 3; x <= sqrt_num; x += 2) { //skip even numbers
        if (num % x == 0) {
            return 0;
        }
    }
    return 1;
}

//writing to file function, takes in a file pointer as an argument
void WriteToFile(char *pFilename) {
    int total = 0;

    for (int t = 0; t < NUM_THREADS; t++) {
        total += prime_count[t];
    }

    if (total == 0) {
        return;
    }

    FILE *pFile = stdout;
    if (pFilename != NULL) {
        pFile = fopen(pFilename, "w");
        if (pFile == NULL) {
            printf("Error opening file.\n");
            return;
        }
    }

    int count = 0;
    for (int t = 0; t < NUM_THREADS; t++) {
        for (int j = 0; j < prime_count[t]; j++) {
            count++;
            fprintf(pFile, "%d", results[t][j]);
            if (count < total) {
                fprintf(pFile, "%s", (count % 25 == 0) ? ",\n" : ", ");//ternary operators to reduce if statements for visibility
            }
        }
    }

    if (pFilename != NULL) {
        fclose(pFile);
    }
}
