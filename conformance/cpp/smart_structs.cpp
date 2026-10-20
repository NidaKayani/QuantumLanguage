#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <sstream>
using namespace std;

struct Student {
    string name;
    int grade;
    double gpa;
};

enum class Color { Red, Green, Blue };

string colorName(Color c) {
    switch (c) {
        case Color::Red: return "red";
        case Color::Green: return "green";
        default: return "blue";
    }
}

tuple<int, int, int> minMaxSum(const vector<int>& v) {
    int mn = v[0], mx = v[0], s = 0;
    for (int x : v) { mn = min(mn, x); mx = max(mx, x); s += x; }
    return {mn, mx, s};
}

class Resource {
    string id;
public:
    Resource(string i) : id(i) { cout << "acquire " << id << endl; }
    ~Resource() { cout << "release " << id << endl; }
    void use() const { cout << "use " << id << endl; }
};

int main() {
    vector<Student> cls = {{"Ann", 10, 3.8}, {"Ben", 11, 3.2}};
    cls.push_back({"Cid", 10, 3.9});
    for (const auto& s : cls) cout << s.name << " " << s.grade << " " << s.gpa << endl;
    auto [mn, mx, sum] = minMaxSum({4, -2, 9, 1});
    cout << mn << " " << mx << " " << sum << endl;
    int a, b;
    tie(a, b) = make_pair(1, 2);
    cout << a + b << endl;
    cout << colorName(Color::Green) << " " << colorName(Color::Blue) << endl;
    {
        auto r = make_unique<Resource>("db");
        r->use();
        shared_ptr<Resource> s1 = make_shared<Resource>("cache");
        {
            shared_ptr<Resource> s2 = s1;
            cout << "count " << s1.use_count() << endl;
        }
        cout << "count " << s1.use_count() << endl;
    }
    stringstream ss("10 20 30");
    int x, total = 0;
    while (ss >> x) total += x;
    cout << total << endl;
    ostringstream os;
    os << "pi~" << 3.14 << "|" << 42;
    cout << os.str() << endl;
    map<string, pair<int, int>> scores = {{"x", {1, 2}}, {"y", {3, 4}}};
    for (const auto& [k, v] : scores) cout << k << ":" << v.first + v.second << " ";
    cout << endl;
    return 0;
}
