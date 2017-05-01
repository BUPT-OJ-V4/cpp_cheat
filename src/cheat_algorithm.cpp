//
// Created by liuwei on 4/30/17.
//
#include "cheat_algorithm.h"

double cheat::frequency_statistic(const std::string a, const std::string b) {
    return 0.5;
}

int cheat::lcs(const std::string& a, const std::string &b) {
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
    return dp[cur][b.length()];
}

std::string cheat::get_brackets(const std::string &x) {
    std::string ans;
    static std::string target = "(){}[]";
    for (int i = 0; i < x.length(); i ++) {
        for(int j = 0; j < target.length(); j ++){
            if (x[i] == target[j]){
                ans += x[i];
            }
        }
    }
    return ans;
}

void cheat::init() {
    static std::string temps[] = {"and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break", "case",
                         "catch", "char", "class", "compl", "const", "const_cast", "continue", "default",
                         "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern",
                         "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable",
                           "namespace", "new", "not", "strlen", "operator", "or", "print", "private","size",
                           "protected", "public", "register", "reinterpret_cast", "return", "short", "signed",
                           "struct", "sizeof", "static", "static_cast", "throw", "switch", "template", "this",
                           "vector", "true", "try", "typedef", "using", "typename", "union", "unsigned", "virtual",
                           "void", "volatile", "scanf", "while", "xor", "memset", "include", "queue", "cin",
                           "cout", "string", "std", "d"};
    const static map_pair data[] ={
            map_pair("break", "l"), map_pair("short", "c"), map_pair("return", "n"), map_pair("struct", "g"),
            map_pair("for", "h"), map_pair("do", "s"), map_pair("int", "a"), map_pair("double", "r"), map_pair("float", "q"),
            map_pair("while", "i"), map_pair("long", "b"), map_pair("else", "k"), map_pair("char", "e"),
            map_pair("unsigned", "u"), map_pair("switch", "d"), map_pair("continue", "m"), map_pair("signed", "t"),
            map_pair("false", "p"), map_pair("true", "o"), map_pair("class", "f"), map_pair("if", "j")
    };
    for (std::string x: temps) {
        cheat::retain.insert(x);
    }
    cheat::key_words = std::map<std::string, std::string>(data, data + 21);

}

std::string cheat::get_param_name(const std::string &x) {
    std::map<std::string, std::string>::iterator ite = cheat::params.find(x);
    if (ite != params.end()) {
        return ite->second;
    }
    std::string param = "param_" + std::to_string(params.size());
    cheat::params[x] = param;
    return param;
}

void normalization(const std::string& idx, char* buffer, const int& length) {

}

void cheat::deal_code_file(const std::string &code_url) {
    unsigned long l = code_url.length();
    /*
    std::string cmd = "cd ./data && curl -O http://10.105.240.51" + code_url;
    system(cmd.c_str());
     */
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
    std::cout <<" original code " << std::endl;
    std::cout << buffer << std::endl;
    normalization(code_name, buffer, length);
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
        pair_iterator ite = segment_1.upper_bound(std::pair(now.second.first, -1));
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
        pair_iterator ite2 = segment_2.upper_bound(std::pair(now.second.second, -1));
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
    return 2.0 * ans / (a.length() + b.length());
}
