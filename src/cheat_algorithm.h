//
// Created by liuwei on 4/30/17.
//

#ifndef CHEAT_CHEAT_ALGORITHM_H
#define CHEAT_CHEAT_ALGORITHM_H

#endif //CHEAT_CHEAT_ALGORITHM_H

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

namespace cheat{
    typedef std::map<std::string, std::string>::value_type map_pair;
    typedef std::set<std::pair<int, int>>::iterator pair_iterator;
    static std::set<std::string> retain;
    static std::map<std::string, std::string> params;
    static std::map<std::string, std::string> key_words;
    static std::map<int, std::vector<std::string>> cache;
    std::string get_brackets(const std::string &x);
    int lcs(const std::string& a, const std::string &b);
    double frequency_statistic(const std::string a, const std::string b);
    void deal_code_file(const std::string &x);
    void init();
    std::string get_param_name(const std::string &x);
    double cal_common_substring(const std::string &a, const std::string &b);
}