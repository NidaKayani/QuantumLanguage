public class ControlFlow {
    static String classify(int n) {
        if (n < 0) return "negative";
        else if (n == 0) return "zero";
        else if (n % 2 == 0) return "even";
        else return "odd";
    }

    static String dayType(String day) {
        switch (day) {
            case "SAT":
            case "SUN":
                return "weekend";
            case "WED":
                System.out.print("(midweek) ");
            default:
                return "weekday";
        }
    }

    static int daysIn(int month) {
        return switch (month) {
            case 2 -> 28;
            case 4, 6, 9, 11 -> 30;
            default -> {
                if (month < 1 || month > 12) yield -1;
                yield 31;
            }
        };
    }

    public static void main(String[] args) {
        for (int n : new int[]{-5, 0, 4, 7}) System.out.print(classify(n) + " ");
        System.out.println();
        System.out.println(dayType("SUN") + " " + dayType("WED") + " " + dayType("MON"));
        System.out.println(daysIn(2) + " " + daysIn(6) + " " + daysIn(7) + " " + daysIn(13));
        int sum = 0;
        for (int i = 1; i <= 100; i++) {
            if (i % 3 == 0) continue;
            if (i > 50) break;
            sum += i;
        }
        System.out.println("sum=" + sum);
        int count = 0;
        outer:
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (j == 5) continue outer;
                if (i == 7) break outer;
                count++;
            }
        }
        System.out.println("count=" + count);
        int x = 100, steps = 0;
        while (x != 1) {
            x = x % 2 == 0 ? x / 2 : 3 * x + 1;
            steps++;
        }
        System.out.println("collatz steps=" + steps);
        int k = 10;
        do {
            k -= 3;
        } while (k > 0);
        System.out.println("k=" + k);
        StringBuilder fizz = new StringBuilder();
        for (int i = 1; i <= 15; i++) {
            fizz.append(i % 15 == 0 ? "FizzBuzz" : i % 3 == 0 ? "Fizz" : i % 5 == 0 ? "Buzz" : String.valueOf(i));
            if (i < 15) fizz.append(' ');
        }
        System.out.println(fizz);
        char grade = 'B';
        switch (grade) {
            case 'A' -> System.out.println("excellent");
            case 'B', 'C' -> System.out.println("good");
            default -> System.out.println("other");
        }
        int total = 0;
        for (int i = 0, j = 10; i < j; i += 2, j--) total += i * j;
        System.out.println("total=" + total);
        boolean flag = false;
        if (!flag && (total > 0 || 1 / 0 == 0)) System.out.println("short-circuit ok");
        int y = 0;
        y += y++ + ++y;
        System.out.println("y=" + y);
        long loops = 0;
        for (int i = 0; i < 300; i++) for (int j = 0; j < i; j++) loops += j % 7;
        System.out.println("loops=" + loops);
    }
}
