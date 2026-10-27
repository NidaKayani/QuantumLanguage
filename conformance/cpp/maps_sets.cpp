#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

int main() {
    map<string, int> ages;
    ages["alice"] = 30;
    ages["bob"] = 25;
    ages.insert({"carol", 35});
    ages["alice"] += 1;
    for (auto& p : ages) cout << p.first << "=" << p.second << endl;
    cout << ages.size() << " " << ages.count("bob") << " " << ages.count("dave") << endl;
    ages.erase("bob");
    if (ages.find("bob") == ages.end()) cout << "bob gone" << endl;
    map<int, vector<string>> byLen;
    for (string w : {"hi", "hey", "yo", "hello", "abc"}) byLen[w.size()].push_back(w);
    for (auto& [len, ws] : byLen) {
        cout << len << ":";
        for (auto& w : ws) cout << " " << w;
        cout << endl;
    }
    set<int> s = {5, 1, 3, 1, 5, 9};
    s.insert(4);
    for (int x : s) cout << x << " ";
    cout << endl;
    cout << s.size() << " " << *s.begin() << " " << *s.rbegin() << " " << s.count(3) << endl;
    auto lb = s.lower_bound(4), ub = s.upper_bound(4);
    cout << *lb << " " << *ub << endl;
    multiset<int> ms = {2, 2, 1, 3, 2};
    cout << ms.count(2) << " " << ms.size() << endl;
    map<char, int> freq;
    for (char c : string("mississippi")) freq[c]++;
    for (auto& [c, n] : freq) cout << c << n;
    cout << endl;
    return 0;
}
