/*
* task1.c
* How to run file:
* 1. Open terminal for Lab #1 (Week 4) file directory
* 2. Compile the file using 'gcc -o task1 task1.c'
* 3. Run the .exe using './task1'
*/


#include <stdio.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>

//global variables
int above_hundred;
FILE *fp = NULL;
int *prime_array;
int prime_count = 0;

//function prototypes
int prime(int num);
void WriteToFile(FILE *pArg); 

int main() {
    int n;
    double total_time_taken, compute_time_taken;
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
        fp = fopen("prime_numbers.txt", "w"); //open file for writing, if doesnt exist it will be created 

        if (fp == NULL) {
            printf("Error: Unable to open prime numbers file.\n");
            return 1;
        }
    } else {
        fp = stdout; //if not above 100, write to terimnal
    }

    printf("Start computation\n");
    clock_gettime(CLOCK_MONOTONIC, &startComp); 

    prime_array = malloc(n * sizeof(int)); //allocates memory for array of size n * 4 (integer bytes)
        
    for (int i = 2; i < n; i++) {
        if (prime(i)) {
            prime_array[prime_count] = i;
            prime_count++;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &endComp);
    printf("\nEnd computation\n");

    compute_time_taken = (endComp.tv_sec - startComp.tv_sec) * 1e9; 
    compute_time_taken = (compute_time_taken + (endComp.tv_nsec - startComp.tv_nsec)) * 1e-9;

    printf("\nComputational time: %f seconds\n", compute_time_taken);

    WriteToFile(fp);

    //free the array for memory
    if (prime_count > 0) {
        free(prime_array);
    }

    clock_gettime(CLOCK_MONOTONIC, &endComp);

    //taken from lab week 3
    total_time_taken = (endComp.tv_sec - startComp.tv_sec) * 1e9; 
    total_time_taken = (total_time_taken + (endComp.tv_nsec - startComp.tv_nsec)) * 1e-9; 

    if (above_hundred == 1) {
        fclose(fp); 
        printf("Prime numbers written to prime_numbers.txt\n");
    }

    printf("\nTotal Computational time (inc writing): %f seconds\n", total_time_taken);

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

    for (int j = 0; j < prime_count; j++) {
        if (primes_per_line > 0) {
            fprintf(pArg, "%s", (primes_per_line % 25 == 0) ? ",\n" : ", "); //ternary operators used for reducing clutter
        }
        fprintf(pArg, "%d", prime_array[j]);
        primes_per_line++;
    }
}
