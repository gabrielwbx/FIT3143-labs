#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

// 
// IMPORTANT: CHANGE THE NUMBER OF THREADS TO CPU CORES, WORKED ON DIFFERENT DEVICES
//
#define NUM_THREADS 6 

int n;
int above_hundred;
FILE *fp = NULL;
int *results[NUM_THREADS];
int prime_count[NUM_THREADS];

//function prototypes
int prime(int num);
void *ThreadFunc(void *pArg);
void WriteToFile(FILE *pArg);

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

    if (above_hundred == 1) {   
        fp = fopen("prime_numberst2.txt", "w"); //open file for writing, if doesnt exist it will be created 

        if (fp == NULL) {
            printf("Error: Unable to open prime numbers file.\n");
            return 1;
        }
    } else {
        fp = stdout; //if not above 100, write to terimnal
    }

    printf("Start computation\n");
    clock_gettime(CLOCK_MONOTONIC, &startComp); 

    if (n <= 2) {
        fprintf(fp, "No prime numbers found.\n"); //only special case
    } else {
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

        WriteToFile(fp);
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

    if (above_hundred == 1) {
        fclose(fp); 
        printf("Prime numbers written to prime_numberst2.txt\n");
    }

    printf("\nEnd computation\n");
    printf("\nTotal omputational time: %f seconds\n", time_taken);

    return 0;
}

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

    int *prime_array = malloc((ep - sp) * sizeof(int));

    count = 0;

    for (int i = sp; i < ep; i++) {
        if (prime(i)) {
            prime_array[count] = i;
            count++;
        }
    }

    results[my_rank] = prime_array;
    prime_count[my_rank] = count;

    clock_gettime(CLOCK_MONOTONIC, &t_end);

    thread_time = (t_end.tv_sec - t_start.tv_sec) * 1e9; 
    thread_time = (thread_time + (t_end.tv_nsec - t_start.tv_nsec)) * 1e-9; 
    printf("Thread %d finished. Computational time: %fs\n", my_rank, thread_time);
    
    return NULL;
}

int prime(int num)
{
    if (num == 2) {
        return 1;
    }

    if (num < 2) {
        return 0;
    }

    if (num % 2 == 0) {
        return 0;  //if u divide 2 and theres no remainder it means its an even number and not prime
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

void WriteToFile(FILE *pArg) {
    int primes_per_line = 0;
 
    for (int t = 0; t < NUM_THREADS; t++) {
        for (int j = 0; j < prime_count[t]; j++) {
            if (primes_per_line > 0) {
                fprintf(pArg, "%s", (primes_per_line % 25 == 0) ? ",\n" : ", "); //intrroduced ternary operators so it doesnt looks too clustered
            }
            fprintf(pArg, "%d", results[t][j]);
            primes_per_line++;
        }
    }
}
