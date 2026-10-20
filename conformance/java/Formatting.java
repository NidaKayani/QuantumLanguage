public class Formatting {
    public static void main(String[] args) {
        System.out.printf("%d|%5d|%-5d|%05d|%+d|%,d|%x|%X|%o%n", 42, 42, 42, 42, 42, 1234567, 255, 255, 8);
        System.out.printf("%.2f|%8.3f|%-8.1f|%e|%.3e|%10.2e%n", 3.14159, 2.71828, 1.5, 12345.678, 0.000123, 9.99);
        System.out.printf("%.2f %.2f %.1f %.1f %.0f %.0f %.3f%n", 0.125, 1.005, 0.25, 0.15, 2.5, 3.5, 2.0005);
        System.out.printf("%s|%10s|%-10s|%.3s|%c|%b|%b|%%%n", "str", "right", "left", "truncate", 'Z', true, null);
        System.out.printf("%s %s %s %s%n", 1.0, 100, 'c', new int[0].length);
        System.out.printf("%08.2f|%,.2f|%(d|% d%n", 3.14159, 1234567.891, -42, 42);
        System.out.println(String.format("%-6s|%6s|", "ab", "cd") + String.format("[%3d%%]", 50));
        System.out.println(String.format("%x %x %o", -1, 255L, -8));
        System.out.println(String.format("%g %g %g", 0.0001234, 123456.0, 1.0));
        System.out.println(String.format("Name: %s, Age: %d, GPA: %.2f", "Ada", 36, 3.875));
        double[] vals = {0.5, 1.5, 2.5, -0.5, 1.125, 10.0};
        for (double v : vals) System.out.print(String.format("%.0f/%.2f ", v, v));
        System.out.println();
        System.out.println(String.format("%5.1f%%", 12.345) + " " + String.format("%,d", -9876543210L));
        System.out.println(String.format("%2$s %1$s", "world", "hello"));
    }
}
