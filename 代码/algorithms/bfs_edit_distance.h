#ifndef BFS_EDIT_DISTANCE_H
#define BFS_EDIT_DISTANCE_H

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>

/**
 * 基于BFS的编辑距离算法实现
 */
class BFSEditDistance {
public:
    // 状态结构：表示搜索过程中的一个字符串状态
    struct State {
        std::string current_string;  // 当前字符串
        int cost;                   // 到达此状态的代价
        std::vector<std::string> path; // 变换路径
        
        State(const std::string& str, int c, const std::vector<std::string>& p)
            : current_string(str), cost(c), path(p) {}
            
        // 添加比较操作符用于priority_queue
        bool operator<(const State& other) const {
            // 优先级队列默认是最大堆，我们需要最小堆，所以反转比较
            return cost > other.cost;
        }
    };
    
    // BFS搜索结果
    struct BFSResult {
        int min_distance;                        // 最小编辑距离
        std::vector<std::vector<std::string>> all_paths; // 所有最优路径
        int nodes_explored;                      // 探索的节点数
        double computation_time;                 // 计算时间(ms)
        int max_queue_size;                     // 队列最大长度
    };

public:
    /**
     * 基础BFS编辑距离算法
     */
    static int basicBFSEditDistance(const std::string& start, const std::string& target);
    
    /**
     * 完整BFS算法 - 返回所有最优路径
     */
    static BFSResult completeBFSEditDistance(const std::string& start, const std::string& target);
    
    /**
     * 带剪枝的BFS算法 - 使用启发式函数提高效率
     */
    static BFSResult optimizedBFSEditDistance(const std::string& start, const std::string& target);
    
    /**
     * 双向BFS算法 - 从两端同时搜索
     */
    static BFSResult bidirectionalBFS(const std::string& start, const std::string& target);
    
    /**
     * 受限BFS算法 - 限制最大搜索深度
     */
    static BFSResult limitedBFS(const std::string& start, const std::string& target, int max_depth);

private:
    // 生成所有可能的下一步状态
    static std::vector<std::string> generateNextStates(const std::string& current);
    
    // 计算启发式函数值（到目标的估计距离）
    static int heuristic(const std::string& current, const std::string& target);
    
    // 字符串哈希函数
    static std::string stateToString(const std::string& str);
};

#endif // BFS_EDIT_DISTANCE_H