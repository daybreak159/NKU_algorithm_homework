#ifndef TEST_DATA_H
#define TEST_DATA_H

#include <string>
#include <vector>
#include <utility>
#include <random>

/**
 * 测试数据生成工具类
 */
class TestDataGenerator {
public:
    /**
     * 预定义的测试用例
     */
    struct TestCase {
        std::string str1;
        std::string str2;
        int expected_distance;
        std::string description;
    };
    
    /**
     * 获取经典测试用例
     */
    static std::vector<TestCase> getClassicTestCases();
    
    /**
     * 获取拼写纠错测试用例
     */
    static std::vector<TestCase> getSpellingTestCases();
    
    /**
     * 生成随机字符串对
     */
    static std::pair<std::string, std::string> generateRandomStringPair(
        int min_length = 5, int max_length = 20, char charset = 'a');
    
    /**
     * 生成指定相似度的字符串对
     */
    static std::pair<std::string, std::string> generateSimilarStrings(
        const std::string& base, double similarity_ratio = 0.8);
    
    /**
     * 生成批量测试数据
     */
    static std::vector<std::pair<std::string, std::string>> generateBatchTestData(
        int count, int min_length = 5, int max_length = 20);
    
    /**
     * 生成性能测试数据（不同规模）
     */
    static std::vector<std::pair<std::string, std::string>> generatePerformanceTestData();
    
    /**
     * 生成拼写纠错候选词集合
     */
    static std::vector<std::string> generateSpellingCandidates(const std::string& target);

public:
    static std::mt19937 rng;
    static std::string generateRandomString(int length, char start_char = 'a', int charset_size = 26);
    static std::string introduceErrors(const std::string& original, int num_errors);
};

#endif // TEST_DATA_H