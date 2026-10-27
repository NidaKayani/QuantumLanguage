#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

void print(const vector<int>& v) {
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << (i + 1 < v.size() ? " " : "\n");
}

int main() {
    vector<int> v = {5, 2, 9, 1, 7};
    v.push_back(3);
    print(v);
    sort(v.begin(), v.end());
    print(v);
    sort(v.rbegin(), v.rend());
    print(v);
    v.pop_back();
    v.insert(v.begin() + 1, 42);
    v.erase(v.begin());
    print(v);
    cout << accumulate(v.begin(), v.end(), 0) << " " << *max_element(v.begin(), v.end())
         << " " << *min_element(v.begin(), v.end()) << endl;
    auto it = find(v.begin(), v.end(), 7);
    cout << (it != v.end() ? "found at " + to_string(it - v.begin()) : "missing") << endl;
    vector<vector<int>> grid(3, vector<int>(4, 0));
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++) grid[i][j] = i * j;
    for (auto& row : grid) print(row);
    vector<int> w(5);
    iota(w.begin(), w.end(), 1);
    for (int& x : w) x *= x;
    print(w);
    w.resize(7, -1);
    print(w);
    cout << w.size() << " " << w.front() << " " << w.back() << " " << w.empty() << endl;
    vector<string> names = {"bob", "alice", "carol"};
    sort(names.begin(), names.end());
    for (const auto& n : names) cout << n << ";";
    cout << endl;
    reverse(names.begin(), names.end());
    cout << names[0] << endl;
    vector<int> c(v);
    c.clear();
    cout << c.size() << " " << v.size() << endl;
    return 0;
}
