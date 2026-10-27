#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;

int applyFn(const function<int(int)>& f, int x) { return f(x); }

function<int(int)> makeAdder(int n) {
    return [n](int x) { return x + n; };
}

int main() {
    auto square = [](int x) { return x * x; };
    cout << square(7) << " " << applyFn(square, 3) << endl;
    int total = 0;
    auto addTo = [&total](int x) { total += x; };
    for (int i = 1; i <= 10; i++) addTo(i);
    cout << total << endl;
    int base = 100;
    auto byValue = [base]() { return base + 1; };
    base = 0;
    cout << byValue() << " " << base << endl;
    auto add5 = makeAdder(5);
    cout << add5(10) << " " << makeAdder(-3)(3) << endl;
    function<long long(int)> fact = [&](int n) -> long long { return n <= 1 ? 1 : n * fact(n - 1); };
    cout << fact(15) << endl;
    vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
    sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    for_each(v.begin(), v.end(), [](int x) { cout << x; });
    cout << endl;
    auto generic = [](auto a, auto b) { return a + b; };
    cout << generic(1, 2) << " " << generic(1.5, 2.25) << " " << generic(string("a"), string("b")) << endl;
    int counter = 0;
    auto incr = [&]() mutable { return ++counter; };
    incr(); incr();
    cout << counter << endl;
    vector<function<int(int)>> ops = {[](int x) { return x + 1; }, [](int x) { return x * 2; }};
    int val = 5;
    for (auto& op : ops) val = op(val);
    cout << val << endl;
    return 0;
}
