import matplotlib.pyplot as plt

# Data: Computation times for 1, 2, and 4 threads
threads = [1, 2, 4]
times = [0.012849, 0.014067, 0.016058]

# bar chart
plt.figure(figsize=(8, 5))
plt.bar(threads, times, color=['blue', 'orange', 'green'], alpha=0.7, width=0.6)
plt.xlabel("Number of Threads")
plt.ylabel("Computation Time (s)")
plt.title("Computation Time vs. Number of Threads")
plt.xticks(threads)
plt.tight_layout()

# Saves chart
plt.savefig("computation_time_vs_threads.png")
plt.show()