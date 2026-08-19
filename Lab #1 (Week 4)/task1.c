/*
* Student 1: Gabriel Wong Bo Xuen 34496114 gwon0021@student.monash.edu
* Student 2: Nicholas Ho Khor Pei 34496009 nhoo0025@student.monash.edu
* task1.c
* this file contains the serial implementation of the prime number getter up to input N
* How to run file:
* 1. Open terminal for Lab #1 (Week 4) file directory
* 2. Compile the file using 'gcc task1.c -o a'
* 3. Run the .exe using './a'
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
void WriteToFile(char *pFilename); 

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

    if (above_hundred == 1) {
        WriteToFile("prime_numberst1.txt"); 
        printf("Prime numbers written to prime_numbers1.txt\n");
    } else {
        WriteToFile(NULL);
    }

    //free the array for memory
    if (prime_count > 0) {
        free(prime_array);
    }

    clock_gettime(CLOCK_MONOTONIC, &endComp);

    //taken from lab week 3
    total_time_taken = (endComp.tv_sec - startComp.tv_sec) * 1e9; 
    total_time_taken = (total_time_taken + (endComp.tv_nsec - startComp.tv_nsec)) * 1e-9; 

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
void WriteToFile(char *pFilename) {
    if (prime_count == 0) {
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

    for (int i = 0; i < prime_count; i++) {
        fprintf(pFile, "%d", prime_array[i]);
        if (i < prime_count - 1) {
            fprintf(pFile, "%s", ((i + 1) % 25 == 0) ? ",\n" : ", ");//ternary operators to reduce if statements for visibility
        }
    }

    if (pFilename != NULL) {
        fclose(pFile);
    }
}
