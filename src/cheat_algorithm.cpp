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
    key_symbol['(']=66; key_symbol[',']=83; key_symbol['0']=52; key_symbol['4']=56; key_symbol['8']=60; key_symbol['<']=86; key_symbol['D']=29; key_symbol['H']=33; key_symbol['L']=37;
    key_symbol['P']=41; key_symbol['T']=45; key_symbol['X']=49; key_symbol['\\']=89; key_symbol['d']=3; key_symbol['h']=7; key_symbol['l']=11; key_symbol['p']=15; key_symbol['t']=19;
    key_symbol['x']=23; key_symbol['|']=69; key_symbol['#']=85; key_symbol['\'']=82; key_symbol['+']=72; key_symbol['/']=75; key_symbol['3']=55; key_symbol['7']=59; key_symbol[';']=77;
    key_symbol['?']=78; key_symbol['C']=28; key_symbol['G']=32; key_symbol['K']=36; key_symbol['O']=40; key_symbol['S']=44; key_symbol['W']=48; key_symbol['[']=62; key_symbol['_']=88;
    key_symbol['c']=2; key_symbol['g']=6; key_symbol['k']=10; key_symbol['o']=14; key_symbol['s']=18; key_symbol['w']=22; key_symbol['{']=64; key_symbol['"']=81; key_symbol['&']=68;
    key_symbol['*']=74; key_symbol['.']=80; key_symbol['2']=54; key_symbol['6']=58; key_symbol[':']=76; key_symbol['>']=87; key_symbol['B']=27; key_symbol['F']=31; key_symbol['J']=35;
    key_symbol['N']=39; key_symbol['R']=43; key_symbol['V']=47; key_symbol['Z']=51; key_symbol['^']=70; key_symbol['b']=1; key_symbol['f']=5; key_symbol['j']=9; key_symbol['n']=13;
    key_symbol['r']=17; key_symbol['v']=21; key_symbol['z']=25; key_symbol['!']=79; key_symbol['%']=71; key_symbol[')']=67; key_symbol['-']=73; key_symbol['1']=53; key_symbol['5']=57;
    key_symbol['9']=61; key_symbol['=']=84; key_symbol['A']=26; key_symbol['E']=30; key_symbol['I']=34; key_symbol['M']=38; key_symbol['Q']=42; key_symbol['U']=46; key_symbol['Y']=50;
    key_symbol[']']=63; key_symbol['a']=0; key_symbol['e']=4; key_symbol['i']=8; key_symbol['m']=12; key_symbol['q']=16; key_symbol['u']=20; key_symbol['y']=24; key_symbol['}']=65;
}

void cheat::clear() {
    cheat::cache.clear();
    cheat::brackets.clear();
}
long long cheat::frequency_statistic(const std::string & a, const std::string & b) {
    int num[2][105]; memset(num, 0, sizeof num);
    const std::string ch[2] = {a, b};
    for(int i = 0; i < 2; i ++) {
        for(int j = 0; j < ch[i].length(); j ++) {
            if ((int)ch[i][j] > 255) continue;
            num[i][ch[i][j]] ++;
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

    std::string in("my is test ! &nbsp;...&nbsp;..&#39;");
    boost::regex e1("&#39;");
    std::string result = boost::regex_replace(in,e1,"\'", boost::match_default | boost::format_all);
    /*
    boost::regex reg("(\\/\\*(\\s|.)*?\\*\\/)|(\\/\\/.*)", boost::regex::icase);
    boost::regex expression("\\w+|{|}");
    boost::regex space("(\\s|\\r\\n)");
    std::string res = boost::regex_replace(std::string(buffer), reg, "");
    boost::sregex_iterator it(res.begin(), res.end(), expression);
    boost::sregex_iterator end;
    std::string temp;
    for (; it != end; ++it) {
        auto ite = cheat::key_words.find(it->str());
        if (ite != cheat::key_words.end()) {
            temp += ite->second;
        }
        else if(it->str() == "{" || it->str() == "}") {
            temp += it->str();
        }
    }
    cheat::brackets[idx] = temp;
    cheat::cache[idx] = boost::regex_replace(res, space, "");
     */
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
    const static int MIN_TEXT_LENTH = 4;
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
        auto ite = segment_1.upper_bound(std::make_pair(now.second.first, -1));
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
        auto ite2 = segment_2.upper_bound(std::make_pair(now.second.second, -1));
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
