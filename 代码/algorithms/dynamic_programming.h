#ifndef DYNAMIC_PROGRAMMING_H
#define DYNAMIC_PROGRAMMING_H

#include <string>
#include <vector>
#include <utility>

/**
 * 动态规划编辑距离算法实现
 */
class DynamicProgrammingEditDistance {
public:
    // 操作类型枚举
    enum Operation {
        MATCH = 0,      // 匹配
        INSERT = 1,     // 插入
        DELETE = 2,     // 删除
        REPLACE = 3     // 替换
    };
    
    // 操作记录结构
    struct EditOperation {
        Operation op;
        int pos1, pos2;    // 在两个字符串中的位置
        char from_char, to_char;  // 字符变换
        
        EditOperation(Operation operation, int p1, int p2, char from = '\0', char to = '\0')
            : op(operation), pos1(p1), pos2(p2), from_char(from), to_char(to) {}
    };
    
    // 算法结果结构
    struct EditDistanceResult {
        int distance;                           // 最小编辑距离
        std::vector<EditOperation> operations;  // 操作序列
        std::vector<std::vector<int>> dp_matrix; // DP矩阵（用于分析）
        double computation_time;                // 计算时间（毫秒）
    };

public:
    /**
     * 基础动态规划算法 - 只计算编辑距离
     * 时间复杂度: O(m*n)
     * 空间复杂度: O(m*n)
     */
    static int basicEditDistance(const std::string& str1, const std::string& str2);
    
    /**
     * 完整动态规划算法 - 计算编辑距离并记录操作路径
     * 时间复杂度: O(m*n)
     * 空间复杂度: O(m*n)
     */
    static EditDistanceResult completeEditDistance(const std::string& str1, const std::string& str2);
    
    /**
     * 空间优化的动态规划算法 - 只使用O(min(m,n))空间
     * 时间复杂度: O(m*n)
     * 空间复杂度: O(min(m,n))
     */
    static int spaceOptimizedEditDistance(const std::string& str1, const std::string& str2);
    
    /**
     * 加权编辑距离算法 - 支持不同操作的权重
     */
    static int weightedEditDistance(const std::string& str1, const std::string& str2,
                                   int insert_cost = 1, int delete_cost = 1, int replace_cost = 1);
    
    /**
     * 批量计算编辑距离 - 一个目标字符串与多个候选字符串
     */
    static std::vector<std::pair<std::string, int>> batchEditDistance(
        const std::string& target, const std::vector<std::string>& candidates);

public:
    // 辅助函数：从DP矩阵重构操作序列
    static std::vector<EditOperation> reconstructPath(
        const std::string& str1, const std::string& str2,
        const std::vector<std::vector<int>>& dp);
    
    // 辅助函数：获取操作描述字符串
    static std::string getOperationString(const EditOperation& op);
};

#endif // DYNAMIC_PROGRAMMING_H