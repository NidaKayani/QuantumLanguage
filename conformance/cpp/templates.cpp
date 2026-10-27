#include <iostream>
#include <vector>
#include <string>
using namespace std;

template <typename T>
T maxOf(T a, T b) { return a > b ? a : b; }

template <typename T>
class Stack {
    vector<T> items;
public:
    void push(const T& x) { items.push_back(x); }
    T pop() { T x = items.back(); items.pop_back(); return x; }
    bool empty() const { return items.empty(); }
    int size() const { return items.size(); }
};

template <typename T, typename U>
struct Pair {
    T first;
    U second;
    void show() const { cout << "<" << first << ", " << second << ">" << endl; }
};

template <typename T>
T sumAll(const vector<T>& v) {
    T total = T();
    for (const T& x : v) total += x;
    return total;
}

int main() {
    cout << maxOf(3, 7) << " " << maxOf(2.5, 1.5) << " " << maxOf<string>("apple", "pear") << endl;
    Stack<int> s;
    for (int i = 1; i <= 4; i++) s.push(i * 10);
    cout << s.size() << ":";
    while (!s.empty()) cout << " " << s.pop();
    cout << endl;
    Stack<string> ss;
    ss.push("a"); ss.push("b");
    cout << ss.pop() << ss.pop() << endl;
    Pair<string, int> p{"age", 42};
    p.show();
    cout << sumAll(vector<int>{1, 2, 3}) << " " << sumAll(vector<double>{0.5, 0.25}) << " "
         << sumAll(vector<string>{"x", "y", "z"}) << endl;
    return 0;
}
