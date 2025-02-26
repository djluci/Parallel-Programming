#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "ppmIO.h"

// Struct that passes data to threads
typedef struct {
    Pixel *src;
    int start;
    int end;
} ThreadData;

// Thread function that processes part of the image
void *process_section(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int i = data->start; i < data->end; i++) {
        data->src[i].r = data->src[i].r > 128 ? (220 + data->src[i].r) / 2 : (30 + data->src[i].r) / 2;
        data->src[i].g = data->src[i].g > 128 ? (220 + data->src[i].g) / 2 : (30 + data->src[i].g) / 2;
        data->src[i].b = data->src[i].b > 128 ? (220 + data->src[i].b) / 2 : (30 + data->src[i].b) / 2;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    Pixel *src;
    int rows, cols, colors;
    int num_threads = 1; // Default to 1 thread
    // Checks usage
    if (argc < 3) {
        printf("Usage: %s <image filename> <num_threads (1, 2, or 4)>\n", argv[0]);
        return -1;
    }
    num_threads = atoi(argv[2]);
    if (num_threads != 1 && num_threads != 2 && num_threads != 4) {
        printf("Number of threads must be 1, 2, or 4.\n");
        return -1;
    }
    // Reads image
    src = ppm_read(&rows, &cols, &colors, argv[1]);
    if (!src) {
        printf("Unable to read file %s\n", argv[1]);
        return -1;
    }
    int total_pixels = rows * cols;
    // Starts timer
    clock_t start = clock();

    if (num_threads == 1) {
        // Serial processing
        for (int i = 0; i < total_pixels; i++) {
            src[i].r = src[i].r > 128 ? (220 + src[i].r) / 2 : (30 + src[i].r) / 2;
            src[i].g = src[i].g > 128 ? (220 + src[i].g) / 2 : (30 + src[i].g) / 2;
            src[i].b = src[i].b > 128 ? (220 + src[i].b) / 2 : (30 + src[i].b) / 2;
        }
    } else {
        // Parallel processing
        pthread_t threads[num_threads];
        ThreadData thread_data[num_threads];
        int chunk_size = total_pixels / num_threads;

        for (int i = 0; i < num_threads; i++) {
            thread_data[i].src = src;
            thread_data[i].start = i * chunk_size;
            thread_data[i].end = (i == num_threads - 1) ? total_pixels : (i + 1) * chunk_size;
            pthread_create(&threads[i], NULL, process_section, &thread_data[i]);
        }
        // Join threads
        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    // Ends timer
    clock_t end = clock();
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;
    // Writes output image
    ppm_write(src, rows, cols, colors, "bold.ppm");
    printf("Processing completed in %f seconds using %d thread(s).\n", elapsed_time, num_threads);
    free(src);
    return 0;
}