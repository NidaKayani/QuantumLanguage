import java.util.*;

public class Exceptions {
    static class InsufficientFundsException extends Exception {
        private final double shortfall;

        InsufficientFundsException(String msg, double shortfall) {
            super(msg);
            this.shortfall = shortfall;
        }

        double getShortfall() {
            return shortfall;
        }
    }

    static class Account {
        private double balance;

        Account(double b) {
            balance = b;
        }

        void withdraw(double amt) throws InsufficientFundsException {
            if (amt > balance) throw new InsufficientFundsException("need more money", amt - balance);
            balance -= amt;
        }
    }

    static int depth = 0;

    static int finallyDemo() {
        try {
            return 1;
        } finally {
            System.out.println("finally runs before return");
        }
    }

    static int finallyOverride() {
        try {
            throw new RuntimeException("lost");
        } finally {
            return 42;
        }
    }

    static String nested(int n) {
        StringBuilder log = new StringBuilder();
        for (int i = 0; i < n; i++) {
            try {
                try {
                    if (i == 1) continue;
                    if (i == 3) break;
                    log.append("body").append(i).append(' ');
                } finally {
                    log.append("inner-finally").append(i).append(' ');
                }
            } finally {
                log.append("outer").append(i).append(' ');
            }
        }
        return log.toString().trim();
    }

    static void recurse() {
        depth++;
        recurse();
    }

    public static void main(String[] args) {
        try {
            int[] a = new int[3];
            a[5] = 1;
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("AIOOBE: " + e.getMessage());
        }
        try {
            System.out.println(10 / 0);
        } catch (ArithmeticException e) {
            System.out.println("Arithmetic: " + e.getMessage() + " | " + e);
        }
        try {
            String s = null;
            s.length();
        } catch (NullPointerException e) {
            System.out.println("NPE caught");
        }
        try {
            Integer.parseInt("12x");
        } catch (NumberFormatException e) {
            System.out.println(e.getMessage());
        }
        try {
            Object o = "string";
            Integer i = (Integer) o;
        } catch (ClassCastException e) {
            System.out.println("ClassCastException caught");
        }
        try {
            new Account(100).withdraw(250);
        } catch (InsufficientFundsException e) {
            System.out.println(e.getMessage() + " short by " + e.getShortfall());
        }
        try {
            throw new IllegalStateException("bad state");
        } catch (IllegalArgumentException | IllegalStateException e) {
            System.out.println("multi-catch: " + e.getClass().getSimpleName() + " " + e.getMessage());
        } finally {
            System.out.println("finally after multi-catch");
        }
        System.out.println(finallyDemo() + " " + finallyOverride());
        System.out.println(nested(5));
        try {
            try {
                throw new RuntimeException("inner");
            } catch (RuntimeException e) {
                throw new IllegalArgumentException("wrapped", e);
            }
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage() + " caused by " + e.getCause().getMessage());
        }
        try {
            recurse();
        } catch (StackOverflowError e) {
            System.out.println("stack overflow caught, depth > 1000: " + (depth > 1000));
        }
        try {
            List<Integer> l = new ArrayList<>();
            l.get(0);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("IOOBE: " + e.getMessage());
        }
        try {
            new ArrayDeque<Integer>().pop();
        } catch (NoSuchElementException e) {
            System.out.println("NoSuchElement");
        }
        try {
            Map<String, Integer> m = new HashMap<>();
            int v = m.get("missing");
        } catch (NullPointerException e) {
            System.out.println("unboxing null -> NPE");
        }
        try {
            "abc".substring(5);
        } catch (StringIndexOutOfBoundsException e) {
            System.out.println("SIOOBE: " + e.getMessage());
        }
        try (AutoCloseable r = () -> System.out.println("resource closed")) {
            System.out.println("using resource");
        } catch (Exception e) {
            System.out.println("unexpected");
        }
        Exception chained = new Exception("top", new RuntimeException("root"));
        System.out.println(chained + " / " + chained.getCause());
        System.out.println("done");
    }
}
