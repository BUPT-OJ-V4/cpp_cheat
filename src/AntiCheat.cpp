//
// Created by liuwei on 7/29/17.
//

#include "AntiCheat.h"
#include <cstring>
#include <iostream>
#include <fstream>

#include <boost/regex.hpp>

CommonData AntiCheat::mData;

double AntiCheat::FrequencyStatistic(const std::string & a, const std::string & b)
{
    size_t num[2][105], len[2] = {a.length(), b.length()}; memset(num, 0, sizeof num);
    const char* ch[2];
    ch[0] = a.c_str();
    ch[1] = b.c_str();
    for(size_t i = 0; i < 2; i ++) {
        for(size_t j = 0; j < len[i]; j ++) {
	  if ((int)ch[i][j] > 255 || (int)ch[i][j] < 0 || mData.mKeySymbol[ch[i][j]] == -1) continue;
            num[i][mData.mKeySymbol[ch[i][j]]] ++;
        }
    }
    long long up = 0, down1 = 0, down2 = 0;
    for(size_t i = 0; i < 90; i ++) {
        up += num[0][i] * num[1][i];
        down1 += num[0][i] * num[0][i];
        down2 += num[1][i] * num[1][i];
    }
    if (!down1 && !down2) {
        return 100;
    }
    return up * 100 / std::sqrt(down1 * down2 * 1.0);
}

double AntiCheat::Lcs(const std::string& a, const std::string &b)
{
    int dp[2][b.length() + 2];
    memset(dp, 0, sizeof dp);
    int cur = 0;
    for(size_t i = 0; i < a.length(); i ++) {
        cur ^= 1;
        memset(dp[cur], 0, sizeof dp[cur]);
        for(size_t j = 0; j < b.length(); j ++) {
            if (a[i] == b[j]) {
                dp[cur][j + 1] = dp[cur^1][j] + 1;
            }
            else{
                dp[cur][j + 1] = std::max(dp[cur][j], dp[cur ^ 1][j + 1]);
            }
        }
    }
    return dp[cur][b.length()] * 200.0 / (a.length() + b.length());
}

void AntiCheat::AddSubmission(const int& idx, const std::string & buffer, const std::string & username)
{
    std::string validString;
    for(auto &c: buffer) {
      if ((int)c < 0 || (int)c > 255) continue;
      validString += c;
    }
    static boost::regex reg("(\\/\\*(\\s|.)*?\\*\\/)|(\\/\\/.*?(\\r|\\n))", boost::regex::icase);
    static boost::regex expression("\\w+|{|}");
    static boost::regex space("(\\s|\\r\\n)");
    std::string res = boost::regex_replace(validString, reg, "", boost::match_default | boost::format_all);
    boost::sregex_iterator it(res.begin(), res.end(), expression);
    boost::sregex_iterator end;
    std::string temp;
    for (; it != end; ++it) {
        auto ite = mData.mKeyWord.find(it->str());
        if (ite != mData.mKeyWord.end()) {
            temp += ite->second;
        }
        else if(it->str() == "{" || it->str() == "}") {
            temp += it->str();
        }
    }
    mBracket[idx] = temp;
    mCache[idx] = boost::regex_replace(res, space, "");
    mAllcode[idx] = res;
    mUserinfo[idx] = username;
}

double AntiCheat::CalCommonSubstring(std::string const& a, std::string const& b)
{
    const static int MIN_TEXT_LENTH = 4;
    unsigned short dp[2][b.length() + 2];
    size_t ans = 0, len1 = a.length(), len2 = b.length();
    char A[len1 + 1], B[len2 + 1];
    a.copy(A, len1);
    b.copy(B, len2);
    while(true){
        size_t cur = 0, res = 0, u = 0, v = 0;
        std::vector<size_t> pos[256];
        for(size_t i = 0; i < len2; i ++) {
            pos[B[i]].push_back(i);
        }
        memset(dp[0], 0, sizeof(unsigned short) * (len2 + 1));
        for(size_t i = 0; i < len1; i ++) {
            cur ^= 1;
            memset(dp[cur], 0, sizeof(unsigned short) * (len2 + 1));
            if (A[i] == '$') continue;
            for(auto j: pos[A[i]]) {
                dp[cur][j + 1] = (unsigned short)1 + dp[cur ^ 1][j];
                if (dp[cur][j + 1] > res) {
                    res = dp[cur][j + 1];
                    u = i;
                    v = j;
                }
            }
        }

        if (res <= MIN_TEXT_LENTH) break;
        ans += res;
        size_t ret = res;

        if (u + 1 < len1 && A[u + 1] != '$') A[u - res + 1] = '$';
        else res ++;
        for(size_t i = u + 1; i < len1; i ++) {
            A[i - res + 1] = A[i];
        }
        len1 -= res - 1;
        A[len1] = '\0';

        if (v + 1 < len2 && B[v + 1] != '$') B[v - ret + 1] = '$';
        else ret ++;
        for(size_t i = v + 1; i < len2; i ++) {
            B[i - ret + 1] = B[i];
        }
        len2 -= ret - 1;
        B[len2] = '\0';
    }
    return ans * 200.0 / (a.length() + b.length() + 1);
}

double AntiCheat::Calc(const int & a, const int & b, std::string& user1, std::string& user2)
{
    double res = Lcs(mBracket[a], mBracket[b]) + CalCommonSubstring(mCache[a], mCache[b]);
    res *= 0.5;
    double third = FrequencyStatistic(mAllcode[a], mAllcode[b]);
    user1 = mUserinfo[a];
    user2 = mUserinfo[b];
    if (third > 99.0) return third;
    return res;
}
