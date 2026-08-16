#include <stdio.h> 
#include <math.h>
#include <stdlib.h>
#include <time.h>

int above_hundred;
FILE *fp = NULL;

//function prototype 
int prime(int num); 

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
        fprintf(fp, "2"); //just for 2
        for (int i = 3; i < n; i += 2) { //skip even numbers, they divisible by 2 and not prime
            if (prime(i)) {
                fprintf(fp, ", %d", i);
            }
        }
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

// // Main function (yes counter, only uncomment if ur number is ridiculously high and u want to see how many primes are in the file)
// int main() {
//     int n;
//     double time_taken;
//     int count; //this is purely for making the txt file not crash when i open it, its to count how many primes per line
//     struct timespec startComp, endComp;

//     printf("Enter number:\n");
//     scanf("%d", &n);

//     //checks if we need to write into a file or not
//     if (n >= 100) {
//         above_hundred = 1; //c dont allow boolean unless library, 0 is false and any non-zero is true
//     } else {
//         above_hundred = 0;
//     }

//     if (above_hundred == 1) {   
//         fp = fopen("prime_numbers.txt", "w"); //open file for writing, if doesnt exist it will be created 

//         if (fp == NULL) {
//             printf("Error: Unable to open prime numbers file.\n");
//             return 1;
//         }
//     } else {
//         fp = stdout; //if not above 100, write to terimnal
//     }

//     printf("Start computation\n");
//     clock_gettime(CLOCK_MONOTONIC, &startComp); 

//     if (n <= 2) {
//         fprintf(fp, "No prime numbers found.\n");
//     } else {
//         fprintf(fp, "2"); //just for 2
//         int count = 1;
//         for (int i = 3; i < n; i += 2) {  //skip even numbers, they divisible by 2 and not prime
//             if (prime(i)) {
//                 fprintf(fp, ",");
//                 if (count % 20 == 0) {
//                     fprintf(fp, "\n");  //bnew line every 20 primes
//                 } else {
//                     fprintf(fp, " ");
//                 }
//                 fprintf(fp, "%d", i);
//                 count++;
//             }
//         }
//     }

//     clock_gettime(CLOCK_MONOTONIC, &endComp);

//     //taken from lab week 3
//     time_taken = (endComp.tv_sec - startComp.tv_sec) * 1e9; 
//     time_taken = (time_taken + (endComp.tv_nsec - startComp.tv_nsec)) * 1e-9; 

//     if (above_hundred == 1) {
//         fclose(fp); 
//         printf("Prime numbers written to prime_numbers.txt\n");
//     }

//     printf("\nEnd computation\n");
//     printf("\nComputational time: %f seconds\n", time_taken);

//     return 0;
// }

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
