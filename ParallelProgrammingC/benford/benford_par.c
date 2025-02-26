/**
 * Duilio Lucio
 * 12/04/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "my_timing.h"

// Global variables 
int global_counts[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
pthread_mutex_t locks[10]; // an array of mutex locks
int N = 0; // number of data points
double *data; // data array

// load data from a binary file
int loadData(char *filename) {
    FILE *fp;
    if (filename != NULL && strlen(filename))
        fp = fopen(filename, "r");
    else
        return -1;
    if (!fp)
        return -1;

    fread(&N, sizeof(int), 1, fp);
    data = (double *)malloc(sizeof(double) * N);
    if (!data) {
        fprintf(stderr, "Failed to allocate memory for data\n");
        fclose(fp);
        return -1;
    }
    fread(data, sizeof(double), N, fp);
    fclose(fp);
    return 1; // success
}

// function to determine the leading digit of a number
int leadingDigit(double n) {
    if (fabs(n) < 1.0) {
        double tmp = fabs(n);
        while (tmp < 1.0)
            tmp *= 10;
        return (int)floor(tmp);
    }
    else {
        long long unsigned in = (long long unsigned)floor(fabs(n));
        while (in > 9)
            in /= 10;
        return in;
    }
}

// Thread function
void *processSection(void *arg) {
    int thread_id = *(int *)arg;
    free(arg);
    int start_idx = (N / 8) * thread_id;
    int end_idx = (thread_id == 7) ? N : (N / 8) * (thread_id + 1);

    for (int i = start_idx; i < end_idx; i++) {
        int d = leadingDigit(data[i]);
        pthread_mutex_lock(&locks[d]);
        global_counts[d]++;
        pthread_mutex_unlock(&locks[d]);
    }
    pthread_exit(NULL);
}

// main function
int main(int argc, char *argv[]) {
    pthread_t threads[8];
    int thread_ids[8];
    double t1, t2;

    // Initialize mutexes
    for (int i = 0; i < 10; i++) {
        if (pthread_mutex_init(&locks[i], NULL) != 0) {
            fprintf(stderr, "Failed to initialize mutex %d\n", i);
            return -1;
        }
    }
    // Load data
    int succ = loadData("medium.bin");
    if (!succ){
        fprintf(stderr ,"Failed to load data\n");
        return -1;
    }
    // start timer
    t1 = get_time_sec();
    // create threads
    for (int i = 0; i < 8; i++) {
        int *thread_arg = malloc(sizeof(int)); // Dynamically allocate memory for each thread argument
        if (!thread_arg) {
            fprintf(stderr, "Failed to allocate memory for thread argument %d\n", i);
            return -1;
        }
        *thread_arg = i; // Set the thread ID
        pthread_create(&threads[i], NULL, processSection, thread_arg);
    }
    // join threads
    for (int i = 0; i < 8; i++){
        pthread_join(threads[i], NULL);
    }
    // end timer
    t2 = get_time_sec();
    // print results
    for (int i = 1; i < 10; i++) {
        printf("There are %d %d's\n", global_counts[i], i);
    }
    printf("It took %f seconds for it to run\n", t2 - t1);
    // free resources
    free(data);
    for(int i = 0; i < 10; i++){
        pthread_mutex_destroy(&locks[i]);
    }
    return 0;
}