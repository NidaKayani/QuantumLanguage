#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

long long factorial(int n) { return n <= 1 ? 1 : n * factorial(n - 1); }
long long memo[100];
long long fib(int n) {
    if (n < 2) return n;
    if (memo[n]) return memo[n];
    return memo[n] = fib(n - 1) + fib(n - 2);
}
void hanoi(int n, char from, char to, char via, int& moves) {
    if (n == 0) return;
    hanoi(n - 1, from, via, to, moves);
    moves++;
    if (n == 3) cout << "move disk 3 " << from << "->" << to << endl;
    hanoi(n - 1, via, to, from, moves);
}
int queens(int row, int n, vector<int>& cols) {
    if (row == n) return 1;
    int count = 0;
    for (int c = 0; c < n; c++) {
        bool ok = true;
        for (int r = 0; r < row; r++)
            if (cols[r] == c || abs(cols[r] - c) == row - r) { ok = false; break; }
        if (ok) { cols[row] = c; count += queens(row + 1, n, cols); }
    }
    return count;
}
void permute(string s, int l, vector<string>& out) {
    if (l == (int)s.size()) { out.push_back(s); return; }
    for (int i = l; i < (int)s.size(); i++) {
        swap(s[l], s[i]);
        permute(s, l + 1, out);
        swap(s[l], s[i]);
    }
}
int main() {
    cout << factorial(10) << " " << factorial(20) << endl;
    cout << fib(50) << " " << fib(90) << endl;
    int moves = 0;
    hanoi(4, 'A', 'C', 'B', moves);
    cout << moves << " moves" << endl;
    vector<int> cols(8);
    cout << queens(0, 8, cols) << " solutions" << endl;
    vector<string> perms;
    permute("abc", 0, perms);
    sort(perms.begin(), perms.end());
    for (auto& p : perms) cout << p << " ";
    cout << endl;
    string w = "cab";
    sort(w.begin(), w.end());
    do cout << w << ","; while (next_permutation(w.begin(), w.end()));
    cout << endl;
    return 0;
}
