import java.util.Arrays;

public class Arrays2D {
    static int[][] multiply(int[][] a, int[][] b) {
        int n = a.length, m = b[0].length, k = b.length;
        int[][] c = new int[n][m];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                for (int t = 0; t < k; t++) c[i][j] += a[i][t] * b[t][j];
        return c;
    }

    static int sum(int... nums) {
        int s = 0;
        for (int v : nums) s += v;
        return s;
    }

    public static void main(String[] args) {
        int[] a = {5, 3, 9, 1, 7};
        System.out.println(a.length + " " + a[0] + " " + a[a.length - 1] + " " + Arrays.toString(a));
        Arrays.sort(a);
        System.out.println(Arrays.toString(a) + " " + Arrays.binarySearch(a, 7));
        int[] copy = Arrays.copyOf(a, 7);
        int[] range = Arrays.copyOfRange(a, 1, 3);
        System.out.println(Arrays.toString(copy) + " " + Arrays.toString(range));
        double[] d = new double[3];
        boolean[] flags = new boolean[2];
        String[] names = new String[2];
        char[] cs = new char[2];
        System.out.println(Arrays.toString(d) + " " + Arrays.toString(flags) + " " + Arrays.toString(names) + " " + (int) cs[0]);
        Arrays.fill(d, 1.5);
        System.out.println(Arrays.toString(d) + " " + Arrays.stream(a).sum() + " " + Arrays.stream(a).max().getAsInt());
        int[][] grid = {{1, 2, 3}, {4, 5, 6}};
        int[][] id = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
        System.out.println(Arrays.deepToString(multiply(grid, id)));
        int[][] jag = new int[3][];
        for (int i = 0; i < jag.length; i++) {
            jag[i] = new int[i + 1];
            Arrays.fill(jag[i], i);
        }
        System.out.println(Arrays.deepToString(jag));
        int[][][] cube = new int[2][2][2];
        cube[1][1][1] = 8;
        System.out.println(cube[1][1][1] + cube[0][0][0] + " " + cube.length);
        String[] words = {"pear", "apple", "fig"};
        Arrays.sort(words);
        System.out.println(String.join(",", words) + " " + Arrays.asList(words).indexOf("fig"));
        Integer[] boxed = {3, 1, 2};
        Arrays.sort(boxed, (x, y) -> y - x);
        System.out.println(Arrays.toString(boxed));
        System.out.println(sum() + " " + sum(1) + " " + sum(1, 2, 3) + " " + sum(new int[]{4, 5}));
        int[] b = a.clone();
        b[0] = 100;
        System.out.println(a[0] + " " + b[0] + " " + Arrays.equals(a, b) + " " + Arrays.equals(a, a.clone()));
        try {
            int[] small = new int[2];
            small[2] = 1;
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("caught: " + e.getMessage());
        }
        long[] longs = new long[3];
        longs[1] = 1L << 50;
        longs[2] = longs[1] * 3;
        System.out.println(Arrays.toString(longs));
        char[][] board = new char[3][3];
        for (char[] row : board) Arrays.fill(row, '.');
        board[1][1] = 'X';
        for (char[] row : board) System.out.println(new String(row));
        int[] counts = new int[26];
        for (char ch : "banana".toCharArray()) counts[ch - 'a']++;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 26; i++) if (counts[i] > 0) sb.append((char) ('a' + i)).append(counts[i]);
        System.out.println(sb);
    }
}
