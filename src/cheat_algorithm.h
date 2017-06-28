//
// Created by liuwei on 4/30/17.
//

#ifndef CHEAT_CHEAT_ALGORITHM_H
#define CHEAT_CHEAT_ALGORITHM_H

#endif //CHEAT_CHEAT_ALGORITHM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/regex.hpp>

template <typename K, typename V>
using map_pair = typename std::map<K, V>::value_type;

template <typename K, typename V>
using map_ite = typename std::map<K, V>::iterator;

template <typename K, typename V>
using pair_ite=typename std::set<typename std::pair<K, V>>::iterator;

namespace cheat{
    extern std::map<std::string, char> key_words;
    extern int key_symbol[300];
    extern std::map<int, std::string> cache;
    extern std::map<int, std::string> brackets;
    extern std::map<int, std::string> allcode;
    extern std::map<int, std::string> userinfo;
    double lcs(const std::string& a, const std::string &b);
    double frequency_statistic(const std::string &a, const std::string &b);
    void deal_code_file(const int& idx, const std::string &x);
    void normalization(const int& idx, const std::string &x, const std::string & username);
    void init();
    double cal_common_substring(std::string const & a, std::string const & b);
    void clear();
}