public class Numbers {
    static long fib(int n) {
        long a = 0, b = 1;
        for (int i = 0; i < n; i++) {
            long t = a + b;
            a = b;
            b = t;
        }
        return a;
    }

    public static void main(String[] args) {
        int a = 17, b = 5;
        System.out.println(a / b + " " + a % b + " " + (-a) / b + " " + (-a) % b + " " + a % -b);
        int max = Integer.MAX_VALUE;
        System.out.println(max + 1);
        System.out.println(Integer.MIN_VALUE - 1);
        System.out.println(max * 2);
        System.out.println(46341 * 46341);
        long big = Long.MAX_VALUE;
        System.out.println(big + " " + (big + 1) + " " + Long.MIN_VALUE);
        System.out.println(fib(90) + " " + fib(92));
        long mod = 1_000_000_007L;
        long x = 999_999_999L, y = 888_888_888L;
        System.out.println(x * y % mod);
        System.out.println((x * y) / 7 + " " + Long.MAX_VALUE / 3);
        long h = 0;
        for (char c : "polynomial rolling hash".toCharArray()) h = (h * 131 + c) % mod;
        System.out.println(h);
        double d = 10 / 4;
        double e = 10 / 4.0;
        System.out.println(d + " " + e + " " + 10.0 / 3 + " " + 1.0 / 0 + " " + -1.0 / 0 + " " + 0.0 / 0);
        System.out.println(0.1 + 0.2);
        System.out.println(100.0 + " " + 1e7 + " " + 1.0E-5 + " " + 123456789.0 + " " + 0.001 + " " + 1234567.0);
        System.out.println((float) 0.1 + " " + 1.1f + " " + (0.1f + 0.2f) + " " + 3.0f / 7 + " " + 1e10f);
        System.out.println((int) 3.99 + " " + (int) -3.99 + " " + (int) 1e20 + " " + (long) -1e30 + " " + (int) Double.NaN);
        System.out.println(Math.round(2.5) + " " + Math.round(-2.5) + " " + Math.round(2.4f) + " " + Math.floor(-1.5) + " " + Math.ceil(-1.5));
        System.out.println(Math.abs(-7) + " " + Math.max(3, 9) + " " + Math.min(2.5, 1.5) + " " + Math.pow(2, 10) + " " + Math.sqrt(2));
        System.out.println(Math.floorDiv(-7, 2) + " " + Math.floorMod(-7, 2) + " " + Math.hypot(3, 4) + " " + Math.cbrt(27));
        char c = 'A';
        c += 2;
        c++;
        System.out.println(c + " " + (int) c + " " + (c + 1) + " " + (char) (c + 1) + " " + (char) 97);
        System.out.println('a' + 'b' + " " + "" + 'a' + 'b' + " " + (char) ('a' + 1));
        byte by = (byte) 200;
        short sh = (short) 70000;
        System.out.println(by + " " + sh + " " + (byte) -129 + " " + (char) 65 + (char) 66);
        byte acc = 10;
        acc += 300;
        int iv = 5;
        iv *= 2.5;
        iv -= 0.5;
        System.out.println(acc + " " + iv);
        int i = 5;
        int j = i++ + ++i;
        int k = i-- - --i;
        System.out.println(i + " " + j + " " + k);
        System.out.println((7 & 3) + " " + (7 | 8) + " " + (7 ^ 2) + " " + (~7) + " " + (1 << 31) + " " + (1 << 33) + " " + (1L << 40));
        System.out.println((-16 >> 2) + " " + (-16 >>> 28) + " " + (-1 >>> 1) + " " + (-1L >>> 1) + " " + (8 >> 1));
        System.out.println(Integer.toBinaryString(10) + " " + Integer.toHexString(255) + " " + Integer.toHexString(-1) + " " + Integer.toOctalString(8) + " " + Long.toBinaryString(5L));
        System.out.println(Integer.parseInt("-123") + 1 + " " + Long.parseLong("9000000000") + " " + Double.parseDouble("2.5") * 2 + " " + Integer.valueOf(42));
        System.out.println(Integer.MAX_VALUE + Integer.MAX_VALUE + " " + (long) Integer.MAX_VALUE + Integer.MAX_VALUE);
        System.out.println(5 / 2 * 2.0 + " " + 5 / 2.0 * 2 + " " + (1 + 2) + "3" + 1 + 2);
        double small = 1.0 / 3;
        float f = 1.0f / 3;
        System.out.println(small + " " + f + " " + (double) f + " " + (float) small);
        System.out.println(Double.MAX_VALUE + " " + Double.MIN_VALUE + " " + Float.MAX_VALUE + " " + Long.MIN_VALUE / -1);
        System.out.println(Integer.compare(3, 7) + " " + Integer.signum(-9) + " " + Integer.bitCount(255) + " " + Character.getNumericValue('7'));
        System.out.println(0x1F + " " + 0b1011 + " " + 017 + " " + 1_000_000 + " " + 0xFFFFFFFF + " " + 0x7fffffffffffffffL);
    }
}
