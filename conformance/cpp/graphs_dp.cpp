#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <string>
#include <algorithm>
using namespace std;

vector<int> bfs(const vector<vector<int>>& adj, int src) {
    vector<int> dist(adj.size(), -1);
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) { dist[v] = dist[u] + 1; q.push(v); }
    }
    return dist;
}

void dfs(const vector<vector<int>>& adj, int u, vector<bool>& seen, vector<int>& order) {
    seen[u] = true;
    order.push_back(u);
    for (int v : adj[u]) if (!seen[v]) dfs(adj, v, seen, order);
}

vector<long long> dijkstra(const vector<vector<pair<int, int>>>& g, int src) {
    vector<long long> d(g.size(), LLONG_MAX);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    d[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du > d[u]) continue;
        for (auto [v, w] : g[u])
            if (d[u] + w < d[v]) { d[v] = d[u] + w; pq.push({d[v], v}); }
    }
    return d;
}

int knapsack(const vector<int>& wt, const vector<int>& val, int cap) {
    vector<int> dp(cap + 1, 0);
    for (size_t i = 0; i < wt.size(); i++)
        for (int c = cap; c >= wt[i]; c--) dp[c] = max(dp[c], dp[c - wt[i]] + val[i]);
    return dp[cap];
}

int lcs(const string& a, const string& b) {
    vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1, 0));
    for (size_t i = 1; i <= a.size(); i++)
        for (size_t j = 1; j <= b.size(); j++)
            dp[i][j] = a[i - 1] == b[j - 1] ? dp[i - 1][j - 1] + 1 : max(dp[i - 1][j], dp[i][j - 1]);
    return dp[a.size()][b.size()];
}

int lis(const vector<int>& v) {
    vector<int> tails;
    for (int x : v) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x); else *it = x;
    }
    return tails.size();
}

int main() {
    int n = 6;
    vector<vector<int>> adj(n);
    int edges[][2] = {{0, 1}, {0, 2}, {1, 3}, {2, 4}, {4, 5}, {3, 5}};
    for (auto& e : edges) { adj[e[0]].push_back(e[1]); adj[e[1]].push_back(e[0]); }
    for (int d : bfs(adj, 0)) cout << d << " ";
    cout << endl;
    vector<bool> seen(n, false);
    vector<int> order;
    dfs(adj, 0, seen, order);
    for (int u : order) cout << u << " ";
    cout << endl;
    vector<vector<pair<int, int>>> g(5);
    g[0] = {{1, 4}, {2, 1}};
    g[2] = {{1, 2}, {3, 7}};
    g[1] = {{3, 1}};
    g[3] = {{4, 3}};
    for (long long d : dijkstra(g, 0)) cout << d << " ";
    cout << endl;
    cout << knapsack({1, 3, 4, 5}, {1, 4, 5, 7}, 7) << endl;
    cout << lcs("ABCBDAB", "BDCABA") << endl;
    cout << lis({10, 9, 2, 5, 3, 7, 101, 18}) << endl;
    return 0;
}
