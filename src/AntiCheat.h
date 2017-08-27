//
// Created by liuwei on 7/29/17.
//

#ifndef CHEAT_ANTICHEAT_H
#define CHEAT_ANTICHEAT_H
#include <map>
#include <set>
#include <vector>
template <typename K, typename V>
using map_pair = typename std::map<K, V>::value_type;
template <typename K, typename V>
using map_ite = typename std::map<K, V>::iterator;
template <typename K, typename V>
using pair_ite=typename std::set<typename std::pair<K, V>>::iterator;
class CommonData {
public:
    std::map<std::string, char> key_words;
    int key_symbol[300];
    CommonData();
};

class AntiCheat {
public:
    typedef std::pair<std::pair<int, int>, double> Res;
    AntiCheat() {}
    double calc(const int & a, const int & b);
    void normalization(const int& idx, const std::string &x, const std::string & username);
private:
    double lcs(const std::string& a, const std::string &b);
    double frequencyStatistic(const std::string &a, const std::string &b);
    void dealCodeFile(const int& idx, const std::string &x);
    double calCommonSubstring(std::string const & a, std::string const & b);
public:
    static CommonData commonData;
    std::map<int, std::string> _userinfo;
    std::string _problem_id;
private:
    std::map<int, std::string> _cache;
    std::map<int, std::string> _brackets;
    std::map<int, std::string> _allcode;
};


#endif //CHEAT_ANTICHEAT_H
