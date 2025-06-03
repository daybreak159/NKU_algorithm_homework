#include <iostream>
#include <iomanip>
#include "algorithms/dynamic_programming.h"
#include "algorithms/bfs_edit_distance.h"
#include "algorithms/greedy_edit_distance.h"
#include "utils/performance_timer.h"
#include "utils/test_data.h"

using namespace std;

void testBasicFunctionality() {
    cout << "========== Basic Functionality Test ==========" << endl;
    
    auto test_cases = TestDataGenerator::getClassicTestCases();
    
    for (const auto& test_case : test_cases) {
        cout << "\nTest: " << test_case.description << endl;
        cout << "String 1: \"" << test_case.str1 << "\"" << endl;
        cout << "String 2: \"" << test_case.str2 << "\"" << endl;
        
        int basic_result = DynamicProgrammingEditDistance::basicEditDistance(
            test_case.str1, test_case.str2);
        
        auto complete_result = DynamicProgrammingEditDistance::completeEditDistance(
            test_case.str1, test_case.str2);
        
        cout << "Calculated Edit Distance: " << basic_result << endl;
        cout << "Expected Edit Distance: " << test_case.expected_distance << endl;
        cout << "Result: " << (basic_result == test_case.expected_distance ? "[PASS]" : "[FAIL]") << endl;
        cout << "Computation Time: " << fixed << setprecision(3) << complete_result.computation_time << " ms" << endl;
        
        if (test_case.str1.length() <= 10 && test_case.str2.length() <= 10) {
            cout << "Operation Sequence: ";
            for (const auto& op : complete_result.operations) {
                if (op.op != DynamicProgrammingEditDistance::MATCH) {
                    cout << DynamicProgrammingEditDistance::getOperationString(op) << " ";
                }
            }
            cout << endl;
        }
    }
}

void testBFSFunctionality() {
    cout << "\n========== BFS Algorithm Test (Limited) ==========" << endl;
    
    // 仅保留一个简单测试用例证明BFS可以运行
    vector<pair<string, string>> bfs_test_cases = {
        {"cat", "bat"}
    };
    
    for (const auto& test_case : bfs_test_cases) {
        cout << "\nBFS Test: \"" << test_case.first << "\" -> \"" << test_case.second << "\"" << endl;
        
        int bfs_distance = BFSEditDistance::basicBFSEditDistance(test_case.first, test_case.second);
        cout << "BFS Edit Distance: " << bfs_distance << endl;
        
        int dp_distance = DynamicProgrammingEditDistance::basicEditDistance(test_case.first, test_case.second);
        cout << "DP vs BFS Consistency: " << (bfs_distance == dp_distance ? "[PASS]" : "[FAIL]") << endl;
        
        cout << "Note: BFS shows exponential state space growth - see detailed analysis in report" << endl;
    }
}

void testGreedyAlgorithms() {
    cout << "\n========== Greedy Algorithm Test ==========" << endl;
    
    vector<pair<string, string>> greedy_test_cases = {
        {"kitten", "sitting"},
        {"algorithm", "logarithm"},
        {"intention", "execution"},
        {"abc", "ab"},
        {"hello", "world"}
    };
    
    for (const auto& test_case : greedy_test_cases) {
        cout << "\nGreedy Test: \"" << test_case.first << "\" -> \"" << test_case.second << "\"" << endl;
        
        // DP算法作为基准
        int dp_result = DynamicProgrammingEditDistance::basicEditDistance(test_case.first, test_case.second);
        
        // 基础贪心算法
        auto basic_greedy = GreedyEditDistance::basicGreedyEditDistance(test_case.first, test_case.second);
        
        // 启发式贪心算法
        auto heuristic_greedy = GreedyEditDistance::heuristicGreedy(test_case.first, test_case.second);
        
        // 快速近似算法
        int quick_approx = GreedyEditDistance::quickApproximation(test_case.first, test_case.second);
        
        cout << "Results:" << endl;
        cout << "DP (Optimal): " << dp_result << endl;
        cout << "Basic Greedy: " << basic_greedy.distance 
             << " (time: " << fixed << setprecision(3) << basic_greedy.computation_time << " ms)" << endl;
        cout << "Heuristic Greedy: " << heuristic_greedy.distance 
             << " (time: " << fixed << setprecision(3) << heuristic_greedy.computation_time << " ms)" << endl;
        cout << "Quick Approximation: " << quick_approx << endl;
        
        // 计算近似比
        if (dp_result > 0) {
            double basic_ratio = (double)basic_greedy.distance / dp_result;
            double heuristic_ratio = (double)heuristic_greedy.distance / dp_result;
            double quick_ratio = (double)quick_approx / dp_result;
            
            cout << "Approximation Ratios:" << endl;
            cout << "Basic Greedy: " << fixed << setprecision(2) << basic_ratio << endl;
            cout << "Heuristic Greedy: " << fixed << setprecision(2) << heuristic_ratio << endl;
            cout << "Quick Approximation: " << fixed << setprecision(2) << quick_ratio << endl;
        }
        
        // 显示操作序列（仅对较短字符串）
        if (test_case.first.length() <= 8) {
            cout << "Basic Greedy Operations: ";
            for (const auto& op : basic_greedy.operations) {
                if (op.type != GreedyEditDistance::MATCH) {
                    cout << GreedyEditDistance::operationToString(op) << " ";
                }
            }
            cout << endl;
        }
    }
}

void testAlgorithmComparison() {
    cout << "\n========== Multi-Algorithm Comparison ==========" << endl;
    
    vector<pair<string, string>> comparison_cases = {
        {"abc", "axc"},
        {"hello", "world"},
        {"kitten", "sitting"}
    };
    
    for (const auto& test_case : comparison_cases) {
        cout << "\nComparing algorithms for: \"" << test_case.first << "\" -> \"" << test_case.second << "\"" << endl;
        
        PerformanceTimer timer;
        
        // DP算法
        timer.start();
        int dp_result = DynamicProgrammingEditDistance::basicEditDistance(test_case.first, test_case.second);
        double dp_time = timer.stop();
        
        // 贪心算法
        timer.start();
        auto greedy_result = GreedyEditDistance::basicGreedyEditDistance(test_case.first, test_case.second);
        double greedy_time = timer.stop();
        
        // 快速近似算法
        timer.start();
        int quick_result = GreedyEditDistance::quickApproximation(test_case.first, test_case.second);
        double quick_time = timer.stop();
        
        cout << "Results:" << endl;
        cout << "DP: distance=" << dp_result << ", time=" << fixed << setprecision(3) << dp_time << " ms" << endl;
        cout << "Greedy: distance=" << greedy_result.distance << ", time=" << fixed << setprecision(3) << greedy_time << " ms" << endl;
        cout << "Quick: distance=" << quick_result << ", time=" << fixed << setprecision(3) << quick_time << " ms" << endl;
        
        if (dp_time > 0) {
            cout << "Speedup - Greedy vs DP: " << fixed << setprecision(2) << dp_time / greedy_time << "x" << endl;
            cout << "Speedup - Quick vs DP: " << fixed << setprecision(2) << dp_time / quick_time << "x" << endl;
        }
    }
}

void testPerformance() {
    cout << "\n========== Performance Test ==========" << endl;
    
    PerformanceTimer timer;
    auto test_data = TestDataGenerator::generatePerformanceTestData();
    
    cout << "Test Data Size: " << test_data.size() << " string pairs" << endl;
    
    // 测试各算法性能
    timer.start();
    for (const auto& pair : test_data) {
        DynamicProgrammingEditDistance::basicEditDistance(pair.first, pair.second);
    }
    double dp_time = timer.stop();
    
    timer.start();
    for (const auto& pair : test_data) {
        GreedyEditDistance::basicGreedyEditDistance(pair.first, pair.second);
    }
    double greedy_time = timer.stop();
    
    timer.start();
    for (const auto& pair : test_data) {
        GreedyEditDistance::quickApproximation(pair.first, pair.second);
    }
    double quick_time = timer.stop();
    
    cout << "\nPerformance Results:" << endl;
    cout << "Dynamic Programming: " << fixed << setprecision(3) << dp_time << " ms" << endl;
    cout << "Greedy Algorithm: " << fixed << setprecision(3) << greedy_time << " ms" << endl;
    cout << "Quick Approximation: " << fixed << setprecision(3) << quick_time << " ms" << endl;
    
    if (dp_time > 0) {
        cout << "Performance Improvement:" << endl;
        cout << "Greedy vs DP: " << fixed << setprecision(2) << dp_time / greedy_time << "x faster" << endl;
        cout << "Quick vs DP: " << fixed << setprecision(2) << dp_time / quick_time << "x faster" << endl;
    }
}

void testBatchProcessing() {
    cout << "\n========== Batch Processing Test ==========" << endl;
    
    string target = "algorithm";
    vector<string> candidates = {
        "algoritm", "algorythm", "algortihm", "algorithms", 
        "logarithm", "arithmetic", "heuristic", "polynomial"
    };
    
    cout << "Target String: \"" << target << "\"" << endl;
    cout << "Candidate Strings: ";
    for (const auto& candidate : candidates) {
        cout << "\"" << candidate << "\" ";
    }
    cout << endl;
    
    PerformanceTimer timer;
    timer.start();
    auto results = DynamicProgrammingEditDistance::batchEditDistance(target, candidates);
    double batch_time = timer.stop();
    
    cout << "\nSorted Results (by edit distance):" << endl;
    for (const auto& result : results) {
        cout << "\"" << result.first << "\": " << result.second << endl;
    }
    
    cout << "\nBatch Processing Time: " << fixed << setprecision(3) << batch_time << " ms" << endl;
}

int main() {
    cout << "Text Edit Distance Multi-Algorithm Test Program" << endl;
    cout << "===============================================" << endl;
    
    try {
        // 基本功能测试
        testBasicFunctionality();
        
        // BFS算法简单测试
        testBFSFunctionality();
        
        // 贪心算法测试
        testGreedyAlgorithms();
        
        // 算法对比测试
        testAlgorithmComparison();
        
        // 性能测试
        testPerformance();
        
        // 批量处理测试
        testBatchProcessing();
        
        cout << "\nAll tests completed!" << endl;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}