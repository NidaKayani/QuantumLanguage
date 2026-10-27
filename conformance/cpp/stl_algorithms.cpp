#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
using namespace std;

struct Person {
    string name;
    int age;
};

int main() {
    vector<int> v = {4, 8, 15, 16, 23, 42, 8, 4};
    cout << count(v.begin(), v.end(), 8) << " "
         << count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }) << endl;
    vector<int> sq(v.size());
    transform(v.begin(), v.end(), sq.begin(), [](int x) { return x * x; });
    for (int x : sq) cout << x << " ";
    cout << endl;
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    for (int x : v) cout << x << " ";
    cout << endl;
    cout << binary_search(v.begin(), v.end(), 15) << binary_search(v.begin(), v.end(), 5) << endl;
    cout << (lower_bound(v.begin(), v.end(), 16) - v.begin()) << " "
         << (upper_bound(v.begin(), v.end(), 16) - v.begin()) << endl;
    vector<Person> people = {{"Zed", 30}, {"Amy", 25}, {"Bob", 30}, {"Cal", 20}};
    stable_sort(people.begin(), people.end(), [](const Person& a, const Person& b) { return a.age > b.age; });
    for (auto& p : people) cout << p.name << "(" << p.age << ") ";
    cout << endl;
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) { return a.name < b.name; });
    cout << people.front().name << " " << people.back().name << endl;
    auto oldest = max_element(people.begin(), people.end(),
                              [](const Person& a, const Person& b) { return a.age < b.age; });
    cout << oldest->name << endl;
    vector<int> f(5);
    fill(f.begin(), f.end(), 7);
    cout << accumulate(f.begin(), f.end(), 0) << endl;
    int a = 3, b = 9;
    swap(a, b);
    cout << a << " " << b << " " << max(a, b) << " " << min({5, 2, 8}) << endl;
    cout << all_of(v.begin(), v.end(), [](int x) { return x > 0; })
         << any_of(v.begin(), v.end(), [](int x) { return x > 40; })
         << none_of(v.begin(), v.end(), [](int x) { return x > 100; }) << endl;
    reverse(v.begin(), v.end());
    for (int x : v) cout << x << " ";
    cout << endl;
    return 0;
}
