//
// Created by liuwei on 4/30/17.
//
#include "cheat_algorithm.h"
using namespace cheat;

namespace cheat
{

typedef unsigned int uint;
std::map<std::string, char> key_words;
int key_symbol[300];
std::map<int, std::string> cache;
std::map<int, std::string> brackets;
std::map<int, std::string> allcode;

void init() {
    std::vector<std::string> words = {"int","long","short","switch","char","class","struct","for","while","if","else","break","continue","return","true","false","float","double","do","signed","unsigned"};
    char symbol[] = "[]{}()&|^%+-*/:;?!.\"\',=#<>_\\";
    memset(key_symbol, -1, sizeof key_symbol);
    size_t len = words.size(), len2 = strlen(symbol);
    for(size_t i = 0; i < len; i ++) {
        key_words[words[i]] = (char)('a' + i);
    }
    std::string empty = "";
    for(int i = 0; i < 26; i ++) {
        int temp = i + 'a';
        int temp2 = i + 'A';
        key_symbol[temp] = i;
        key_symbol[temp2] = i + 26;
        if (i < 10) {
            int temp3 = i + '0';
            key_symbol[temp3] = i + 52;
        }
    }
    for(int i = 0; i < len2; i ++) {
        key_symbol[symbol[i]] = i + 62;
    }
}

void clear() {
    cache.clear();
    brackets.clear();
}

double frequency_statistic(const std::string & a, const std::string & b) {
    uint num[2][105], len[2] = {(uint)a.length(), (uint)b.length()}; memset(num, 0, sizeof num);
    const char* ch[2];
    ch[0] = a.c_str();
    ch[1] = b.c_str();
    for(int i = 0; i < 2; i ++) {
        for(int j = 0; j < len[i]; j ++) {
            if ((int)ch[i][j] > 255 || key_symbol[ch[i][j]] == -1) continue;
            num[i][key_symbol[ch[i][j]]] ++;
        }
    }
    long long up = 0, down1 = 0, down2 = 0;
    for(int i = 0; i < 90; i ++) {
        up += num[0][i] * num[1][i];
        down1 += num[0][i] * num[0][i];
        down2 += num[1][i] * num[1][i];
    }
    if (!down1 && !down2) {
        return 100;
    }
    return up * 100 / std::sqrt(down1 * down2);
}

double lcs(const std::string& a, const std::string &b) {
    int dp[2][b.length() + 2];
    memset(dp, 0, sizeof dp);
    int cur = 0;
    for(int i = 0; i < a.length(); i ++) {
        cur ^= 1;
        memset(dp[cur], 0, sizeof dp[cur]);
        for(int j = 0; j < b.length(); j ++) {
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

void normalization(const int& idx, const std::string & buffer) {

    boost::regex reg("(\\/\\*(\\s|.)*?\\*\\/)|(\\/\\/.*?(\\r|\\n))", boost::regex::icase);
    boost::regex expression("\\w+|{|}");
    boost::regex space("(\\s|\\r\\n)");
    std::string res = boost::regex_replace(buffer, reg, "", boost::match_default | boost::format_all);
    boost::sregex_iterator it(res.begin(), res.end(), expression);
    boost::sregex_iterator end;
    std::string temp;
    for (; it != end; ++it) {
        auto ite = key_words.find(it->str());
        if (ite != key_words.end()) {
            temp += ite->second;
        }
        else if(it->str() == "{" || it->str() == "}") {
            temp += it->str();
        }
    }
    brackets[idx] = temp;
    cache[idx] = boost::regex_replace(res, space, "");
    allcode[idx] = res;
}

void deal_code_file(const int &idx, const std::string& code_name) {
    std::ifstream t(code_name.c_str());
    std::string s(std::istreambuf_iterator<char>(t), (std::istreambuf_iterator<char>()));
    normalization(idx, s);
}

double cal_common_substring(std::string const& a, std::string const& b) {
    const static int MIN_TEXT_LENTH = 4;
    int dp[2][b.length() + 2];
    char A[a.length() + 1], B[b.length() + 1];
    size_t ans = 0, len1 = a.length(), len2 = b.length();
    a.copy(A, len1);
    b.copy(B, len2);
    while(true){
        memset(dp, 0, sizeof dp);
        int cur = 0, res = 0, u = 0, v = 0;
        for(int i = 0; i < len1; i ++) {
            cur ^= 1;
            for(int j = 0; j < len2; j ++) {
                if (B[j] == '$' || A[i] == '$' || A[i] != B[j]) {
                    dp[cur][j + 1] = 0;
                }
                else{
                    dp[cur][j + 1] = 1 + dp[cur ^ 1][j];
                    if (dp[cur][j + 1] > res) {
                        res = dp[cur][j + 1];
                        u = i;
                        v = j;
                    }
                }
            }
        }

        if (res <= MIN_TEXT_LENTH) break;
        ans += res;
        int ret = res;

        if (u + 1 < len1 && A[u + 1] != '$') A[u - res + 1] = '$';
        else res ++;
        for(int i = u + 1; i < len1; i ++) {
            A[i - res + 1] = A[i];
        }
        len1 -= res - 1;
        A[len1] = '\0';

        if (v + 1 < len2 && B[v + 1] != '$') B[v - ret + 1] = '$';
        else ret ++;
        for(int i = v + 1; i < len2; i ++) {
            B[i - ret + 1] = B[i];
        }
        len2 -= ret - 1;
        B[len2] = '\0';
    }
    return ans * 200.0 / (a.length() + b.length());
}
}
