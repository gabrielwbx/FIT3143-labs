/*
* task3.c
* this task uses OpenMP to create threads to compute prime numbers in parallel
* How to run file:
* 1. Open terminal for Lab #1 (Week 4) file directory
* 2. Compile the file using 'gcc task3.c -o a -fopenmp'
* 3. Run the .exe using './a'
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>   
#include <omp.h>
#include <time.h>

//
//change value for testing diff stuff and computational time for different n
//
#define NUM_THREADS 6

int n;
int above_hundred;
FILE *fp = NULL;
int *results[NUM_THREADS];
int prime_count[NUM_THREADS];

//function prototypes
int prime(int num);
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
        fp = fopen("prime_numberst3.txt", "w"); //open file for writing, if doesnt exist it will be created 

        if (fp == NULL) {
            printf("Error: Unable to open prime numbers file.\n");
            return 1;
        }
    } else {
        fp = stdout; //if not above 100, write to terminal
    }

    printf("Start computation\n");
    clock_gettime(CLOCK_MONOTONIC, &startComp);

    //array for each thread initially
    for (i = 0; i < NUM_THREADS; i++) {
        results[i] = NULL;
        prime_count[i] = 0;
    }

    //parallel region
    #pragma omp parallel num_threads(NUM_THREADS) 
    {
        int my_rank = omp_get_thread_num();
        int count = 0;
        struct timespec t_start, t_end;
        double thread_time;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        printf("Thread %d created\n", my_rank);

        int *prime_array = malloc(n * sizeof(int));

        //dyanmic worlkoad balance, much better than static, as some threads will idle after
        #pragma omp for schedule(guided)
        for (int i = 2; i < n; i++) {
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
    } //exit parallel region

    WriteToFile(fp);

    //free allocated memory
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
        printf("Prime numbers written to prime_numberst3.txt\n");
    }

    printf("\nEnd computation\n");
    printf("\nTotal Computational time (inc writing): %f seconds\n", time_taken);

    return 0;
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
void WriteToFile(FILE *pArg) {
    int primes_per_line = 0;
 
    for (int t = 0; t < NUM_THREADS; t++) {
        for (int j = 0; j < prime_count[t]; j++) {
            if (primes_per_line > 0) {
                fprintf(pArg, "%s", (primes_per_line % 25 == 0) ? ",\n" : ", "); //ternary operators used for reducing clutter
            }
            fprintf(pArg, "%d", results[t][j]);
            primes_per_line++;
        }
    }
}
