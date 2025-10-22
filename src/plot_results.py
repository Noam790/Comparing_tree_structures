import pandas as pd
import sys
import matplotlib.pyplot as plt
import os
import numpy as np
from matplotlib.ticker import LogLocator

csv_path = sys.argv[1]
png_path = os.path.join(os.path.dirname(csv_path), "time_complexity_plot.png")

df = pd.read_csv(csv_path)

n_values = df["n"].values

# log(0) undefined so no plot for the first lists, replaced by 1e-6
insert_times = np.maximum(df["insert_time"].values, 1e-6)
delete_times = np.maximum(df["delete_time"].values, 1e-6)

# f(x) = x (normalized)
ref_line = insert_times[-1] * (n_values / n_values[-1])

plt.figure(figsize=(12, 7))

plt.plot(n_values, insert_times, marker='o', label="Insertion", color='blue')
plt.plot(n_values, delete_times, marker='x', label="Deletion", color='red')
plt.plot(n_values, ref_line, linestyle='--', color='gray', label="f(n) = n")

plt.xscale("log")
plt.yscale("log")

plt.xticks(n_values, [str(n) for n in n_values], rotation=45)
plt.gca().xaxis.set_major_locator(LogLocator(base=10.0, subs=np.arange(1.0, 10.0)*0.1, numticks=20))
plt.gca().yaxis.set_major_locator(LogLocator(base=10.0, subs=np.arange(1.0, 10.0)*0.1, numticks=20))

plt.xlabel("Number of elements (n)")
plt.ylabel("Time (seconds)")
plt.title("Time Complexity of Red-Black Tree Insertions and Deletions")
plt.legend()
plt.grid(True, which="both", ls="--", lw=0.5)

plt.tight_layout()
plt.savefig(png_path)
plt.show()
