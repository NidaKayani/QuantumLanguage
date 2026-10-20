import java.util.Scanner;

public class ScannerInput {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("How many numbers? ");
        int n = sc.nextInt();
        int sum = 0, max = Integer.MIN_VALUE;
        for (int i = 0; i < n; i++) {
            int v = sc.nextInt();
            sum += v;
            max = Math.max(max, v);
        }
        System.out.println("sum=" + sum + " max=" + max + " avg=" + (double) sum / n);
        String rest = sc.nextLine();
        System.out.println("rest of line: [" + rest + "]");
        String line = sc.nextLine();
        System.out.println("line: " + line.toUpperCase());
        String word = sc.next();
        double d = sc.nextDouble();
        long big = sc.nextLong();
        boolean flag = sc.nextBoolean();
        System.out.println(word + " " + d * 2 + " " + (big + 1) + " " + !flag);
        int count = 0;
        long total = 0;
        while (sc.hasNextInt()) {
            total += sc.nextInt();
            count++;
        }
        System.out.println("read " + count + " more ints, total " + total);
        System.out.println("next token: " + sc.next());
        while (sc.hasNextLine()) {
            String l = sc.nextLine();
            if (!l.isBlank()) System.out.println("tail: " + l.trim());
        }
        sc.close();
    }
}
