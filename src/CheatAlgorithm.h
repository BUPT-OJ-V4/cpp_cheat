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

class InitialData{
public:
    InitialData() {
        std::vector<std::string> words = {"int","long","short","switch","char","class","struct","for","while","if","else","break","continue","return","true","false","float","double","do","signed","unsigned"};
        char symbol[] = "[]{}()&|^%+-*/:;?!.\"\',=#<>_\\";
        memset(mKeySymbol, -1, sizeof mKeySymbol);
        size_t len = words.size(), len2 = strlen(symbol);
        for(size_t i = 0; i < len; i ++) {
            mKeyWord[words[i]] = (char)('a' + i);
        }
        std::string empty = "";
        for(int i = 0; i < 26; i ++) {
            int temp = i + 'a';
            int temp2 = i + 'A';
            mKeySymbol[temp] = i;
            mKeySymbol[temp2] = i + 26;
            if (i < 10) {
                int temp3 = i + '0';
                mKeySymbol[temp3] = i + 52;
            }
        }
        for(int i = 0; i < len2; i ++) {
            mKeySymbol[symbol[i]] = i + 62;
        }
    }
    ~InitialData() { }
private:
    std::map<std::string, char> mKeyWord;
    int mKeySymbol[300];
};


class CheatAlgorithm{
pulic:
    CheatAlgorithm();
    ~CheatAlgorithm();
    double calc(const int& sub1, const int& sub2);
    const std::string& GetUserInfo(const std::string& sub)
    {
        return mUserinfo[sub];
    }
    const std::string& GetProblemId()
    {
        return mProblemId;
    }
    void AddSubmission(const int& idx, const std::string &x, const std::string & username);
private:
    double Lcs(const std::string& a, const std::string &b);
    double FrequencyStatistic(const std::string &a, const std::string &b);
    double CalCommonSubstring(std::string const & a, std::string const & b);
private:
    std::map<int, std::string> mCache;
    std::map<int, std::string> mBracket;
    std::map<int, std::string> mAllcode;
    std::map<int, std::string> mUserinfo;
    std::string mProblemId;
private:
    static InitialData mData;
};
