import threading
import time
from PPMLibrary import PPM

# Thread function to porcess part of the image
def process_pixels(ppm, start_row, end_row):
    for row in range(start_row, end_row):
        for col in range(ppm.cols):
            r, g, b = ppm.get(row, col)
            # invert colors
            ppm.set(row, col, 255 - r, 255 - g, 255 - b)
            
# Main function
def main(filename, num_threads):
    #load image using library
    ppm = PPM(filename)
    
    # divide work among threads
    threads = []
    rows_per_thread = ppm.rows // num_threads

    start_time = time.time()

    for thread_id in range(num_threads):
        start_row = thread_id * rows_per_thread
        end_row = (thread_id + 1) * rows_per_thread if thread_id != num_threads - 1 else ppm.rows
        thread = threading.Thread(target=process_pixels, args=(ppm, start_row, end_row))
        threads.append(thread)
        thread.start()

    # Wait for all threads to finish
    for thread in threads:
        thread.join()

    end_time = time.time()

    # Save the processed image
    output_file = "output_" + str(num_threads) + "_threads.ppm"
    ppm.write(output_file)
    print("Processing completed in {:.6f} seconds using {} thread(s).".format(end_time - start_time, num_threads))
    
if __name__ == "__main__":
    # Test with 1, 2, and 4 threads
    input_file = "IMG_4203.ppm"  # Replace with your .ppm file
    for threads in [1, 2, 4]:
        main(input_file, threads)