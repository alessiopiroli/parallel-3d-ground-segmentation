import matplotlib.pyplot as plt
import statistics
import os

def plot_timed_runs(thread_values, group, title, directory):
    plt.figure()
    for label, run_times in group.items():
        plt.plot(thread_values, run_times, marker='o', label=label)
    plt.xlabel("Number of Threads")
    plt.ylabel("Average run times [ms]")
    plt.title(title)
    plt.grid(True, which="both", ls="--", linewidth=0.5)
    xmin, xmax = plt.xlim()
    ymin, ymax = plt.ylim()
    plt.ylim(ymin, ymax + (ymax - ymin) * 0.2)
    plt.xticks(thread_values)
    plt.legend(loc='upper left', fontsize=8)
    file_path = os.path.join(directory, f"{title.replace(' ', '_')}.pdf")
    plt.savefig(file_path, format="pdf", bbox_inches='tight')
    plt.show(block=True)

N_THREADS = [1, 2, 4, 8, 12, 16]

# COMPLETE FUNCTION
# N_BINS = 1
seq_full_1_avg = statistics.mean([30.1193, 30.7788, 29.5829, 30.3082, 30.5125, 30.443])
seq_full_1_vec = [seq_full_1_avg, seq_full_1_avg, seq_full_1_avg, seq_full_1_avg, seq_full_1_avg, seq_full_1_avg]
par_full_1_vec = [30.8133, 25.696, 25.3933, 24.9176, 25.5337, 28.4327]

# N_BINS = 10
seq_full_10_avg = statistics.mean([33.1974, 34.2456, 40.4427, 33.5749, 33.8074, 37.6991])
seq_full_10_vec = [seq_full_10_avg, seq_full_10_avg, seq_full_10_avg, seq_full_10_avg, seq_full_10_avg, seq_full_10_avg]
par_full_10_vec = [32.5195, 31.1013, 31.9696, 24.5339, 32.9904, 45.0163]

# N_BINS = 50
seq_full_50_avg = statistics.mean([46.2602, 40.1311, 46.9259, 39.6457, 47.7139, 44.1569])
seq_full_50_vec = [seq_full_50_avg, seq_full_50_avg, seq_full_50_avg, seq_full_50_avg, seq_full_50_avg, seq_full_50_avg]
par_full_50_vec = [46.3097, 32.2045, 34.5606, 27.2757, 40.3934, 52.9466]

# N_BINS = 100
seq_full_100_avg = statistics.mean([55.6855, 46.6796, 46.6274, 55.8556, 55.6062, 46.8775])
seq_full_100_vec = [seq_full_100_avg, seq_full_100_avg, seq_full_100_avg, seq_full_100_avg, seq_full_100_avg, seq_full_100_avg]
par_full_100_vec = [52.7188, 37.3439, 30.5145, 43.9723, 46.4898, 53.1083]

run_1_full = {
    "Number of bins = 1, Sequential Full Function duration": seq_full_1_vec,
    "Number of bins = 1, Parallel Full Function duration": par_full_1_vec
}

run_10_full = {
    "Number of bins = 10, Sequential Full Function duration": seq_full_10_vec,
    "Number of bins = 10, Parallel Full Function duration": par_full_10_vec
}

run_50_full = {
    "Number of bins = 50, Sequential Full Function duration": seq_full_50_vec,
    "Number of bins = 50, Parallel Full Function duration": par_full_50_vec
}

run_100_full = {
    "Number of bins = 100, Sequential Full Function duration": seq_full_100_vec,
    "Number of bins = 100, Parallel Full Function duration": par_full_100_vec
}

# ASSIGN PROTOTYPE POINTS
# N_BINS = 1
seq_f2_1_avg = statistics.mean([1.1686, 1.18131, 1.14602, 1.17317, 1.15872, 1.15706])
seq_f2_1_vec = [seq_f2_1_avg, seq_f2_1_avg, seq_f2_1_avg, seq_f2_1_avg, seq_f2_1_avg, seq_f2_1_avg]
par_f2_1_vec = [1.20625, 0.601607, 0.363848, 0.291766, 0.21347, 0.887104]

# N_BINS = 10
seq_f2_10_avg = statistics.mean([1.26312, 1.31732, 1.50555, 1.26883, 1.27315, 1.40885])
seq_f2_10_vec = [seq_f2_10_avg, seq_f2_10_avg, seq_f2_10_avg, seq_f2_10_avg, seq_f2_10_avg, seq_f2_10_avg]
par_f2_10_vec = [1.22871, 0.7595, 0.46862, 0.29964, 0.349484, 4.3866]

# N_BINS = 50
seq_f2_50_avg = statistics.mean([1.63916, 1.46548, 1.70149, 1.40882, 1.69021, 1.5623])
seq_f2_50_vec = [seq_f2_50_avg, seq_f2_50_avg, seq_f2_50_avg, seq_f2_50_avg, seq_f2_50_avg, seq_f2_50_avg]
par_f2_50_vec = [1.62125, 0.76179, 0.558759, 0.347279, 0.360495, 4.96561]

# N_BINS = 100
seq_f2_100_avg = statistics.mean([1.93777, 1.60621, 1.61063, 1.92406, 1.92734, 1.62845])
seq_f2_100_vec = [seq_f2_100_avg, seq_f2_100_avg, seq_f2_100_avg, seq_f2_100_avg, seq_f2_100_avg, seq_f2_100_avg]
par_f2_100_vec = [1.78012, 0.835977, 0.475597, 0.531229, 0.427995, 4.32706]

run_1_f2 = {
    "Number of bins = 1, Sequential Assign Prototype Points duration": seq_f2_1_vec,
    "Number of bins = 1, Parallel Assign Prototype Points duration": par_f2_1_vec
}

run_10_f2 = {
    "Number of bins = 10, Sequential FAssign Prototype Points duration": seq_f2_10_vec,
    "Number of bins = 10, Parallel Assign Prototype Points duration": par_f2_10_vec
}

run_50_f2 = {
    "Number of bins = 50, Sequential Assign Prototype Points duration": seq_f2_50_vec,
    "Number of bins = 50, Parallel Assign Prototype Points duration": par_f2_50_vec
}

run_100_f2 = {
    "Number of bins = 100, Sequential Assign Prototype Points duration": seq_f2_100_vec,
    "Number of bins = 100, Parallel Assign Prototype Points duration": par_f2_100_vec
}

full_directory = "/home/alessio/workspace/tum/par-comp/parallel-3d-ground-segmentation/performances/plots/experiment_3/complete_function"
f2_app_directory = "/home/alessio/workspace/tum/par-comp/parallel-3d-ground-segmentation/performances/plots/experiment_3/prototype_points"

# PLOTTING FULL FUNCTIONS DURATIONS
plot_timed_runs(N_THREADS, run_1_full, "Number of bins = 1, Full Function average run times [ms]", full_directory)
plot_timed_runs(N_THREADS, run_10_full, "Number of bins = 10, Full Function average run times [ms]", full_directory)
plot_timed_runs(N_THREADS, run_50_full, "Number of bins = 50, Full Function average run times [ms]", full_directory)
plot_timed_runs(N_THREADS, run_100_full, "Number of bins = 100, Full Function average run times [ms]", full_directory)

# PLOTTING F2: ASSIGN PROTOTYPE POINTS DURATIONS
plot_timed_runs(N_THREADS, run_1_f2, "Number of bins = 1, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_10_f2, "Number of bins = 10, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_50_f2, "Number of bins = 50, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_100_f2, "Number of bins = 100, Assign Prototype Points average run times [ms]", f2_app_directory)