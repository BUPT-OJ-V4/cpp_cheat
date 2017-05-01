//
// Created by liuwei on 4/30/17.
//
#include "cheat_algorithm.h"

void cheat::init() {
    const static map_pair<std::string, std::string> data[] ={
            map_pair<std::string, std::string>("break", "l"), map_pair<std::string, std::string>("short", "c"), map_pair<std::string, std::string>("return", "n"), map_pair<std::string, std::string>("struct", "g"),
            map_pair<std::string, std::string>("for", "h"), map_pair<std::string, std::string>("do", "s"), map_pair<std::string, std::string>("int", "a"), map_pair<std::string, std::string>("double", "r"), map_pair<std::string, std::string>("float", "q"),
            map_pair<std::string, std::string>("while", "i"), map_pair<std::string, std::string>("long", "b"), map_pair<std::string, std::string>("else", "k"), map_pair<std::string, std::string>("char", "e"),
            map_pair<std::string, std::string>("unsigned", "u"), map_pair<std::string, std::string>("switch", "d"), map_pair<std::string, std::string>("continue", "m"), map_pair<std::string, std::string>("signed", "t"),
            map_pair<std::string, std::string>("false", "p"), map_pair<std::string, std::string>("true", "o"), map_pair<std::string, std::string>("class", "f"), map_pair<std::string, std::string>("if", "j")
    };
    cheat::key_words = std::map<std::string, std::string>(data, data + 21);
    const static map_pair<char, int> data2[] = {
            map_pair<char,int>('(', 66), map_pair<char,int>(',', 83), map_pair<char,int>('0', 52), map_pair<char,int>('4', 56), map_pair<char,int>('8', 60), map_pair<char,int>('<', 86), map_pair<char,int>('D', 29), map_pair<char,int>('H', 33), map_pair<char,int>('L', 37),
            map_pair<char,int>('P', 41), map_pair<char,int>('T', 45), map_pair<char,int>('X', 49), map_pair<char,int>('\\', 89), map_pair<char,int>('d', 3), map_pair<char,int>('h', 7), map_pair<char,int>('l', 11), map_pair<char,int>('p', 15), map_pair<char,int>('t', 19),
            map_pair<char,int>('x', 23), map_pair<char,int>('|', 69), map_pair<char,int>('#', 85), map_pair<char,int>('\'', 82), map_pair<char,int>('+', 72), map_pair<char,int>('/', 75), map_pair<char,int>('3', 55), map_pair<char,int>('7', 59), map_pair<char,int>(';', 77),
            map_pair<char,int>('?', 78), map_pair<char,int>('C', 28), map_pair<char,int>('G', 32), map_pair<char,int>('K', 36), map_pair<char,int>('O', 40), map_pair<char,int>('S', 44), map_pair<char,int>('W', 48), map_pair<char,int>('[', 62), map_pair<char,int>('_', 88),
            map_pair<char,int>('c', 2), map_pair<char,int>('g', 6), map_pair<char,int>('k', 10), map_pair<char,int>('o', 14), map_pair<char,int>('s', 18), map_pair<char,int>('w', 22), map_pair<char,int>('{', 64), map_pair<char,int>('"', 81), map_pair<char,int>('&', 68),
            map_pair<char,int>('*', 74), map_pair<char,int>('.', 80), map_pair<char,int>('2', 54), map_pair<char,int>('6', 58), map_pair<char,int>(':', 76), map_pair<char,int>('>', 87), map_pair<char,int>('B', 27), map_pair<char,int>('F', 31), map_pair<char,int>('J', 35),
            map_pair<char,int>('N', 39), map_pair<char,int>('R', 43), map_pair<char,int>('V', 47), map_pair<char,int>('Z', 51), map_pair<char,int>('^', 70), map_pair<char,int>('b', 1), map_pair<char,int>('f', 5), map_pair<char,int>('j', 9), map_pair<char,int>('n', 13),
            map_pair<char,int>('r', 17), map_pair<char,int>('v', 21), map_pair<char,int>('z', 25), map_pair<char,int>('!', 79), map_pair<char,int>('%', 71), map_pair<char,int>(')', 67), map_pair<char,int>('-', 73), map_pair<char,int>('1', 53), map_pair<char,int>('5', 57),
            map_pair<char,int>('9', 61), map_pair<char,int>('=', 84), map_pair<char,int>('A', 26), map_pair<char,int>('E', 30), map_pair<char,int>('I', 34), map_pair<char,int>('M', 38), map_pair<char,int>('Q', 42), map_pair<char,int>('U', 46), map_pair<char,int>('Y', 50),
            map_pair<char,int>(']', 63), map_pair<char,int>('a', 0), map_pair<char,int>('e', 4), map_pair<char,int>('i', 8), map_pair<char,int>('m', 12), map_pair<char,int>('q', 16), map_pair<char,int>('u', 20), map_pair<char,int>('y', 24), map_pair<char,int>('}', 65)
    };
    cheat::key_symbol = std::map(data2, data2 + 90);
}

void cheat::clear() {
    cheat::cache.clear();
    cheat::brackets.clear();
}
long long cheat::frequency_statistic(const std::string a, const std::string b) {
    int num[2][105]; memset(num, 0, sizeof num);
    const std::string & ch[2] = {a, b};
    for(int i = 0; i < 2; i ++) {
        for(int j = 0; j < ch[i].length(); j ++) {
            map_ite it = key_symbol.find(ch[i][j]);
            if (it == key_symbol.end()) continue;
            num[it->second] ++;
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
    return up * up * 100 / (down1 * down2);
}

double cheat::lcs(const std::string& a, const std::string &b) {
    int dp[2][b.length() + 2];
    memset(dp, 0, sizeof dp);
    int cur = 0;
    for(int i = 0; i < a.length(); i ++) {
        cur ^= 1;
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

void normalization(const int& idx, char* buffer, const int& length) {
    boost::regex reg("(\\/\\*(\\s|.)*?\\*\\/)|(\\/\\/.*)|", boost::regex::icase);
    boost::regex expression("\\w+|{|}");
    boost::regex space("(\\s|\\r\\n)");
    std::string res = boost::regex_replace(buffer, reg, "", boost::format_default | boost::format_all);
    boost::sregex_iterator it(res.begin(), res.end(), expression);
    boost::sregex_iterator end;
    std::string temp;
    for (; it != end; ++it) {
        map_ite ite = cheat::key_words.find(it->str());
        if (ite != cheat::key_words.end()) {
            temp += ite->second;
        }
        else if(it->str() == "{" || it->str() == "}") {
            temp += it->str();
        }
    }
    cheat::brackets[idx] = temp;
    cheat::cache[idx] = boost::regex_replace(res, space, "");
}


void cheat::deal_code_file(const int &idx, const std::string &code_url) {
    unsigned long l = code_url.length();
    std::string code_name ="data/";
    char *buffer;
    for (int i = (int)l - 6, j = 0; i < l; i ++, j ++) {
        code_name += code_url[i];
    }
    std::ifstream t;
    t.open(code_name.c_str());      // open input file
    t.seekg(0, std::ios::end);    // go to the end
    long long length = t.tellg();           // report location (this is the length)
    t.seekg(0, std::ios::beg);    // go back to the beginning
    buffer = new char[length + 1];    // allocate memory for a buffer of appropriate dimension
    t.read(buffer, length);       // read the whole file into the buffer
    t.close();                    // close file handle /* s//
    buffer[length] = '\0';
    normalization(idx, buffer, length);
}

double cheat::cal_common_substring(const std::string &a, const std::string &b) {
    const static MIN_TEXT_LENTH = 4;
    int dp[2][b.length() + 1];
    memset(dp, 0, sizeof dp);
    std::vector<std::pair<int, std::pair<int, int>>> vs;
    int cur = 0;
    for(int i = 0; i < a.length(); i ++) {
        cur ^= 1;
        for(int j = 0; j < b.length(); j ++) {
            dp[cur][j + 1] = a[i] == b[j] ? (1 + dp[cur ^ 1][j]) : 0;
            vs.emplace_back(dp[cur][j + 1], std::make_pair(i, j));
        }
    }
    std::sort(vs.begin(), vs.end());
    std::set<std::pair<int, int> > segment_1, segment_2;
    int ans = 0;
    while(!vs.empty() && vs.back().first > MIN_TEXT_LENTH){
        std::pair<int, std::pair<int, int>>& now = vs.back();
        bool validate = true;
        pair_ite ite = segment_1.upper_bound(std::pair(now.second.first, -1));
        if (ite != segment_1.end() && ite->first == now.second.first) {
            validate = false;
        }
        else if(ite != segment_1.begin()) {
            -- ite;
            if (ite->second >= now.second.first) {
                validate = false;
            }
        }
        if (!validate) {
            vs.pop_back();
            continue;
        }
        pair_ite ite2 = segment_2.upper_bound(std::pair(now.second.second, -1));
        if (ite2 != segment_2.end() && ite2->first == now.second.second) {
            validate = false;
        }
        else if(ite2 != segment_2.begin()) {
            -- ite2;
            if (ite2->second >= now.second.second) {
                validate = false;
            }
        }
        if (!validate) {
            vs.pop_back();
            continue;
        }
        ans += now.first;
        segment_1.insert(std::make_pair(now.second.first - now.first + 1, now.second.first));
        segment_2.insert(std::make_pair(now.second.second - now.first + 1, now.second.second));
        vs.pop_back();
    }
    return ans * 200.0 / (a.length() + b.length());
}
