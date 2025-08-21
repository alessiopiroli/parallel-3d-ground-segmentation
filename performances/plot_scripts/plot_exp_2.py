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

# FUNCTION 2: ASSIGN PROTOTYPE POINTS
# N_SEGMENTS = 1
seq_f2_1_avg = statistics.mean([1.12747, 1.14445, 1.13767, 1.13313, 1.14452, 1.12588])
seq_f2_1_vec = [seq_f2_1_avg, seq_f2_1_avg, seq_f2_1_avg, seq_f2_1_avg, seq_f2_1_avg, seq_f2_1_avg]
par_f2_1_vec = [1.17354, 1.19612, 1.23579, 1.39207, 1.25932, 2.55714]

# N_SEGMENTS = 10
seq_f2_10_avg = statistics.mean([1.18688, 1.16112, 1.16959, 1.20197, 1.13469, 1.14879])
seq_f2_10_vec = [seq_f2_10_avg, seq_f2_10_avg, seq_f2_10_avg, seq_f2_10_avg, seq_f2_10_avg, seq_f2_10_avg]
par_f2_10_vec = [1.36327, 0.608997, 0.391184, 0.292014, 0.230763, 2.4589]

# N_SEGMENTS = 50
seq_f2_50_avg = statistics.mean([1.29165, 1.26364, 1.42447, 1.30937, 1.26958, 1.27129])
seq_f2_50_vec = [seq_f2_50_avg, seq_f2_50_avg, seq_f2_50_avg, seq_f2_50_avg, seq_f2_50_avg, seq_f2_50_avg]
par_f2_50_vec = [1.27881, 0.65004, 0.662987, 0.332466, 0.220598, 4.21605]

# N_SEGMENTS = 100
seq_f2_100_avg = statistics.mean([1.32823, 1.33081, 1.34473, 1.37542, 1.52984, 1.38649])
seq_f2_100_vec = [seq_f2_100_avg, seq_f2_100_avg, seq_f2_100_avg, seq_f2_100_avg, seq_f2_100_avg, seq_f2_100_avg]
par_f2_100_vec = [1.36022, 0.684372, 0.375012, 0.820409, 0.309451, 1.18883]

# N_SEGMENTS = 250
seq_f2_250_avg = statistics.mean([1.78513, 1.58147, 1.60698, 1.62865, 1.99087, 1.56811])
seq_f2_250_vec = [seq_f2_250_avg, seq_f2_250_avg, seq_f2_250_avg, seq_f2_250_avg, seq_f2_250_avg, seq_f2_250_avg]
par_f2_250_vec = [1.74014, 0.825684, 0.472251, 0.434043, 0.353506, 3.53108]

# N_SEGMENTS = 500
seq_f2_500_avg = statistics.mean([1.8775, 1.84835, 1.89563, 1.86833, 1.87326, 1.87243])
seq_f2_500_vec = [seq_f2_500_avg, seq_f2_500_avg, seq_f2_500_avg, seq_f2_500_avg, seq_f2_500_avg, seq_f2_500_avg]
par_f2_500_vec = [2.13818, 1.15513, 0.566731, 0.397502, 0.307995, 7.98704]

# N_SEGMENTS = 750
seq_f2_750_avg = statistics.mean([2.03425, 2.5432, 2.09672, 2.07012, 2.10039, 2.11086])
seq_f2_750_vec = [seq_f2_750_avg, seq_f2_750_avg, seq_f2_750_avg, seq_f2_750_avg, seq_f2_750_avg, seq_f2_750_avg]
par_f2_750_vec = [2.01691, 1.24549, 0.594884, 0.693532, 2.42259, 11.7896]

# N_SEGMENTS = 1000
seq_f2_1000_avg = statistics.mean([2.54257, 2.25518, 2.24823, 2.22181, 2.20819, 2.56706])
seq_f2_1000_vec = [seq_f2_1000_avg, seq_f2_1000_avg, seq_f2_1000_avg, seq_f2_1000_avg, seq_f2_1000_avg, seq_f2_1000_avg]
par_f2_1000_vec = [2.31966, 1.15775, 0.747171, 0.611349, 0.446616, 11.336]

run_1_f2 = {
    "Number of segments = 1, Sequential Assign Prototype Points function duration": seq_f2_1_vec,
    "Number of segments = 1, Parallel Assign Prototype Points durations": par_f2_1_vec
}

run_10_f2 = {
    "Number of segments = 10, Sequential Assign Prototype Points function duration": seq_f2_10_vec,
    "Number of segments = 10, Parallel Assign Prototype Points function durations": par_f2_10_vec
}

run_50_f2 = {
    "Number of segments = 50, Sequential Assign Prototype Points function duration": seq_f2_50_vec,
    "Number of segments = 50, Parallel Assign Prototype Points function durations": par_f2_50_vec
}

run_100_f2 = {
    "Number of segments = 100, Sequential Assign Prototype Points function duration": seq_f2_100_vec,
    "Number of segments = 100, Parallel Assign Prototype Points function durations": par_f2_100_vec
}

run_250_f2 = {
    "Number of segments = 250, Sequential Assign Prototype Points function duration": seq_f2_250_vec,
    "Number of segments = 250, Parallel Assign Prototype Points function durations": par_f2_250_vec
}

run_500_f2 = {
    "Number of segments = 500, Sequential Assign Prototype Points function duration": seq_f2_500_vec,
    "Number of segments = 500, Parallel Assign Prototype Points function durations": par_f2_500_vec
}

run_750_f2 = {
    "Number of segments = 750, Sequential Assign Prototype Points function duration": seq_f2_750_vec,
    "Number of segments = 750, Parallel Assign Prototype Points function durations": par_f2_750_vec
}

run_1000_f2 = {
    "Number of segments = 1000, Sequential Assign Prototype Points function duration": seq_f2_1000_vec,
    "Number of segments = 1000, Parallel Assign Prototype Points function durations": par_f2_1000_vec
}

# FUNCTION 3: FIT LINES
# N_SEGMENTS = 1
seq_f3_1_avg = statistics.mean([0.0060065, 0.0062211, 0.0058468, 0.0059555, 0.00614305, 0.00623335, ])
seq_f3_1_vec = [seq_f3_1_avg, seq_f3_1_avg, seq_f3_1_avg, seq_f3_1_avg, seq_f3_1_avg, seq_f3_1_avg]
par_f3_1_vec = [0.00643945, 0.0060785, 0.0058756, 0.0072496, 0.00833105, 0.757877]

# N_SEGMENTS = 10
seq_f3_10_avg = statistics.mean([0.0256652, 0.0250659, 0.0252204, 0.02628, 0.0242519, 0.0248538])
seq_f3_10_vec = [seq_f3_10_avg, seq_f3_10_avg, seq_f3_10_avg, seq_f3_10_avg, seq_f3_10_avg, seq_f3_10_avg]
par_f3_10_vec = [0.0291099, 0.0157184, 0.0132137, 0.0111815, 0.0106115, 0.960489]

# N_SEGMENTS = 50
seq_f3_50_avg = statistics.mean([0.0837224, 0.083528, 0.0946446, 0.0870314, 0.0837458, 0.0838738])
seq_f3_50_vec = [seq_f3_50_avg, seq_f3_50_avg, seq_f3_50_avg, seq_f3_50_avg, seq_f3_50_avg, seq_f3_50_avg]
par_f3_50_vec = [0.0859137, 0.0470904, 0.0654395, 0.0409427, 0.0232186, 2.2192]

# N_SEGMENTS = 100
seq_f3_100_avg = statistics.mean([0.143662, 0.14556, 0.144454, 0.149649, 0.166652, 0.149207])
seq_f3_100_vec = [seq_f3_100_avg, seq_f3_100_avg, seq_f3_100_avg, seq_f3_100_avg, seq_f3_100_avg, seq_f3_100_avg]
par_f3_100_vec = [0.14876, 0.0757072, 0.0483516, 0.0657528, 0.0553366, 0.408405]

# N_SEGMENTS = 250
seq_f3_250_avg = statistics.mean([0.350713, 0.31305, 0.307785, 0.318032, 0.382243, 0.311454])
seq_f3_250_vec = [seq_f3_250_avg, seq_f3_250_avg, seq_f3_250_avg, seq_f3_250_avg, seq_f3_250_avg, seq_f3_250_avg]
par_f3_250_vec = [0.352607, 0.171894, 0.113572, 0.12218, 0.120465, 2.15117]

# N_SEGMENTS = 500
seq_f3_500_avg = statistics.mean([0.580218, 0.564118, 0.566983, 0.564819, 0.562571, 0.569979])
seq_f3_500_vec = [seq_f3_500_avg, seq_f3_500_avg, seq_f3_500_avg, seq_f3_500_avg, seq_f3_500_avg, seq_f3_500_avg]
par_f3_500_vec = [0.654674, 0.382614, 0.198605, 0.198396, 0.163911, 3.96131]

# N_SEGMENTS = 750
seq_f3_750_avg = statistics.mean([0.802414, 0.988645, 0.817392, 0.811146, 0.811593, 0.825594])
seq_f3_750_vec = [seq_f3_750_avg, seq_f3_750_avg, seq_f3_750_avg, seq_f3_750_avg, seq_f3_750_avg, seq_f3_750_avg]
par_f3_750_vec = [0.815364, 0.54747, 0.291347, 0.422353, 0.465475, 5.85641]

# N_SEGMENTS = 1000
seq_f3_1000_avg = statistics.mean([1.18753, 1.18753, 1.04177, 1.03388, 1.02855, 1.18253])
seq_f3_1000_vec = [seq_f3_1000_avg, seq_f3_1000_avg, seq_f3_1000_avg, seq_f3_1000_avg, seq_f3_1000_avg, seq_f3_1000_avg]
par_f3_1000_vec = [1.13664, 0.613348, 0.398772, 0.485409, 0.321594, 4.921]

run_1_f3 = {
    "Number of segments = 1, Sequential Fit Lines function duration": seq_f3_1_vec,
    "Number of segments = 1, Parallel Fit Lines durations": par_f3_1_vec
}

run_10_f3 = {
    "Number of segments = 10, Sequential Fit Lines function duration": seq_f3_10_vec,
    "Number of segments = 10, Parallel Fit Lines function durations": par_f3_10_vec
}

run_50_f3 = {
    "Number of segments = 50, Sequential Fit Lines function duration": seq_f3_50_vec,
    "Number of segments = 50, Parallel Fit Lines function durations": par_f3_50_vec
}

run_100_f3 = {
    "Number of segments = 100, Sequential Fit Lines function duration": seq_f3_100_vec,
    "Number of segments = 100, Parallel Fit Lines function durations": par_f3_100_vec
}

run_250_f3 = {
    "Number of segments = 250, Sequential Fit Lines function duration": seq_f3_250_vec,
    "Number of segments = 250, Parallel Fit Lines function durations": par_f3_250_vec
}

run_500_f3 = {
    "Number of segments = 500, Sequential Fit Lines function duration": seq_f3_500_vec,
    "Number of segments = 500, Parallel Fit Lines function durations": par_f3_500_vec
}

run_750_f3 = {
    "Number of segments = 750, Sequential Fit Lines function duration": seq_f3_750_vec,
    "Number of segments = 750, Parallel Fit Lines function durations": par_f3_750_vec
}

run_1000_f3 = {
    "Number of segments = 1000, Sequential Fit Lines function duration": seq_f3_1000_vec,
    "Number of segments = 1000, Parallel Fit Lines function durations": par_f3_1000_vec
}

f2_app_directory = "/home/alessio/workspace/tum/par-comp/parallel-3d-ground-segmentation/performances/plots/experiment_2/prototype_points"
f3_fl_directory = "/home/alessio/workspace/tum/par-comp/parallel-3d-ground-segmentation/performances/plots/experiment_2/fit_lines"

# PLOTTING PROTOTYPE POINTS FUNCTIONS DURATIONS
plot_timed_runs(N_THREADS, run_1_f2, "Number of segments = 1, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_10_f2, "Number of segments = 10, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_50_f2, "Number of segments = 50, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_100_f2, "Number of segments = 100, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_250_f2, "Number of segments = 250, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_500_f2, "Number of segments = 500, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_750_f2, "Number of segments = 750, Assign Prototype Points average run times [ms]", f2_app_directory)
plot_timed_runs(N_THREADS, run_1000_f2, "Number of segments = 1000, Assign Prototype Points average run times [ms]", f2_app_directory)

# PLOTTING FIT LINES FUNCTIONS DURATIONS
plot_timed_runs(N_THREADS, run_1_f3, "Number of segments = 1, Fit Lines average run times [ms]", f3_fl_directory)
plot_timed_runs(N_THREADS, run_10_f3, "Number of segments = 10, Fit Lines average run times [ms]", f3_fl_directory)
plot_timed_runs(N_THREADS, run_50_f3, "Number of segments = 50, Fit Lines average run times [ms]", f3_fl_directory)
plot_timed_runs(N_THREADS, run_100_f3, "Number of segments = 100, Fit Lines average run times [ms]", f3_fl_directory)
plot_timed_runs(N_THREADS, run_250_f3, "Number of segments = 250, Fit Lines average run times [ms]", f3_fl_directory)
plot_timed_runs(N_THREADS, run_500_f3, "Number of segments = 500, Fit Lines average run times [ms]", f3_fl_directory)
plot_timed_runs(N_THREADS, run_750_f3, "Number of segments = 750, Fit Lines average run times [ms]", f3_fl_directory)
plot_timed_runs(N_THREADS, run_1000_f3, "Number of segments = 1000, Fit Lines average run times [ms]", f3_fl_directory)