import matplotlib.pyplot as plt

# Data: Execution times for different program versions (mean of middle three runs)
versions = [
    "Sequential",
    "Global Mutex",
    "Array of Mutexes",
    "Local Counters + Single Mutex",
    "Local Counters + Array of Mutexes",
    "No Mutex (Array of Arrays)"
]
mean_times = [0.000562, 0.003065, 0.002808, 0.002900, 0.002748, 0.002190]

# bar chart
plt.figure(figsize=(10, 6))
bars = plt.bar(versions, mean_times, color=['blue', 'orange', 'orange', 'orange', 'orange', 'green'], alpha=0.7)

# add value annotations on the bars
for bar, time in zip(bars, mean_times):
    plt.text(bar.get_x() + bar.get_width() / 2, bar.get_height(), f"{time:.6f} s", ha='center', va='bottom', fontsize=9)
    

plt.xlabel("Program Version")
plt.ylabel("Mean Execution Time (s)")
plt.title("Execution Time Comparison for Different Program Versions")
plt.xticks(rotation=45, ha="right")
plt.tight_layout()

# Saves chart as an image
plt.savefig("execution_time_comparison.png")
plt.show()