#include "performance_timer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

void PerformanceTimer::start() {
    start_time = chrono::high_resolution_clock::now();
}

double PerformanceTimer::stop() {
    auto end_time = chrono::high_resolution_clock::now();
    Duration duration = end_time - start_time;
    return duration.count();
}

void PerformanceTimer::recordTime(const string& label, double time_ms) {
    timing_results[label] = time_ms;
}

map<string, double> PerformanceTimer::getAllResults() const {
    return timing_results;
}

void PerformanceTimer::printReport() const {
    cout << "\n========== Performance Report ==========" << endl;
    cout << left << setw(30) << "Test Item" << setw(15) << "Time (ms)" << endl;
    cout << string(45, '-') << endl;
    
    for (const auto& result : timing_results) {
        cout << left << setw(30) << result.first 
             << setw(15) << fixed << setprecision(3) << result.second << endl;
    }
    cout << string(45, '=') << endl;
}

void PerformanceTimer::clear() {
    timing_results.clear();
}

void PerformanceBenchmark::printBenchmarkResult(const BenchmarkResult& result) {
    cout << "\n========== Benchmark Result ==========" << endl;
    cout << "Test Name: " << result.test_name << endl;
    cout << "Iterations: " << result.iterations << endl;
    cout << fixed << setprecision(3);
    cout << "Average Time: " << result.avg_time << " ms" << endl;
    cout << "Min Time: " << result.min_time << " ms" << endl;
    cout << "Max Time: " << result.max_time << " ms" << endl;
    
    // Calculate standard deviation
    double variance = 0.0;
    for (double time : result.all_times) {
        variance += pow(time - result.avg_time, 2);
    }
    variance /= result.iterations;
    double std_dev = sqrt(variance);
    cout << "Standard Deviation: " << std_dev << " ms" << endl;
    
    // Calculate median
    vector<double> sorted_times = result.all_times;
    sort(sorted_times.begin(), sorted_times.end());
    double median = sorted_times[result.iterations / 2];
    cout << "Median: " << median << " ms" << endl;
    cout << string(35, '=') << endl;
}