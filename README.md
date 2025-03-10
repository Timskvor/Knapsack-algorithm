# Knapsack Algorithm Comparison

This repository contains the results of three different Knapsack algorithms:
- **Dynamic Programming (DP)**
- **Branch and Bound (BnB)**
- **Genetic Algorithm (GA)**

## Results Table

| Instance   | DP Max Value / Time (ms) | BnB Max Value / Time (ms) | GA Max Value / Time (ms) |
|------------|-------------------------|---------------------------|-------------------------|
| ks_4_0     | 19 / 1                  | 18 / 0                    | 19 / 2242               |
| ks_19_0    | 12248 / 2339            | 12248 / 0                 | 12197 / 2487            |
| ks_30_0    | 99798 / 8469            | 99798 / 22                | 99759 / 2639            |
| ks_40_0    | 99924 / 10716           | 99924 / 14783             | 99924 / 1038            |
| ks_45_0    | 23974 / 10339           | 23974 / 0                 | 23650 / 2871            |
| ks_50_0    | 142156 / 74118          | 142156 / 0                | 141112 / 2959           |
| ks_50_1    | 5345 / 1147             | 5345 / 1                  | 5224 / 2959             |
| ks_60_0    | 99837 / 14713           | 99837 / 43112             | 99837 / 3100            |
| ks_100_0   | 99837 / 21406           | 99837 / 822818            | 99789 / 3286            |
| ks_100_1   | 1333930 / 1495367       | 1333930 / 3               | 1331705 / 3264          |
| ks_100_2   | 10892 / 3929            | 10892 / 0                 | 10664 / 3293            |
| ks_200_0   | 100236 / 40181          | 100236 / 61534            | 100236 / 1522           |
| ks_200_1   | 1103604 / 2259330       | 1103604 / 14              | 1100959 / 1802          |
| ks_300_0   | 1688692 / 4991563       | 1688688 / 377             | 1683388 / 2739          |
| ks_400_0   | 3967180 / 17124620      | 3967180 / 314             | 3961974 / 3256          |
| ks_500_0   | 54939 / 79597           | 54938 / 9                 | 53781 / 3128            |
| ks_1000_0  | 109899 / 310413         | 109899 / 15               | 106810 / 4756           |
| ks_10000_0 | 1099893 / 29552939      | 1099893 / 139             | 1048242 / 28766         |

## Execution Time Comparison

Below is a graphical comparison of execution times (log scale) for each algorithm:

![Time Comparison](knapsack_time_comparison_sorted.png)

