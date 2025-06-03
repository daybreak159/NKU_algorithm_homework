
# 文本编辑距离与变换优化系统

## 项目简介

本项目实现了文本编辑距离（Levenshtein距离）计算的三种核心算法：动态规划、广度优先搜索（BFS）和贪心算法。通过系统的性能对比和实验分析，为智能拼写纠错、搜索引擎模糊匹配等实际应用提供算法选择方案。

## 项目结构


📦 NKU_algorithm_homework
├── 📁 algorithms/                     # 算法实现模块
│   ├── 📄 dynamic_programming.h       # 动态规划算法头文件
│   ├── 📄 dynamic_programming.cpp     # 动态规划算法实现
│   ├── 📄 bfs_edit_distance.h         # BFS搜索算法头文件  
│   ├── 📄 bfs_edit_distance.cpp       # BFS算法实现
│   ├── 📄 greedy_edit_distance.h      # 贪心算法头文件
│   └── 📄 greedy_edit_distance.cpp    # 贪心算法实现
├── 📁 utils/                          # 工具模块
│   ├── 📄 performance_timer.h         # 性能计时器头文件
│   ├── 📄 performance_timer.cpp       # 计时和统计工具
│   ├── 📄 test_data.h                 # 测试数据头文件
│   └── 📄 test_data.cpp               # 测试用例生成器
├── 📄 main.cpp                        # 主程序（算法测试和对比）
├── 📄 我的报告.tex                    # 研究报告
└── 📄 README.md                       # 项目说明文档


## 文件说明

| 文件 | 功能 |
|------|------|
| `dynamic_programming.h/cpp` | 动态规划算法，保证最优解，$O(m×n)$时间复杂度 |
| `bfs_edit_distance.h/cpp` | BFS搜索算法，理论最优但状态空间大，适合短字符串 |
| `greedy_edit_distance.h/cpp` | 贪心算法，$O(max(m,n))$线性时间，实际准确率100% |
| `performance_timer.h/cpp` | 高精度性能测试工具 |
| `test_data.h/cpp` | 标准测试用例和数据生成 |
| `main.cpp` | 主程序入口，集成所有算法测试 |
| `我的报告.tex` | 完整研究报告，包含理论分析和实验结果 |

## 快速开始

# 编译
g++ -std=c++11 -I. main.cpp algorithms/dynamic_programming.cpp algorithms/bfs_edit_distance.cpp algorithms/greedy_edit_distance.cpp utils/performance_timer.cpp utils/test_data.cpp -o main

# 运行
./main


