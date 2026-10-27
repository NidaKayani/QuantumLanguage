import java.util.*;

public class Simulation {
    static char[][] step(char[][] g) {
        int r = g.length, c = g[0].length;
        char[][] next = new char[r][c];
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                int n = 0;
                for (int di = -1; di <= 1; di++)
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0) continue;
                        int a = (i + di + r) % r, b = (j + dj + c) % c;
                        if (g[a][b] == '#') n++;
                    }
                next[i][j] = (g[i][j] == '#' && (n == 2 || n == 3)) || (g[i][j] != '#' && n == 3) ? '#' : '.';
            }
        return next;
    }

    static class Account {
        private static int nextId = 1;
        private final int id;
        private final String owner;
        private double balance;
        private final List<String> history = new ArrayList<>();

        Account(String owner, double initial) {
            this.id = nextId++;
            this.owner = owner;
            this.balance = initial;
            history.add(String.format("open %.2f", initial));
        }

        void deposit(double amt) {
            if (amt <= 0) throw new IllegalArgumentException("Deposit must be positive");
            balance += amt;
            history.add(String.format("dep %.2f", amt));
        }

        boolean withdraw(double amt) {
            if (amt > balance) {
                history.add(String.format("denied %.2f", amt));
                return false;
            }
            balance -= amt;
            history.add(String.format("wd %.2f", amt));
            return true;
        }

        @Override
        public String toString() { return String.format("#%03d %-6s %10.2f", id, owner, balance); }
    }

    public static void main(String[] args) {
        char[][] g = {
            "......".toCharArray(),
            "..#...".toCharArray(),
            "...#..".toCharArray(),
            ".###..".toCharArray(),
            "......".toCharArray(),
        };
        for (int gen = 0; gen < 4; gen++) g = step(g);
        for (char[] row : g) System.out.println(new String(row));
        int[][] m = new int[4][];
        for (int i = 0; i < m.length; i++) {
            m[i] = new int[i + 1];
            m[i][0] = m[i][i] = 1;
            for (int j = 1; j < i; j++) m[i][j] = m[i - 1][j - 1] + m[i - 1][j];
        }
        System.out.println(Arrays.deepToString(m));
        int[][] a = {{1, 2}, {3, 4}}, b = {{5, 6}, {7, 8}};
        int[][] p = new int[2][2];
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++) p[i][j] += a[i][k] * b[k][j];
        System.out.println(Arrays.deepToString(p) + " " + Arrays.toString(p[1]));
        int[][] copy = p.clone();
        copy[0][0] = -1;
        System.out.println(p[0][0] + " " + (copy[0] == p[0]));
        List<Account> accts = new ArrayList<>();
        accts.add(new Account("Ann", 100));
        accts.add(new Account("Bob", 50.5));
        accts.get(0).deposit(25.25);
        System.out.println(accts.get(1).withdraw(70) + " " + accts.get(0).withdraw(20));
        try {
            accts.get(1).deposit(-5);
        } catch (IllegalArgumentException e) {
            System.out.println("Error: " + e.getMessage());
        }
        accts.forEach(System.out::println);
        System.out.println(accts.get(0).history + " " + accts.get(1).history);
        double total = accts.stream().mapToDouble(x -> x.balance).sum();
        System.out.printf("Total: %.2f across %d accounts%n", total, accts.size());
        StringBuilder bar = new StringBuilder();
        int[] data = {3, 7, 1, 5};
        for (int v : data) bar.append(String.format("%-2d|%s%n", v, "*".repeat(v)));
        System.out.print(bar);
    }
}
