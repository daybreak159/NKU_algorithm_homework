#ifndef PERFORMANCE_TIMER_H
#define PERFORMANCE_TIMER_H

#include <chrono>
#include <string>
#include <vector>
#include <map>

/**
 * 性能计时工具类
 */
class PerformanceTimer {
public:
    using TimePoint = std::chrono::high_resolution_clock::time_point;
    using Duration = std::chrono::duration<double, std::milli>;
    
    TimePoint start_time;
    std::map<std::string, double> timing_results;
    
public:
    /**
     * 开始计时
     */
    void start();
    
    /**
     * 结束计时并返回耗时（毫秒）
     */
    double stop();
    
    /**
     * 记录带标签的计时结果
     */
    void recordTime(const std::string& label, double time_ms);
    
    /**
     * 获取所有计时结果
     */
    std::map<std::string, double> getAllResults() const;
    
    /**
     * 打印性能报告
     */
    void printReport() const;
    
    /**
     * 清除所有计时记录
     */
    void clear();
    
    /**
     * 静态方法：测试函数执行时间
     */
    template<typename Func>
    static double measureTime(Func&& func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0;  // 转换为毫秒
    }
};

/**
 * 性能基准测试类
 */
class PerformanceBenchmark {
public:
    struct BenchmarkResult {
        std::string test_name;
        double avg_time;
        double min_time;
        double max_time;
        int iterations;
        std::vector<double> all_times;
    };
    
    /**
     * 运行基准测试
     */
    template<typename Func>
    static BenchmarkResult runBenchmark(const std::string& test_name, Func&& func, int iterations = 100) {
        BenchmarkResult result;
        result.test_name = test_name;
        result.iterations = iterations;
        result.all_times.reserve(iterations);
        
        double total_time = 0.0;
        double min_time = std::numeric_limits<double>::max();
        double max_time = 0.0;
        
        for (int i = 0; i < iterations; i++) {
            double time = PerformanceTimer::measureTime(func);
            result.all_times.push_back(time);
            
            total_time += time;
            min_time = std::min(min_time, time);
            max_time = std::max(max_time, time);
        }
        
        result.avg_time = total_time / iterations;
        result.min_time = min_time;
        result.max_time = max_time;
        
        return result;
    }
    
    /**
     * 打印基准测试结果
     */
    static void printBenchmarkResult(const BenchmarkResult& result);
};

#endif // PERFORMANCE_TIMER_H