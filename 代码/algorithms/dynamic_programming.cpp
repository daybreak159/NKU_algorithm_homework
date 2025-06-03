#include "dynamic_programming.h"
#include <algorithm>
#include <climits>
#include <chrono>

using namespace std;

int DynamicProgrammingEditDistance::basicEditDistance(const string& str1, const string& str2) {
    int m = str1.length();
    int n = str2.length();
    
    // 创建DP矩阵
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    // 初始化边界条件
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;  // 删除str1的前i个字符
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;  // 插入str2的前j个字符
    }
    
    // 填充DP矩阵
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i-1] == str2[j-1]) {
                // 字符匹配，不需要操作
                dp[i][j] = dp[i-1][j-1];
            } else {
                // 选择代价最小的操作
                dp[i][j] = 1 + min({
                    dp[i-1][j],     // 删除str1[i-1]
                    dp[i][j-1],     // 插入str2[j-1]
                    dp[i-1][j-1]    // 替换str1[i-1]为str2[j-1]
                });
            }
        }
    }
    
    return dp[m][n];
}

DynamicProgrammingEditDistance::EditDistanceResult 
DynamicProgrammingEditDistance::completeEditDistance(const string& str1, const string& str2) {
    auto start_time = chrono::high_resolution_clock::now();
    
    EditDistanceResult result;
    int m = str1.length();
    int n = str2.length();
    
    // 创建DP矩阵
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    // 初始化边界条件
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }
    
    // 填充DP矩阵
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = 1 + min({
                    dp[i-1][j],     // 删除
                    dp[i][j-1],     // 插入
                    dp[i-1][j-1]    // 替换
                });
            }
        }
    }
    
    // 保存结果
    result.distance = dp[m][n];
    result.dp_matrix = dp;
    result.operations = reconstructPath(str1, str2, dp);
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;  // 转换为毫秒
    
    return result;
}

int DynamicProgrammingEditDistance::spaceOptimizedEditDistance(const string& str1, const string& str2) {
    int m = str1.length();
    int n = str2.length();
    
    // 确保str1是较短的字符串，减少空间使用
    if (m > n) {
        return spaceOptimizedEditDistance(str2, str1);
    }
    
    // 只使用两行空间
    vector<int> prev(m + 1);
    vector<int> curr(m + 1);
    
    // 初始化第一行
    for (int i = 0; i <= m; i++) {
        prev[i] = i;
    }
    
    // 逐行计算
    for (int j = 1; j <= n; j++) {
        curr[0] = j;
        for (int i = 1; i <= m; i++) {
            if (str1[i-1] == str2[j-1]) {
                curr[i] = prev[i-1];
            } else {
                curr[i] = 1 + min({
                    prev[i],        // 删除
                    curr[i-1],      // 插入
                    prev[i-1]       // 替换
                });
            }
        }
        prev = curr;
    }
    
    return prev[m];
}

int DynamicProgrammingEditDistance::weightedEditDistance(const string& str1, const string& str2,
                                                        int insert_cost, int delete_cost, int replace_cost) {
    int m = str1.length();
    int n = str2.length();
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    // 初始化边界条件
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i * delete_cost;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j * insert_cost;
    }
    
    // 填充DP矩阵
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = min({
                    dp[i-1][j] + delete_cost,       // 删除
                    dp[i][j-1] + insert_cost,       // 插入
                    dp[i-1][j-1] + replace_cost     // 替换
                });
            }
        }
    }
    
    return dp[m][n];
}

vector<pair<string, int>> DynamicProgrammingEditDistance::batchEditDistance(
    const string& target, const vector<string>& candidates) {
    
    vector<pair<string, int>> results;
    results.reserve(candidates.size());
    
    for (const auto& candidate : candidates) {
        int distance = basicEditDistance(target, candidate);
        results.emplace_back(candidate, distance);
    }
    
    // 按编辑距离排序
    sort(results.begin(), results.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second < b.second;
         });
    
    return results;
}

vector<DynamicProgrammingEditDistance::EditOperation> 
DynamicProgrammingEditDistance::reconstructPath(const string& str1, const string& str2,
                                               const vector<vector<int>>& dp) {
    vector<EditOperation> operations;
    int i = str1.length();
    int j = str2.length();
    
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && str1[i-1] == str2[j-1]) {
            // 字符匹配
            operations.emplace_back(MATCH, i-1, j-1, str1[i-1], str2[j-1]);
            i--; j--;
        } else if (i > 0 && j > 0 && dp[i][j] == dp[i-1][j-1] + 1) {
            // 替换操作
            operations.emplace_back(REPLACE, i-1, j-1, str1[i-1], str2[j-1]);
            i--; j--;
        } else if (i > 0 && dp[i][j] == dp[i-1][j] + 1) {
            // 删除操作
            operations.emplace_back(DELETE, i-1, -1, str1[i-1], '\0');
            i--;
        } else if (j > 0 && dp[i][j] == dp[i][j-1] + 1) {
            // 插入操作
            operations.emplace_back(INSERT, i, j-1, '\0', str2[j-1]);
            j--;
        }
    }
    
    // 反转操作序列，使其按正确顺序排列
    reverse(operations.begin(), operations.end());
    return operations;
}

string DynamicProgrammingEditDistance::getOperationString(const EditOperation& op) {
    switch (op.op) {
        case MATCH:
            return "Match(" + string(1, op.from_char) + ")";
        case INSERT:
            return "Insert(" + string(1, op.to_char) + ")";
        case DELETE:
            return "Delete(" + string(1, op.from_char) + ")";
        case REPLACE:
            return "Replace(" + string(1, op.from_char) + "->" + string(1, op.to_char) + ")";
        default:
            return "Unknown";
    }
}