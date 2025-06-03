#include "test_data.h"
#include <algorithm>
#include <chrono>

using namespace std;

mt19937 TestDataGenerator::rng(chrono::steady_clock::now().time_since_epoch().count());

vector<TestDataGenerator::TestCase> TestDataGenerator::getClassicTestCases() {
    return {
        {"kitten", "sitting", 3, "Classic example: kitten -> sitting"},
        {"", "abc", 3, "Empty string to non-empty string"},
        {"abc", "", 3, "Non-empty string to empty string"},
        {"", "", 0, "Two empty strings"},
        {"same", "same", 0, "Identical strings"},
        {"abc", "ab", 1, "Delete operation"},
        {"ab", "abc", 1, "Insert operation"},
        {"abc", "axc", 1, "Replace operation"},
        {"intention", "execution", 5, "Longer string test"},
        {"algorithm", "logarithm", 2, "Algorithm related words"},
        {"sunday", "saturday", 3, "Day names"},
        {"cat", "dog", 3, "Completely different short words"},
        {"exponential", "polynomial", 6, "Complexity related words"}
    };
}

vector<TestDataGenerator::TestCase> TestDataGenerator::getSpellingTestCases() {
    return {
        {"algorithm", "algorith", 1, "Missing letter"},
        {"algorithm", "algoritm", 1, "Missing letter"},
        {"algorithm", "algorythm", 1, "Letter substitution"},
        {"receive", "recieve", 2, "Common spelling mistake"},
        {"necessary", "neccessary", 1, "Double letter error"},
        {"definitely", "definately", 1, "Vowel error"},
        {"separate", "seperate", 1, "Vowel error"},
        {"occurrence", "occurence", 1, "Double letter error"},
        {"embarrass", "embarass", 1, "Double letter error"},
        {"accommodate", "accomodate", 1, "Double letter error"}
    };
}

pair<string, string> TestDataGenerator::generateRandomStringPair(int min_length, int max_length, char charset) {
    uniform_int_distribution<int> length_dist(min_length, max_length);
    
    int len1 = length_dist(rng);
    int len2 = length_dist(rng);
    
    string str1 = generateRandomString(len1, charset);
    string str2 = generateRandomString(len2, charset);
    
    return make_pair(str1, str2);
}

pair<string, string> TestDataGenerator::generateSimilarStrings(const string& base, double similarity_ratio) {
    int num_errors = static_cast<int>((1.0 - similarity_ratio) * base.length());
    num_errors = max(1, num_errors);
    
    string modified = introduceErrors(base, num_errors);
    return make_pair(base, modified);
}

vector<pair<string, string>> TestDataGenerator::generateBatchTestData(int count, int min_length, int max_length) {
    vector<pair<string, string>> test_data;
    test_data.reserve(count);
    
    for (int i = 0; i < count; i++) {
        test_data.push_back(generateRandomStringPair(min_length, max_length));
    }
    
    return test_data;
}

vector<pair<string, string>> TestDataGenerator::generatePerformanceTestData() {
    vector<pair<string, string>> test_data;
    
    // Different scale test data
    vector<int> sizes = {10, 20, 50, 100, 200, 500};
    
    for (int size : sizes) {
        // Generate several test cases for each size
        for (int i = 0; i < 3; i++) {
            string str1 = generateRandomString(size);
            string str2 = generateRandomString(size);
            test_data.emplace_back(str1, str2);
        }
    }
    
    return test_data;
}

vector<string> TestDataGenerator::generateSpellingCandidates(const string& target) {
    vector<string> candidates;
    
    // Generate some similar candidate words
    candidates.push_back(introduceErrors(target, 1));  // 1 error
    candidates.push_back(introduceErrors(target, 2));  // 2 errors
    candidates.push_back(introduceErrors(target, 1));  // Another 1 error version
    
    // Add some candidates with different lengths
    if (target.length() > 2) {
        candidates.push_back(target.substr(0, target.length() - 1));  // Remove last character
        candidates.push_back(target.substr(1));  // Remove first character
    }
    
    candidates.push_back(target + "s");  // Add character
    candidates.push_back("x" + target);  // Add character at front
    
    return candidates;
}

string TestDataGenerator::generateRandomString(int length, char start_char, int charset_size) {
    string result;
    result.reserve(length);
    
    uniform_int_distribution<int> char_dist(0, charset_size - 1);
    
    for (int i = 0; i < length; i++) {
        result += static_cast<char>(start_char + char_dist(rng));
    }
    
    return result;
}

string TestDataGenerator::introduceErrors(const string& original, int num_errors) {
    if (original.empty()) return original;
    
    string result = original;
    uniform_int_distribution<int> pos_dist(0, result.length() - 1);
    uniform_int_distribution<int> op_dist(0, 2);  // 0=replace, 1=insert, 2=delete
    uniform_int_distribution<int> char_dist('a', 'z');
    
    for (int i = 0; i < num_errors && !result.empty(); i++) {
        int operation = op_dist(rng);
        int pos = pos_dist(rng);
        
        if (pos >= result.length()) pos = result.length() - 1;
        
        switch (operation) {
            case 0:  // Replace
                result[pos] = static_cast<char>(char_dist(rng));
                break;
            case 1:  // Insert
                result.insert(pos, 1, static_cast<char>(char_dist(rng)));
                break;
            case 2:  // Delete
                if (!result.empty()) {
                    result.erase(pos, 1);
                }
                break;
        }
    }
    
    return result;
}