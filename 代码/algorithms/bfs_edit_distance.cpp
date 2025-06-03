#include "bfs_edit_distance.h"
#include <algorithm>
#include <chrono>

using namespace std;

int BFSEditDistance::basicBFSEditDistance(const string& start, const string& target) {
    if (start == target) return 0;
    
    queue<pair<string, int>> q;
    unordered_set<string> visited;
    
    q.push(make_pair(start, 0));
    visited.insert(start);
    
    while (!q.empty()) {
        pair<string, int> current_pair = q.front();
        q.pop();
        
        string current = current_pair.first;
        int cost = current_pair.second;
        
        // 生成所有可能的下一步状态
        vector<string> next_states = generateNextStates(current);
        
        for (const string& next_state : next_states) {
            if (next_state == target) {
                return cost + 1;
            }
            
            if (visited.find(next_state) == visited.end()) {
                visited.insert(next_state);
                q.push(make_pair(next_state, cost + 1));
            }
        }
    }
    
    return -1; // 不应该到达这里
}

BFSEditDistance::BFSResult BFSEditDistance::completeBFSEditDistance(
    const string& start, const string& target) {
    
    auto start_time = chrono::high_resolution_clock::now();
    
    BFSResult result;
    result.min_distance = -1;
    result.nodes_explored = 0;
    result.max_queue_size = 0;
    
    if (start == target) {
        result.min_distance = 0;
        result.all_paths.push_back(vector<string>(1, start));
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        result.computation_time = duration.count() / 1000.0;
        return result;
    }
    
    queue<State> q;
    unordered_map<string, int> best_cost; // 记录到达每个状态的最小代价
    
    vector<string> initial_path(1, start);
    q.push(State(start, 0, initial_path));
    best_cost[start] = 0;
    
    while (!q.empty()) {
        result.max_queue_size = max(result.max_queue_size, (int)q.size());
        
        State current = q.front();
        q.pop();
        result.nodes_explored++;
        
        // 如果已经找到最优解，且当前代价大于最优解，跳过
        if (result.min_distance != -1 && current.cost > result.min_distance) {
            continue;
        }
        
        // 生成所有可能的下一步状态
        vector<string> next_states = generateNextStates(current.current_string);
        
        for (const string& next_state : next_states) {
            int new_cost = current.cost + 1;
            
            if (next_state == target) {
                // 找到目标状态
                if (result.min_distance == -1 || new_cost < result.min_distance) {
                    // 找到更优解
                    result.min_distance = new_cost;
                    result.all_paths.clear();
                    vector<string> new_path = current.path;
                    new_path.push_back(target);
                    result.all_paths.push_back(new_path);
                } else if (new_cost == result.min_distance) {
                    // 找到等优解
                    vector<string> new_path = current.path;
                    new_path.push_back(target);
                    result.all_paths.push_back(new_path);
                }
            } else {
                // 中间状态，继续搜索
                if (best_cost.find(next_state) == best_cost.end() || 
                    new_cost <= best_cost[next_state]) {
                    
                    best_cost[next_state] = new_cost;
                    vector<string> new_path = current.path;
                    new_path.push_back(next_state);
                    q.push(State(next_state, new_cost, new_path));
                }
            }
        }
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;
    
    return result;
}

BFSEditDistance::BFSResult BFSEditDistance::optimizedBFSEditDistance(
    const string& start, const string& target) {
    
    auto start_time = chrono::high_resolution_clock::now();
    
    BFSResult result;
    result.min_distance = -1;
    result.nodes_explored = 0;
    result.max_queue_size = 0;
    
    if (start == target) {
        result.min_distance = 0;
        result.all_paths.push_back(vector<string>(1, start));
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        result.computation_time = duration.count() / 1000.0;
        return result;
    }
    
    // 使用优先队列实现A*搜索的思想
    priority_queue<pair<int, State>, vector<pair<int, State>>, greater<pair<int, State>>> pq;
    unordered_map<string, int> best_cost;
    
    int h = heuristic(start, target);
    vector<string> initial_path(1, start);
    pq.push(make_pair(h, State(start, 0, initial_path)));
    best_cost[start] = 0;
    
    while (!pq.empty()) {
        result.max_queue_size = max(result.max_queue_size, (int)pq.size());
        
        pair<int, State> current_pair = pq.top();
        pq.pop();
        
        int f_cost = current_pair.first;
        State current = current_pair.second;
        result.nodes_explored++;
        
        // 剪枝：如果已经找到最优解，且当前代价大于最优解，跳过
        if (result.min_distance != -1 && current.cost >= result.min_distance) {
            continue;
        }
        
        vector<string> next_states = generateNextStates(current.current_string);
        
        for (const string& next_state : next_states) {
            int new_cost = current.cost + 1;
            
            if (next_state == target) {
                if (result.min_distance == -1 || new_cost < result.min_distance) {
                    result.min_distance = new_cost;
                    result.all_paths.clear();
                    vector<string> new_path = current.path;
                    new_path.push_back(target);
                    result.all_paths.push_back(new_path);
                } else if (new_cost == result.min_distance) {
                    vector<string> new_path = current.path;
                    new_path.push_back(target);
                    result.all_paths.push_back(new_path);
                }
            } else {
                if (best_cost.find(next_state) == best_cost.end() || 
                    new_cost < best_cost[next_state]) {
                    
                    best_cost[next_state] = new_cost;
                    vector<string> new_path = current.path;
                    new_path.push_back(next_state);
                    
                    int h_cost = heuristic(next_state, target);
                    int f_cost = new_cost + h_cost;
                    pq.push(make_pair(f_cost, State(next_state, new_cost, new_path)));
                }
            }
        }
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;
    
    return result;
}

BFSEditDistance::BFSResult BFSEditDistance::bidirectionalBFS(
    const string& start, const string& target) {
    
    auto start_time = chrono::high_resolution_clock::now();
    
    BFSResult result;
    result.min_distance = -1;
    result.nodes_explored = 0;
    result.max_queue_size = 0;
    
    if (start == target) {
        result.min_distance = 0;
        result.all_paths.push_back(vector<string>(1, start));
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        result.computation_time = duration.count() / 1000.0;
        return result;
    }
    
    // 正向搜索
    queue<pair<string, int>> forward_queue;
    unordered_map<string, int> forward_visited;
    
    // 反向搜索
    queue<pair<string, int>> backward_queue;
    unordered_map<string, int> backward_visited;
    
    forward_queue.push(make_pair(start, 0));
    forward_visited[start] = 0;
    
    backward_queue.push(make_pair(target, 0));
    backward_visited[target] = 0;
    
    while (!forward_queue.empty() || !backward_queue.empty()) {
        result.max_queue_size = max(result.max_queue_size, 
                                   (int)(forward_queue.size() + backward_queue.size()));
        
        // 正向搜索一步
        if (!forward_queue.empty()) {
            pair<string, int> current_pair = forward_queue.front();
            forward_queue.pop();
            
            string current = current_pair.first;
            int cost = current_pair.second;
            result.nodes_explored++;
            
            vector<string> next_states = generateNextStates(current);
            for (const string& next_state : next_states) {
                int new_cost = cost + 1;
                
                // 检查是否与反向搜索相遇
                if (backward_visited.find(next_state) != backward_visited.end()) {
                    int total_cost = new_cost + backward_visited[next_state];
                    if (result.min_distance == -1 || total_cost < result.min_distance) {
                        result.min_distance = total_cost;
                    }
                }
                
                if (forward_visited.find(next_state) == forward_visited.end() ||
                    new_cost < forward_visited[next_state]) {
                    forward_visited[next_state] = new_cost;
                    forward_queue.push(make_pair(next_state, new_cost));
                }
            }
        }
        
        // 反向搜索一步
        if (!backward_queue.empty()) {
            pair<string, int> current_pair = backward_queue.front();
            backward_queue.pop();
            
            string current = current_pair.first;
            int cost = current_pair.second;
            result.nodes_explored++;
            
            vector<string> next_states = generateNextStates(current);
            for (const string& next_state : next_states) {
                int new_cost = cost + 1;
                
                // 检查是否与正向搜索相遇
                if (forward_visited.find(next_state) != forward_visited.end()) {
                    int total_cost = new_cost + forward_visited[next_state];
                    if (result.min_distance == -1 || total_cost < result.min_distance) {
                        result.min_distance = total_cost;
                    }
                }
                
                if (backward_visited.find(next_state) == backward_visited.end() ||
                    new_cost < backward_visited[next_state]) {
                    backward_visited[next_state] = new_cost;
                    backward_queue.push(make_pair(next_state, new_cost));
                }
            }
        }
        
        // 如果找到解且队列中的状态代价都大于当前最优解，可以提前退出
        if (result.min_distance != -1) {
            bool can_stop = true;
            if (!forward_queue.empty() && forward_queue.front().second <= result.min_distance) {
                can_stop = false;
            }
            if (!backward_queue.empty() && backward_queue.front().second <= result.min_distance) {
                can_stop = false;
            }
            if (can_stop) break;
        }
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;
    
    return result;
}

BFSEditDistance::BFSResult BFSEditDistance::limitedBFS(
    const string& start, const string& target, int max_depth) {
    
    auto start_time = chrono::high_resolution_clock::now();
    
    BFSResult result;
    result.min_distance = -1;
    result.nodes_explored = 0;
    result.max_queue_size = 0;
    
    if (start == target) {
        result.min_distance = 0;
        result.all_paths.push_back(vector<string>(1, start));
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
        result.computation_time = duration.count() / 1000.0;
        return result;
    }
    
    queue<State> q;
    unordered_set<string> visited;
    
    vector<string> initial_path(1, start);
    q.push(State(start, 0, initial_path));
    visited.insert(start);
    
    while (!q.empty()) {
        result.max_queue_size = max(result.max_queue_size, (int)q.size());
        
        State current = q.front();
        q.pop();
        result.nodes_explored++;
        
        // 检查深度限制
        if (current.cost >= max_depth) {
            continue;
        }
        
        vector<string> next_states = generateNextStates(current.current_string);
        
        for (const string& next_state : next_states) {
            if (next_state == target) {
                int new_cost = current.cost + 1;
                if (result.min_distance == -1 || new_cost < result.min_distance) {
                    result.min_distance = new_cost;
                    result.all_paths.clear();
                    vector<string> new_path = current.path;
                    new_path.push_back(target);
                    result.all_paths.push_back(new_path);
                } else if (new_cost == result.min_distance) {
                    vector<string> new_path = current.path;
                    new_path.push_back(target);
                    result.all_paths.push_back(new_path);
                }
            } else if (visited.find(next_state) == visited.end()) {
                visited.insert(next_state);
                vector<string> new_path = current.path;
                new_path.push_back(next_state);
                q.push(State(next_state, current.cost + 1, new_path));
            }
        }
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    result.computation_time = duration.count() / 1000.0;
    
    return result;
}

vector<string> BFSEditDistance::generateNextStates(const string& current) {
    vector<string> next_states;
    
    // 删除操作
    for (int i = 0; i < current.length(); i++) {
        string next = current;
        next.erase(i, 1);
        next_states.push_back(next);
    }
    
    // 插入操作
    for (int i = 0; i <= current.length(); i++) {
        for (char c = 'a'; c <= 'z'; c++) {
            string next = current;
            next.insert(i, 1, c);
            next_states.push_back(next);
        }
    }
    
    // 替换操作
    for (int i = 0; i < current.length(); i++) {
        for (char c = 'a'; c <= 'z'; c++) {
            if (c != current[i]) {
                string next = current;
                next[i] = c;
                next_states.push_back(next);
            }
        }
    }
    
    return next_states;
}

int BFSEditDistance::heuristic(const string& current, const string& target) {
    // 简单的启发式函数：字符串长度差的绝对值
    return abs((int)current.length() - (int)target.length());
}

string BFSEditDistance::stateToString(const string& str) {
    return str;
}