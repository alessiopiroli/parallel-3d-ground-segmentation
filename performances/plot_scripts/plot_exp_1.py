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

# 25% OF TOTAL DATA FULL_FUNCTION
seq_25_full_avg = statistics.mean([18.8507, 18.8156, 18.8937, 19.1844, 18.9632, 19.2608])

seq_25_full_vec = [seq_25_full_avg, seq_25_full_avg, seq_25_full_avg, seq_25_full_avg, seq_25_full_avg, seq_25_full_avg]
par_25_full_vec = [19.599, 18.1477, 17.1181, 16.7943, 18.3029, 25.986]


# 50% OF TOTAL DATA FULL_FUNCTION
seq_50_full_avg = statistics.mean([26.3253, 25.4791, 25.5308, 25.8435, 29.0655, 25.704])
seq_50_full_vec = [seq_50_full_avg, seq_50_full_avg, seq_50_full_avg, seq_50_full_avg, seq_50_full_avg, seq_50_full_avg]
par_50_full_vec = [26.7413, 21.5906, 20.3689, 20.159, 34.3942, 36.3881]

# 75% OF TOTAL DATA FULL_FUNCTION
seq_75_full_avg = statistics.mean([31.9094, 31.5069, 32.8599, 32.9812, 33.6187, 32.2035])
seq_75_full_vec = [seq_75_full_avg, seq_75_full_avg, seq_75_full_avg, seq_75_full_avg, seq_75_full_avg, seq_75_full_avg]
par_75_full_vec = [31.6811, 25.7223, 23.8614, 22.7552, 32.7484, 32.3546]

# 100% OF TOTAL DATA FULL_FUNCTION
seq_100_full_avg = statistics.mean([39.1859, 38.8087, 38.9317, 43.5203, 39.2036, 39.2779])
seq_100_full_vec = [seq_100_full_avg, seq_100_full_avg, seq_100_full_avg, seq_100_full_avg, seq_100_full_avg, seq_100_full_avg]
par_100_full_vec = [39.7348, 34.1626, 36.4128, 36.6172, 27.3748, 53.3165]

run_25_full = {
    "25% of data: Complete function sequential duration": seq_25_full_vec,
    "25% of data: Complete function parallel duration": par_25_full_vec
}

run_50_full = {
    "50% of data: Complete function sequential duration": seq_50_full_vec,
    "50% of data: Complete function parallel duration": par_50_full_vec
}

run_75_full = {
    "75% of data: Complete function sequential duration": seq_75_full_vec,
    "75% of data: Complete function parallel duration": par_75_full_vec
}

run_100_full = {
    "100% of data: Complete function sequential duration": seq_100_full_vec,
    "100% of data: Complete function parallel duration": par_100_full_vec
}


# 25% OF TOTAL DATA F4 - GPC
seq_25_f4_avg = statistics.mean([2.79414, 2.795, 2.8306, 2.86354, 2.83391, 2.83704])
seq_25_f4_vec = [seq_25_f4_avg, seq_25_f4_avg, seq_25_f4_avg, seq_25_f4_avg, seq_25_f4_avg, seq_25_f4_avg]
par_25_f4_vec = [2.84793, 1.4404, 1.12446, 0.616711, 0.415177, 1.61763]

# 50% OF TOTAL DATA F4 - GPC
seq_50_f4_avg = statistics.mean([5.85345, 5.63632, 5.71039, 5.71097, 6.37669, 5.7315])
seq_50_f4_vec = [seq_50_f4_avg, seq_50_f4_avg, seq_50_f4_avg, seq_50_f4_avg, seq_50_f4_avg, seq_50_f4_avg]
par_50_f4_vec = [5.94588, 2.97397, 1.93432, 1.25409, 1.13044, 2.42759]

# 75% OF TOTAL DATA F4 - GPC
seq_75_f4_avg = statistics.mean([8.55292, 8.52842, 8.60705, 8.80176, 8.89629, 8.57056])
seq_75_f4_vec = [seq_75_f4_avg, seq_75_f4_avg, seq_75_f4_avg, seq_75_f4_avg, seq_75_f4_avg, seq_75_f4_avg]
par_75_f4_vec = [8.48606, 4.3757, 2.53774, 1.87029, 1.48362, 2.06895]

# 100% OF TOTAL DATA F4 - GPC
seq_100_f4_avg = statistics.mean([11.8388, 11.7955, 11.8497, 13.1282, 11.8584, 11.9017])
seq_100_f4_vec = [seq_100_f4_avg, seq_100_f4_avg, seq_100_f4_avg, seq_100_f4_avg, seq_100_f4_avg, seq_100_f4_avg]
par_100_f4_vec = [11.7317, 6.62458, 4.55039, 2.9715, 1.67035, 4.66089]

run_25_f4 = {
    "25% of data: Ground Point Classfication sequential duration": seq_25_f4_vec,
    "25% of data: Ground Point Classfication parallel duration": par_25_f4_vec
}

run_50_f4 = {
    "50% of data: Ground Point Classfication sequential duration": seq_50_f4_vec,
    "50% of data: Ground Point Classfication parallel duration": par_50_f4_vec
}

run_75_f4 = {
    "75% of data: Ground Point Classfication sequential duration": seq_75_f4_vec,
    "75% of data: Ground Point Classfication parallel duration": par_75_f4_vec
}

run_100_f4 = {
    "100% of data: Ground Point Classfication sequential duration": seq_100_f4_vec,
    "100% of data: Ground Point Classfication parallel duration": par_100_f4_vec
}

# 25% OF TOTAL DATA F1 - SSB
seq_25_f1_avg = statistics.mean([1.66744, 1.65692, 1.67515, 1.70324, 1.68411, 1.70289])
seq_25_f1_vec = [seq_25_f1_avg, seq_25_f1_avg, seq_25_f1_avg, seq_25_f1_avg, seq_25_f1_avg, seq_25_f1_avg]
par_25_f1_vec = [1.92226, 1.30519, 1.23284, 1.42609, 1.78604, 2.75539]

# 50% OF TOTAL DATA F1 - SSB
seq_50_f1_avg = statistics.mean([3.23065, 3.08245, 3.1178, 3.15618, 3.5004, 3.13932])
seq_50_f1_vec = [seq_50_f1_avg, seq_50_f1_avg, seq_50_f1_avg, seq_50_f1_avg, seq_50_f1_avg, seq_50_f1_avg]
par_50_f1_vec = [3.6248, 2.17509, 1.88248, 1.84298, 4.52518, 3.88602]

# 75% OF TOTAL DATA F1 - SSB
seq_75_f1_avg = statistics.mean([4.54726, 4.53534, 4.59338, 4.6558, 4.83445, 4.56143])
seq_75_f1_vec = [seq_75_f1_avg, seq_75_f1_avg, seq_75_f1_avg, seq_75_f1_avg, seq_75_f1_avg, seq_75_f1_avg]
par_75_f1_vec = [4.81071, 3.02316, 2.35688, 2.26366, 3.88844, 3.5591]

# 100% OF TOTAL DATA F1 - SSB
seq_100_f1_avg = statistics.mean([6.18661, 6.03597, 6.08403, 6.74801, 6.12641, 6.17132])
seq_100_f1_vec = [seq_100_f1_avg, seq_100_f1_avg, seq_100_f1_avg, seq_100_f1_avg, seq_100_f1_avg, seq_100_f1_avg]
par_100_f1_vec = [6.65493, 4.3123, 3.96728, 4.20657, 2.975, 6.33479]

run_25_f1 = {
    "25% of data: Sorting in Segments and Bins sequential duration": seq_25_f1_vec,
    "25% of data: Sorting in Segments and Bins parallel duration": par_25_f1_vec
}

run_50_f1 = {
    "50% of data: Sorting in Segments and Bins sequential duration": seq_50_f1_vec,
    "50% of data: Sorting in Segments and Bins parallel duration": par_50_f1_vec
}

run_75_f1 = {
    "75% of data: Sorting in Segments and Bins sequential duration": seq_75_f1_vec,
    "75% of data: Sorting in Segments and Bins parallel duration": par_75_f1_vec
}

run_100_f1 = {
    "100% of data: Sorting in Segments and Bins sequential duration": seq_100_f1_vec,
    "100% of data: Sorting in Segments and Bins parallel duration": par_100_f1_vec
}

full_func_directory = "/home/alessio/workspace/tum/par-comp/parallel-3d-ground-segmentation/performances/plots/experiment_1/complete_function"
f4_gpc_directory = "/home/alessio/workspace/tum/par-comp/parallel-3d-ground-segmentation/performances/plots/experiment_1/par_ground_point_classification"
f1_seg_bin_directory = "/home/alessio/workspace/tum/par-comp/parallel-3d-ground-segmentation/performances/plots/experiment_1/seg_and_bin_sorting"