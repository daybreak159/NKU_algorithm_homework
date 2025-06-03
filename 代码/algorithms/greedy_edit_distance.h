#ifndef GREEDY_EDIT_DISTANCE_H
#define GREEDY_EDIT_DISTANCE_H

#include <string>
#include <vector>
#include <map>

/**
 * 基于贪心策略的编辑距离算法实现
 */
class GreedyEditDistance {
public:
    // 编辑操作类型
    enum OperationType {
        INSERT,
        DELETE,
        REPLACE,
        MATCH
    };
    
    // 编辑操作结构
    struct EditOperation {
        OperationType type;
        int position;
        char from_char;
        char to_char;
        
        EditOperation(OperationType t, int pos, char from, char to)
            : type(t), position(pos), from_char(from), to_char(to) {}
    };
    
    // 贪心算法结果
    struct GreedyResult {
        int distance;
        std::vector<EditOperation> operations;
        double computation_time;
        std::string final_string;
    };

public:
    /**
     * 基础贪心算法 - 优先匹配相同字符
     */
    static GreedyResult basicGreedyEditDistance(const std::string& source, const std::string& target);
    
    /**
     * 频率优化贪心算法 - 考虑字符出现频率
     */
    static GreedyResult frequencyBasedGreedy(const std::string& source, const std::string& target);
    
    /**
     * 位置优化贪心算法 - 优先处理位置接近的字符
     */
    static GreedyResult positionBasedGreedy(const std::string& source, const std::string& target);
    
    /**
     * 启发式贪心算法 - 结合多种策略
     */
    static GreedyResult heuristicGreedy(const std::string& source, const std::string& target);
    
    /**
     * 快速近似算法 - 仅做简单的前缀后缀匹配
     */
    static int quickApproximation(const std::string& source, const std::string& target);

public:
    // 计算字符频率
    static std::map<char, int> calculateFrequency(const std::string& str);
    
    // 寻找最长公共子序列
    static std::vector<std::pair<int, int>> findLCS(const std::string& str1, const std::string& str2);
    
    // 计算操作代价
    static int calculateOperationCost(OperationType type, char from_char, char to_char);
    
    // 操作转字符串
    static std::string operationToString(const EditOperation& op);
};

#endif // GREEDY_EDIT_DISTANCE_H