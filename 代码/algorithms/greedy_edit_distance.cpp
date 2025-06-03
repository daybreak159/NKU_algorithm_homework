#include "greedy_edit_distance.h"
#include <algorithm>
#include <chrono>
#include <unordered_set>

using namespace std;

GreedyEditDistance::GreedyResult GreedyEditDistance::basicGreedyEditDistance(
    const string& source, const string& target) {
    
    auto start_time = chrono::high_resolution_clock::now();
    
    GreedyResult result;
    result.distance = 0;
    result.final_string = source;
    
    string current = source;
    int pos = 0;
    
    // 贪心策略：从左到右逐字符匹配
    while (pos < max(current.length(), target.length())) {
        if (pos >= current.length()) {
            // 需要插入字符
            char insert_char = target[pos];
            current.insert(pos, 1, insert_char);
            result.operations.push_back(EditOperation(INSERT, pos, '\0', insert_char));
            result.distance++;
        } else if (pos >= target.length()) {
            // 需要删除字符
            char delete_char = current[pos];
            current.erase(pos, 1);
            result.operations.push_back(EditOperation(DELETE, pos, delete_char, '\0'));
            result.distance++;
            continue; // 不增加pos，因为字符串变短了
        } else if (current[pos] == target[pos]) {
            // 字符匹配，直接跳过
            result.operations.push_back(EditOperation(MATCH, pos, current[pos], target[pos]));
        } else {
            // 需要替换字符
            char from_char = current[pos];
            char to_char = target[pos];
            current[pos] = to_char;
            result.operations.push_back(EditOperation(REPLACE, pos, from_char, to_char));
            result.distance++;
        }
        pos++;
    }
    
    result.final_string = current;
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;
    
    return result;
}

GreedyEditDistance::GreedyResult GreedyEditDistance::frequencyBasedGreedy(
    const string& source, const string& target) {
    
    auto start_time = chrono::high_resolution_clock::now();
    
    GreedyResult result;
    result.distance = 0;
    
    // 计算字符频率
    auto source_freq = calculateFrequency(source);
    auto target_freq = calculateFrequency(target);
    
    string current = source;
    
    // 贪心策略：优先保留目标字符串中频率高的字符
    for (auto& pair : target_freq) {
        char target_char = pair.first;
        int target_count = pair.second;
        int source_count = (source_freq.find(target_char) != source_freq.end()) ? 
                          source_freq[target_char] : 0;
        
        if (source_count < target_count) {
            // 需要插入字符
            int insert_count = target_count - source_count;
            for (int i = 0; i < insert_count; i++) {
                current += target_char;
                result.operations.push_back(EditOperation(INSERT, current.length()-1, '\0', target_char));
                result.distance++;
            }
        }
    }
    
    // 删除不需要的字符
    for (auto& pair : source_freq) {
        char source_char = pair.first;
        int source_count = pair.second;
        int target_count = (target_freq.find(source_char) != target_freq.end()) ? 
                          target_freq[source_char] : 0;
        
        if (source_count > target_count) {
            int delete_count = source_count - target_count;
            for (int i = 0; i < delete_count; i++) {
                // 找到第一个该字符并删除
                auto pos = current.find(source_char);
                if (pos != string::npos) {
                    current.erase(pos, 1);
                    result.operations.push_back(EditOperation(DELETE, pos, source_char, '\0'));
                    result.distance++;
                }
            }
        }
    }
    
    result.final_string = current;
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;
    
    return result;
}

GreedyEditDistance::GreedyResult GreedyEditDistance::positionBasedGreedy(
    const string& source, const string& target) {
    
    auto start_time = chrono::high_resolution_clock::now();
    
    GreedyResult result;
    result.distance = 0;
    
    // 找到最长公共子序列的位置
    auto lcs_positions = findLCS(source, target);
    
    string current = source;
    int offset = 0; // 由于插入删除操作导致的位置偏移
    
    // 贪心策略：保持LCS不变，调整其他字符
    for (size_t i = 0; i < max(source.length(), target.length()); i++) {
        int current_pos = i + offset;
        
        if (i >= target.length()) {
            // 删除多余字符
            if (current_pos < current.length()) {
                char delete_char = current[current_pos];
                current.erase(current_pos, 1);
                result.operations.push_back(EditOperation(DELETE, current_pos, delete_char, '\0'));
                result.distance++;
                offset--;
            }
        } else if (current_pos >= current.length()) {
            // 插入缺失字符
            char insert_char = target[i];
            current.insert(current_pos, 1, insert_char);
            result.operations.push_back(EditOperation(INSERT, current_pos, '\0', insert_char));
            result.distance++;
            offset++;
        } else if (current[current_pos] != target[i]) {
            // 替换不匹配字符
            char from_char = current[current_pos];
            char to_char = target[i];
            current[current_pos] = to_char;
            result.operations.push_back(EditOperation(REPLACE, current_pos, from_char, to_char));
            result.distance++;
        } else {
            // 字符匹配
            result.operations.push_back(EditOperation(MATCH, current_pos, current[current_pos], target[i]));
        }
    }
    
    result.final_string = current;
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;
    
    return result;
}

GreedyEditDistance::GreedyResult GreedyEditDistance::heuristicGreedy(
    const string& source, const string& target) {
    
    auto start_time = chrono::high_resolution_clock::now();
    
    GreedyResult result;
    result.distance = 0;
    
    string current = source;
    
    // 启发式策略：结合前缀匹配、后缀匹配和中间处理
    
    // 1. 处理相同前缀
    int prefix_len = 0;
    while (prefix_len < min(current.length(), target.length()) && 
           current[prefix_len] == target[prefix_len]) {
        result.operations.push_back(EditOperation(MATCH, prefix_len, current[prefix_len], target[prefix_len]));
        prefix_len++;
    }
    
    // 2. 处理相同后缀
    int suffix_len = 0;
    while (suffix_len < min(current.length() - prefix_len, target.length() - prefix_len) &&
           current[current.length() - 1 - suffix_len] == target[target.length() - 1 - suffix_len]) {
        suffix_len++;
    }
    
    // 3. 处理中间部分
    if (prefix_len + suffix_len < max(current.length(), target.length())) {
        string middle_source = current.substr(prefix_len, current.length() - prefix_len - suffix_len);
        string middle_target = target.substr(prefix_len, target.length() - prefix_len - suffix_len);
        
        // 对中间部分应用基础贪心算法
        auto middle_result = basicGreedyEditDistance(middle_source, middle_target);
        
        // 调整操作位置
        for (auto& op : middle_result.operations) {
            if (op.type != MATCH) {
                op.position += prefix_len;
                result.operations.push_back(op);
            }
        }
        result.distance += middle_result.distance;
        
        // 更新当前字符串
        current = current.substr(0, prefix_len) + middle_result.final_string + 
                 current.substr(current.length() - suffix_len);
    }
    
    // 添加后缀匹配操作
    for (int i = 0; i < suffix_len; i++) {
        int pos = target.length() - suffix_len + i;
        result.operations.push_back(EditOperation(MATCH, pos, target[pos], target[pos]));
    }
    
    result.final_string = current;
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;
    
    return result;
}

int GreedyEditDistance::quickApproximation(const string& source, const string& target) {
    if (source == target) return 0;
    
    // 快速近似：长度差 + 不匹配字符数的估计
    int len_diff = abs((int)source.length() - (int)target.length());
    
    unordered_set<char> source_chars(source.begin(), source.end());
    unordered_set<char> target_chars(target.begin(), target.end());
    
    int unique_in_source = 0;
    int unique_in_target = 0;
    
    for (char c : source_chars) {
        if (target_chars.find(c) == target_chars.end()) {
            unique_in_source++;
        }
    }
    
    for (char c : target_chars) {
        if (source_chars.find(c) == source_chars.end()) {
            unique_in_target++;
        }
    }
    
    return len_diff + max(unique_in_source, unique_in_target);
}

map<char, int> GreedyEditDistance::calculateFrequency(const string& str) {
    map<char, int> freq;
    for (char c : str) {
        freq[c]++;
    }
    return freq;
}

vector<pair<int, int>> GreedyEditDistance::findLCS(const string& str1, const string& str2) {
    int m = str1.length(), n = str2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    // 计算LCS长度
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i-1] == str2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    
    // 回溯找到LCS位置
    vector<pair<int, int>> positions;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (str1[i-1] == str2[j-1]) {
            positions.push_back(make_pair(i-1, j-1));
            i--; j--;
        } else if (dp[i-1][j] > dp[i][j-1]) {
            i--;
        } else {
            j--;
        }
    }
    
    reverse(positions.begin(), positions.end());
    return positions;
}

int GreedyEditDistance::calculateOperationCost(OperationType type, char from_char, char to_char) {
    // 简单的代价模型，可以根据需要扩展
    switch (type) {
        case INSERT:
        case DELETE:
            return 1;
        case REPLACE:
            return 1;
        case MATCH:
            return 0;
        default:
            return 1;
    }
}

string GreedyEditDistance::operationToString(const EditOperation& op) {
    switch (op.type) {
        case INSERT:
            return "Insert(" + string(1, op.to_char) + ")";
        case DELETE:
            return "Delete(" + string(1, op.from_char) + ")";
        case REPLACE:
            return "Replace(" + string(1, op.from_char) + "->" + string(1, op.to_char) + ")";
        case MATCH:
            return "Match(" + string(1, op.from_char) + ")";
        default:
            return "Unknown";
    }
}