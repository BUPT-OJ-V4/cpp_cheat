//
// Created by liuwei on 7/29/17.
//

#ifndef CHEAT_ANTICHEAT_H
#define CHEAT_ANTICHEAT_H

#include <cstring>
#include "common.h"

class CommonData {
public:
    std::map<std::string, char> mKeyWord;
    int mKeySymbol[300];
    CommonData() {
        std::vector<std::string> words = {"int","long","short","switch","char","class","struct","for","while","if","else","break","continue","return","true","false","float","double","do","signed","unsigned", "scanf", "printf", "cin", "cout", "else if"};
        char symbol[] = "[]{}()&|^%+-*/:;?!.\"\',=#<>_\\";
        memset(mKeySymbol, -1, sizeof mKeySymbol);
        size_t len = words.size(), len2 = strlen(symbol);
        for(size_t i = 0; i < len; i ++) {
            mKeyWord[words[i]] = (char)('a' + i);
        }
        std::string empty = "";
        for(size_t i = 0; i < 26; i ++) {
            int temp = i + 'a';
            int temp2 = i + 'A';
            mKeySymbol[temp] = i;
            mKeySymbol[temp2] = i + 26;
            if (i < 10) {
                int temp3 = i + '0';
                mKeySymbol[temp3] = i + 52;
            }
        }
        for(size_t i = 0; i < len2; i ++) {
            mKeySymbol[symbol[i]] = i + 62;
        }
    }
    ~CommonData() { }        
};

class AntiCheat {
public:

    AntiCheat() {}
    ~AntiCheat() {}
    double Calc(const int & a, const int & b, std::string& user1, std::string& user2);
    const std::string& GetUserInfo(const int& sub)
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
    static CommonData mData;
};


#endif //CHEAT_ANTICHEAT_H
