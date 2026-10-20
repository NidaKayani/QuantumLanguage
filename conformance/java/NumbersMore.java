public class NumbersMore {
    public static void main(String[] args) {
        int max = Integer.MAX_VALUE;
        System.out.println((max + 1) + " " + (Integer.MIN_VALUE - 1) + " " + (max * 2) + " " + Math.abs(Integer.MIN_VALUE) + " " + (-Integer.MIN_VALUE));
        long big = Long.MAX_VALUE;
        System.out.println((big + 1) + " " + (1L << 62) + " " + (1 << 33) + " " + (1L << 65) + " " + (-1 >>> 28) + " " + (-1L >>> 60) + " " + (-17 >> 2) + " " + (-17 >>> 28));
        System.out.println((7 / 2) + " " + (-7 / 2) + " " + (7 % 3) + " " + (-7 % 3) + " " + (7 % -3) + " " + Math.floorDiv(-7, 2) + " " + Math.floorMod(-7, 3) + " " + Math.floorMod(7, -3));
        System.out.println((0.1 + 0.2) + " " + (1.0 / 3) + " " + 100.0 + " " + 1e10 + " " + 1e-5 + " " + 123456789.0 + " " + 1234567.0 + " " + 0.001 + " " + (2.0 / 0) + " " + (0.0 / 0) + " " + (-1.0 / 0));
        System.out.println(1.1f + " " + (1.1f * 3) + " " + (float) 0.1 + " " + 3.0f + " " + (double) 1.1f + " " + 16777217f + " " + 1e20f);
        System.out.println((int) 3.99 + " " + (int) -3.99 + " " + (int) Double.NaN + " " + (int) 1e20 + " " + (long) -1e30 + " " + (int) 'A' + " " + (char) 66 + " " + (byte) 200 + " " + (short) 70000 + " " + (byte) -129);
        byte b = 10;
        b += 300;
        short s = 1000;
        s *= 100;
        char c = 'a';
        c++;
        int i = 'z' - c;
        System.out.println(b + " " + s + " " + c + " " + i);
        System.out.println(Integer.toBinaryString(10) + " " + Integer.toBinaryString(-1).length() + " " + Integer.toHexString(-1) + " " + Integer.toOctalString(64) + " " + Long.toHexString(255L << 32) + " " + Integer.toString(-255, 2));
        System.out.println(Integer.parseInt("ff", 16) + " " + Integer.parseInt("-1010", 2) + " " + Long.parseLong("9000000000") + " " + Integer.valueOf("077") + " " + Integer.decode("0x1F"));
        System.out.println(Math.round(2.5) + " " + Math.round(-2.5) + " " + Math.round(2.4f) + " " + Math.ceil(-0.5) + " " + Math.floor(-0.5) + " " + Math.rint(2.5) + " " + Math.rint(3.5));
        System.out.println(0x1F + " " + 0b1011 + " " + 017 + " " + 1_000_000 + " " + 0xFFFF_FFFFL + " " + 'A' + 1 + " " + (char) ('A' + 1));
        System.out.println(Integer.bitCount(255) + " " + Integer.highestOneBit(100) + " " + Integer.numberOfTrailingZeros(48) + " " + Integer.reverse(1) + " " + Long.numberOfLeadingZeros(1L) + " " + Integer.signum(-9));
        System.out.println(Math.sqrt(2) + " " + Math.pow(2, 10) + " " + Math.pow(2, 0.5) + " " + Math.cbrt(27) + " " + Math.hypot(3, 4) + " " + Math.exp(1) + " " + Math.log(Math.E) + " " + Math.log10(1000));
        System.out.println(Math.sin(Math.PI / 6) + " " + Math.cos(0) + " " + Math.toDegrees(Math.PI) + " " + Math.atan2(1, 1) + " " + Math.max(-0.0, 0.0) + " " + Math.min(-0.0, 0.0) + " " + Math.signum(-3.2));
        System.out.println(Double.compare(0.0, -0.0) + " " + (0.0 == -0.0) + " " + Double.valueOf(0.0).equals(-0.0) + " " + Double.isNaN(0.0 / 0) + " " + (Double.NaN == Double.NaN) + " " + Double.MIN_VALUE + " " + Double.MAX_VALUE + " " + Float.MAX_VALUE);
        long fact = 1;
        for (int k = 1; k <= 25; k++) fact *= k;
        System.out.println(fact + " " + Long.MIN_VALUE + " " + Math.multiplyHigh(1L << 40, 1L << 40));
        int x = 5;
        x += x++ + ++x;
        int y = 10;
        y = y++ + y--;
        System.out.println(x + " " + y + " " + (5 & 3) + " " + (5 | 3) + " " + (5 ^ 3) + " " + (~5) + " " + (true ^ true) + " " + (false | true));
        double d = 10;
        d /= 4;
        int n = 10;
        n /= 4;
        n -= 2.7;
        System.out.println(d + " " + n + " " + (1 + 2 * 3 - 4 / 2 % 3) + " " + (10 - 2 - 3) + " " + (2 << 1 + 1));
        System.out.println(String.format("%.2f %.3f %.1f %.0f %.4f", 1.005, 2.0005, 0.05, 0.5, Math.PI) + " " + String.format("%08.3f|%+d|%(d|% d", -3.14159, 5, -5, 5));
    }
}
