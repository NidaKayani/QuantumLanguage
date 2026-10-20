import java.util.*;

public class ExceptionsMore {
    static class ValidationException extends Exception {
        final int code;
        ValidationException(String msg, int code) {
            super(msg);
            this.code = code;
        }
    }

    static class FieldException extends ValidationException {
        FieldException(String field) { super("bad field: " + field, 42); }
    }

    static class Res implements AutoCloseable {
        final String name;
        final boolean failOnClose;
        Res(String name, boolean failOnClose) {
            this.name = name;
            this.failOnClose = failOnClose;
            System.out.println("open " + name);
        }
        public void close() throws Exception {
            System.out.println("close " + name);
            if (failOnClose) throw new IllegalStateException("close failed: " + name);
        }
    }

    static void validate(String field, int value) throws ValidationException {
        if (field.isEmpty()) throw new FieldException("<empty>");
        if (value < 0) throw new ValidationException("negative " + field, 7);
    }

    @SuppressWarnings("finally")
    static int finallyWins() {
        try {
            return 1;
        } finally {
            return 2;
        }
    }

    static int order(List<String> log) {
        int x = 10;
        try {
            log.add("try");
            return x;
        } finally {
            x = 20;
            log.add("finally x=" + x);
        }
    }

    static void rethrow(int n) {
        try {
            if (n == 0) throw new ArithmeticException("zero");
            Object o = "str";
            Integer i = (Integer) o;
        } catch (ArithmeticException | ClassCastException e) {
            throw new RuntimeException("wrapped " + e.getClass().getSimpleName(), e);
        }
    }

    public static void main(String[] args) {
        String[][] cases = {{"age", "5"}, {"", "1"}, {"score", "-3"}};
        for (String[] c : cases) {
            try {
                validate(c[0], Integer.parseInt(c[1]));
                System.out.println("ok " + c[0]);
            } catch (FieldException e) {
                System.out.println("field: " + e.getMessage() + " " + e.code);
            } catch (ValidationException e) {
                System.out.println("validation: " + e.getMessage() + " " + e.code);
            } finally {
                System.out.println("checked " + Arrays.toString(c));
            }
        }
        try (Res a = new Res("A", false); Res b = new Res("B", true)) {
            System.out.println("body");
            throw new RuntimeException("body failed");
        } catch (Exception e) {
            System.out.println("caught " + e.getMessage() + " suppressed=" + e.getSuppressed().length + " " + e.getSuppressed()[0].getMessage());
        }
        try (Res c = new Res("C", false)) {
            System.out.println("using C");
        } catch (Exception e) {
            System.out.println("unexpected");
        }
        System.out.println(finallyWins());
        List<String> log = new ArrayList<>();
        System.out.println(order(log) + " " + log);
        for (int n = 0; n < 2; n++) {
            try {
                rethrow(n);
            } catch (RuntimeException e) {
                System.out.println(e.getMessage() + " <- " + e.getCause().getMessage());
            }
        }
        try {
            Object[] objs = new String[1];
            List<Integer> list = new ArrayList<>();
            list.get(3);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("IOOBE " + e.getMessage());
        }
        try {
            Map<String, Integer> m = new HashMap<>();
            int v = m.get("missing");
        } catch (NullPointerException e) {
            System.out.println("NPE on unboxing");
        }
        try {
            Iterator<Integer> it = new ArrayList<Integer>().iterator();
            it.next();
        } catch (NoSuchElementException e) {
            System.out.println("NSEE");
        }
        try {
            throw new Error("custom error");
        } catch (Throwable t) {
            System.out.println(t + " | " + (t instanceof Exception));
        }
        int depth = 0;
        try {
            depth = recurse(0);
        } catch (StackOverflowError e) {
            System.out.println("SOE caught");
        }
        RuntimeException chain = new RuntimeException("top", new IllegalArgumentException("middle", new NullPointerException("bottom")));
        Throwable t = chain;
        StringBuilder sb = new StringBuilder();
        while (t != null) {
            sb.append(t.getClass().getSimpleName()).append(':').append(t.getMessage()).append(' ');
            t = t.getCause();
        }
        System.out.println(sb.toString().trim());
        System.out.println(new Exception().getMessage() + " " + new Exception((String) null).getMessage() + " " + new Exception(new Error("x")).getMessage());
    }

    static int recurse(int n) { return recurse(n + 1) + 1; }
}
