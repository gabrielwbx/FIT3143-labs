#include <stdio.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>

int above_hundred;
FILE *fp = NULL;
int *prime_array;
int prime_count = 0;

//function prototype 
int prime(int num);
void WriteToFile(FILE *pArg); 

// Main function (no counter)
int main() {
    int n;
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

    if (n <= 2) {
        fprintf(fp, "No prime numbers found.\n");
    } else {
        prime_array = malloc(n * sizeof(int));
        
        prime_array[prime_count] = 2;
        prime_count++;
        
        for (int i = 3; i < n; i += 2) { //skip even numbers, they divisible by 2 and not prime
            if (prime(i)) {
                prime_array[prime_count] = i;
                prime_count++;
            }
        }
    }

    WriteToFile(fp);

    if (prime_count > 0) {
        free(prime_array);
    }

    clock_gettime(CLOCK_MONOTONIC, &endComp);

    //taken from lab week 3
    time_taken = (endComp.tv_sec - startComp.tv_sec) * 1e9; 
    time_taken = (time_taken + (endComp.tv_nsec - startComp.tv_nsec)) * 1e-9; 

    if (above_hundred == 1) {
        fclose(fp); 
        printf("Prime numbers written to prime_numbers.txt\n");
    }

    printf("\nEnd computation\n");
    printf("\nComputational time: %f seconds\n", time_taken);

    return 0;
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

    for (int j = 0; j < prime_count; j++) {
        if (primes_per_line > 0) {
            fprintf(pArg, "%s", (primes_per_line % 25 == 0) ? ",\n" : ", ");
        }
        fprintf(pArg, "%d", prime_array[j]);
        primes_per_line++;
    }
}
