import java.util.*;

public class Recursion {
    static Map<Integer, Long> memo = new HashMap<>();

    static long fib(int n) {
        if (n < 2) return n;
        Long cached = memo.get(n);
        if (cached != null) return cached;
        long r = fib(n - 1) + fib(n - 2);
        memo.put(n, r);
        return r;
    }

    static void permute(String prefix, String rest, List<String> out) {
        if (rest.isEmpty()) {
            out.add(prefix);
            return;
        }
        for (int i = 0; i < rest.length(); i++)
            permute(prefix + rest.charAt(i), rest.substring(0, i) + rest.substring(i + 1), out);
    }

    static int queens(int n, int row, int[] cols) {
        if (row == n) return 1;
        int total = 0;
        for (int c = 0; c < n; c++) {
            boolean ok = true;
            for (int r = 0; r < row && ok; r++)
                if (cols[r] == c || Math.abs(cols[r] - c) == row - r) ok = false;
            if (ok) {
                cols[row] = c;
                total += queens(n, row + 1, cols);
            }
        }
        return total;
    }

    static int hanoi(int n, char from, char to, char via, StringBuilder log) {
        if (n == 0) return 0;
        int moves = hanoi(n - 1, from, via, to, log);
        if (n <= 2) log.append(from).append("->").append(to).append(' ');
        moves++;
        return moves + hanoi(n - 1, via, to, from, log);
    }

    static int ackermann(int m, int n) {
        if (m == 0) return n + 1;
        if (n == 0) return ackermann(m - 1, 1);
        return ackermann(m - 1, ackermann(m, n - 1));
    }

    static int fill(char[][] g, int r, int c) {
        if (r < 0 || c < 0 || r >= g.length || c >= g[0].length || g[r][c] != '.') return 0;
        g[r][c] = '#';
        return 1 + fill(g, r + 1, c) + fill(g, r - 1, c) + fill(g, r, c + 1) + fill(g, r, c - 1);
    }

    static int bsearch(int[] a, int key, int lo, int hi) {
        if (lo > hi) return -1;
        int mid = (lo + hi) >>> 1;
        if (a[mid] == key) return mid;
        return a[mid] < key ? bsearch(a, key, mid + 1, hi) : bsearch(a, key, lo, mid - 1);
    }

    static long gcd(long a, long b) { return b == 0 ? a : gcd(b, a % b); }

    static List<List<Integer>> subsets(int[] items, int i) {
        if (i == items.length) {
            List<List<Integer>> base = new ArrayList<>();
            base.add(new ArrayList<>());
            return base;
        }
        List<List<Integer>> rest = subsets(items, i + 1);
        List<List<Integer>> out = new ArrayList<>();
        for (List<Integer> s : rest) {
            out.add(s);
            List<Integer> with = new ArrayList<>(s);
            with.add(0, items[i]);
            out.add(with);
        }
        return out;
    }

    static String toBinary(int n) { return n < 2 ? String.valueOf(n) : toBinary(n / 2) + (n % 2); }

    public static void main(String[] args) {
        System.out.println(fib(90) + " " + memo.size());
        List<String> perms = new ArrayList<>();
        permute("", "abc", perms);
        System.out.println(perms);
        for (int n = 4; n <= 8; n++)
            System.out.print(queens(n, 0, new int[n]) + (n < 8 ? " " : "\n"));
        StringBuilder log = new StringBuilder();
        System.out.println(hanoi(10, 'A', 'C', 'B', log) + " moves; " + log.length());
        System.out.println(ackermann(2, 3) + " " + ackermann(3, 3));
        char[][] grid = {
            "..#..".toCharArray(),
            ".##..".toCharArray(),
            "#...#".toCharArray(),
            "..#..".toCharArray(),
        };
        System.out.println(fill(grid, 0, 0) + " " + fill(grid, 0, 4) + " " + new String(grid[2]));
        int[] sorted = {1, 3, 5, 7, 9, 11, 13};
        System.out.println(bsearch(sorted, 11, 0, sorted.length - 1) + " " + bsearch(sorted, 4, 0, sorted.length - 1));
        System.out.println(gcd(1071, 462) + " " + (1071L * 462 / gcd(1071, 462)));
        System.out.println(subsets(new int[] {1, 2, 3}, 0));
        System.out.println(toBinary(37) + " " + toBinary(1024));
    }
}
