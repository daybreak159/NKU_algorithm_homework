
# 文本编辑距离与变换优化系统

## 项目简介

本项目实现了文本编辑距离（Levenshtein距离）计算的三种核心算法：动态规划、广度优先搜索（BFS）和贪心算法。通过系统的性能对比和实验分析，为智能拼写纠错、搜索引擎模糊匹配等实际应用提供算法选择方案。


## 文件说明

| 文件 | 功能 |
|------|------|
| `dynamic_programming.h/cpp` | 动态规划算法，保证最优解，$O(m×n)$时间复杂度 |
| `bfs_edit_distance.h/cpp` | BFS搜索算法，理论最优但状态空间大，适合短字符串 |
| `greedy_edit_distance.h/cpp` | 贪心算法，$O(max(m,n))$线性时间|
| `performance_timer.h/cpp` |性能测试工具 |
| `test_data.h/cpp` | 标准测试用例和数据生成 |
| `main.cpp` | 主程序入口，集成所有算法测试 |

## 快速开始

# 编译
g++ -std=c++11 -I. main.cpp algorithms/dynamic_programming.cpp algorithms/bfs_edit_distance.cpp algorithms/greedy_edit_distance.cpp utils/performance_timer.cpp utils/test_data.cpp -o main

# 运行
./main


