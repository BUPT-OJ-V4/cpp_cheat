//
// Created by liuwei on 10/21/17.
//

#ifndef CHEAT_COMMON_H
#define CHEAT_COMMON_H
#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>
#include <queue>

typedef std::pair<int, int> P;
typedef std::shared_ptr<std::queue<P>> QueuePtr;

template <typename K, typename V>
using map_pair = typename std::map<K, V>::value_type;
template <typename K, typename V>
using map_ite = typename std::map<K, V>::iterator;
template <typename K, typename V>
using pair_ite=typename std::set<typename std::pair<K, V>>::iterator;

struct Result{
    double ans;
    int sub1;
    int sub2;
    std::string user1;
    std::string user2;
    Result() {}
    Result(const double& a, const int& s1, const int& s2, const std::string& u1, const std::string& u2)
            : ans(a), sub1(s1), sub2(s2), user1(u1), user2(u2)
    {}
    bool operator < (const Result& p) const {
        if (ans + 1e-6 < p.ans) {
            return false;
        }
        if (ans > p.ans + 1e-6) {
            return true;
        }
        return sub1 > p.sub1;
    }
};

#endif //CHEAT_COMMON_H
