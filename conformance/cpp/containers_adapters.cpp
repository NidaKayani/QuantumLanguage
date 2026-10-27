#include <iostream>
#include <stack>
#include <queue>
#include <deque>
#include <vector>
#include <string>
#include <utility>
using namespace std;

bool balanced(const string& s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') st.push(c);
        else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char o = st.top();
            st.pop();
            if ((c == ')' && o != '(') || (c == ']' && o != '[') || (c == '}' && o != '{')) return false;
        }
    }
    return st.empty();
}

int main() {
    stack<int> s;
    for (int i = 0; i < 5; i++) s.push(i * i);
    cout << s.size() << " top=" << s.top() << endl;
    while (!s.empty()) { cout << s.top() << " "; s.pop(); }
    cout << endl;
    queue<string> q;
    q.push("a"); q.push("b"); q.push("c");
    cout << q.front() << q.back() << q.size() << endl;
    q.pop();
    cout << q.front() << endl;
    deque<int> d = {3, 4};
    d.push_front(2); d.push_back(5); d.push_front(1);
    for (int x : d) cout << x;
    cout << " " << d.front() << d.back() << d[2] << endl;
    d.pop_front(); d.pop_back();
    cout << d.size() << endl;
    priority_queue<int> maxh;
    priority_queue<int, vector<int>, greater<int>> minh;
    for (int x : {5, 1, 8, 3, 9, 2}) { maxh.push(x); minh.push(x); }
    while (!maxh.empty()) { cout << maxh.top(); maxh.pop(); }
    cout << " ";
    while (!minh.empty()) { cout << minh.top(); minh.pop(); }
    cout << endl;
    priority_queue<pair<int, string>> tasks;
    tasks.push({2, "write"}); tasks.push({5, "test"}); tasks.push({1, "rest"});
    while (!tasks.empty()) { cout << tasks.top().second << ":" << tasks.top().first << " "; tasks.pop(); }
    cout << endl;
    pair<int, double> p = make_pair(3, 1.5);
    cout << p.first << " " << p.second << endl;
    cout << balanced("{[()()]}") << balanced("([)]") << balanced("((") << endl;
    return 0;
}
