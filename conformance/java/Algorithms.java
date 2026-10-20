import java.util.*;

public class Algorithms {
    static Map<Integer, Long> memo = new HashMap<>();

    static long fib(int n) {
        if (n < 2) return n;
        if (memo.containsKey(n)) return memo.get(n);
        long v = fib(n - 1) + fib(n - 2);
        memo.put(n, v);
        return v;
    }

    static int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    static boolean isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; (long) i * i <= n; i++) if (n % i == 0) return false;
        return true;
    }

    static List<Integer> sieve(int n) {
        boolean[] composite = new boolean[n + 1];
        List<Integer> primes = new ArrayList<>();
        for (int i = 2; i <= n; i++) {
            if (composite[i]) continue;
            primes.add(i);
            for (long j = (long) i * i; j <= n; j += i) composite[(int) j] = true;
        }
        return primes;
    }

    static void quickSort(int[] a, int lo, int hi) {
        if (lo >= hi) return;
        int p = a[(lo + hi) >>> 1], i = lo, j = hi;
        while (i <= j) {
            while (a[i] < p) i++;
            while (a[j] > p) j--;
            if (i <= j) {
                int t = a[i];
                a[i++] = a[j];
                a[j--] = t;
            }
        }
        quickSort(a, lo, j);
        quickSort(a, i, hi);
    }

    static void mergeSort(int[] a, int[] tmp, int lo, int hi) {
        if (hi - lo < 2) return;
        int mid = (lo + hi) / 2;
        mergeSort(a, tmp, lo, mid);
        mergeSort(a, tmp, mid, hi);
        int i = lo, j = mid, k = lo;
        while (i < mid && j < hi) tmp[k++] = a[i] <= a[j] ? a[i++] : a[j++];
        while (i < mid) tmp[k++] = a[i++];
        while (j < hi) tmp[k++] = a[j++];
        System.arraycopy(tmp, lo, a, lo, hi - lo);
    }

    static int binarySearch(int[] a, int key) {
        int lo = 0, hi = a.length - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (a[mid] == key) return mid;
            if (a[mid] < key) lo = mid + 1;
            else hi = mid - 1;
        }
        return -(lo + 1);
    }

    static void permutations(String prefix, String rest, List<String> out) {
        if (rest.isEmpty()) {
            out.add(prefix);
            return;
        }
        for (int i = 0; i < rest.length(); i++)
            permutations(prefix + rest.charAt(i), rest.substring(0, i) + rest.substring(i + 1), out);
    }

    static int nQueens(int n, int row, int cols, int d1, int d2) {
        if (row == n) return 1;
        int count = 0;
        for (int c = 0; c < n; c++) {
            if ((cols >> c & 1) == 1 || (d1 >> (row + c) & 1) == 1 || (d2 >> (row - c + n) & 1) == 1) continue;
            count += nQueens(n, row + 1, cols | 1 << c, d1 | 1 << (row + c), d2 | 1 << (row - c + n));
        }
        return count;
    }

    static int lcs(String a, String b) {
        int[][] dp = new int[a.length() + 1][b.length() + 1];
        for (int i = 1; i <= a.length(); i++)
            for (int j = 1; j <= b.length(); j++)
                dp[i][j] = a.charAt(i - 1) == b.charAt(j - 1) ? dp[i - 1][j - 1] + 1 : Math.max(dp[i - 1][j], dp[i][j - 1]);
        return dp[a.length()][b.length()];
    }

    static List<Integer> bfs(Map<Integer, List<Integer>> g, int start) {
        List<Integer> order = new ArrayList<>();
        Set<Integer> seen = new HashSet<>();
        Queue<Integer> q = new ArrayDeque<>();
        q.add(start);
        seen.add(start);
        while (!q.isEmpty()) {
            int u = q.poll();
            order.add(u);
            for (int v : g.getOrDefault(u, List.of()))
                if (seen.add(v)) q.add(v);
        }
        return order;
    }

    static int[] dijkstra(int n, int[][] edges, int src) {
        List<List<int[]>> adj = new ArrayList<>();
        for (int i = 0; i < n; i++) adj.add(new ArrayList<>());
        for (int[] e : edges) adj.get(e[0]).add(new int[]{e[1], e[2]});
        int[] dist = new int[n];
        Arrays.fill(dist, Integer.MAX_VALUE);
        dist[src] = 0;
        PriorityQueue<int[]> pq = new PriorityQueue<>((x, y) -> Integer.compare(x[1], y[1]));
        pq.add(new int[]{src, 0});
        while (!pq.isEmpty()) {
            int[] cur = pq.poll();
            if (cur[1] > dist[cur[0]]) continue;
            for (int[] e : adj.get(cur[0])) {
                int nd = cur[1] + e[1];
                if (nd < dist[e[0]]) {
                    dist[e[0]] = nd;
                    pq.add(new int[]{e[0], nd});
                }
            }
        }
        return dist;
    }

    public static void main(String[] args) {
        System.out.println(fib(50) + " " + fib(90) + " " + gcd(1071, 462) + " " + isPrime(97) + " " + isPrime(1_000_000_007));
        List<Integer> primes = sieve(100000);
        System.out.println(primes.size() + " " + primes.get(primes.size() - 1) + " " + primes.subList(0, 10));
        int[] data = new int[2000];
        long seed = 12345;
        for (int i = 0; i < data.length; i++) {
            seed = (seed * 1103515245 + 12345) % 2147483648L;
            data[i] = (int) (seed % 10000);
        }
        int[] q = data.clone(), m = data.clone();
        quickSort(q, 0, q.length - 1);
        mergeSort(m, new int[m.length], 0, m.length);
        int[] ref = data.clone();
        Arrays.sort(ref);
        System.out.println(Arrays.equals(q, ref) + " " + Arrays.equals(m, ref) + " " + ref[0] + " " + ref[1999] + " " + binarySearch(ref, ref[700]));
        List<String> perms = new ArrayList<>();
        permutations("", "abcd", perms);
        System.out.println(perms.size() + " " + perms.subList(0, 6));
        System.out.println(nQueens(8, 0, 0, 0, 0) + " " + lcs("AGGTAB", "GXTXAYB"));
        Map<Integer, List<Integer>> g = new HashMap<>();
        g.put(1, List.of(2, 3));
        g.put(2, List.of(4));
        g.put(3, List.of(4, 5));
        g.put(4, List.of(6));
        System.out.println(bfs(g, 1));
        int[][] edges = {{0, 1, 4}, {0, 2, 1}, {2, 1, 2}, {1, 3, 1}, {2, 3, 5}, {3, 4, 3}};
        System.out.println(Arrays.toString(dijkstra(5, edges, 0)));
        long count = 0;
        for (int i = 0; i < 2000; i++) for (int j = i; j < 2000; j++) if ((i ^ j) % 3 == 0) count++;
        System.out.println(count);
    }
}
