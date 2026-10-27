#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) cin >> nums[i];
    long long sum = 0;
    int mx = nums[0], mn = nums[0];
    for (int x : nums) {
        sum += x;
        mx = max(mx, x);
        mn = min(mn, x);
    }
    cout << "sum=" << sum << " max=" << mx << " min=" << mn << endl;
    cout << fixed << setprecision(2) << "avg=" << (double)sum / n << endl;
    string name;
    cin >> name;
    cin.ignore();
    string line;
    getline(cin, line);
    cout << "Hello, " << name << "! You said: " << line << endl;
    double d;
    char c;
    cin >> d >> c;
    cout << d * 2 << " " << c << endl;
    return 0;
}
