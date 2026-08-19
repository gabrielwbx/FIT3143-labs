/*
* Student 1: Gabriel Wong Bo Xuen 34496114 gwon0021@student.monash.edu
* Student 2: Nicholas Ho Khor Pei 34496009 nhoo0025@student.monash.edu
* task3.c
* this task uses OpenMP to create threads to compute prime numbers in parallel
* How to run file:
* 1. Open terminal for Lab #1 (Week 4) file directory
* 2. Compile the file using 'gcc task3.c -o a -fopenmp'
* 3. Run the .exe using './a'
*
* AI DECLARATION:
* I used AI https://gemini.google.com/ to learn about the qsort() function in the stdlib.h library
* and how it could be applied to several types of arrays. I used it to learn about the parameters it
* accepts as well as the comparison function, which i used in my code for the qsort() function.
* I integrated the overall structure as well as the logic into my task3's WriteToFile function.
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
int *results[NUM_THREADS];
int prime_count[NUM_THREADS];

//function prototypes
int prime(int num);
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

    if (above_hundred == 1) {   
        WriteToFile("prime_numberst3.txt");
        printf("Prime numbers written to prime_numberst3.txt\n");
    } else {
        WriteToFile(NULL); //pass on null as the fptr because we want to write to stdout
    }

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

//compare function, OBTAINED FROM GEMINI
int compare_function(const void *a, const void *b) {
    int val_a = *(const int *)a;
    int val_b = *(const int *)b;
    return (val_a > val_b) - (val_a < val_b);
}

//new updated writetofile function that handles schedule(guided) segments
void WriteToFile(char *pFilename) {
    int total = 0;
    int position = 0;

    //gets total number of primes found by all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        total += prime_count[i];
    }

    if (total == 0){
        return;
    } 

    //flatten global array into a single array
    int *all = (int *)malloc(total * sizeof(int));
    for (int i = 0; i < NUM_THREADS; i++) {
        for (int j = 0; j < prime_count[i]; j++) {
            all[position] = results[i][j];
            position++;
        }
    }

    //sort the numbers
    qsort(all, total, sizeof(int), compare_function);

    FILE *pFile = stdout;
    if (pFilename != NULL) {
        pFile = fopen(pFilename, "w");
        if (pFile == NULL) {
            printf("Error opening file.\n");
            return;
        }
    }

    //print 25 numbers per line
    for (int i = 0; i < total; i++) {
        fprintf(pFile, "%d", all[i]);
        if (i < total - 1) {
            fprintf(pFile, "%s", ((i + 1) % 25 == 0) ? ",\n" : ", "); //ternary operators to reduce if statements for visibility
        }
    }

    if (pFilename != NULL) {
        fclose(pFile);
    }
}
