import java.util.*;
import java.util.function.*;

public class Lambdas {
    @FunctionalInterface
    interface TriFunction<A, B, C, R> {
        R apply(A a, B b, C c);
    }

    static class Greeter {
        private final String greeting;
        Greeter(String greeting) { this.greeting = greeting; }
        String greet(String name) { return greeting + ", " + name; }
        static String shout(String s) { return s.toUpperCase() + "!"; }
    }

    static int applyTwice(IntUnaryOperator f, int x) { return f.applyAsInt(f.applyAsInt(x)); }

    public static void main(String[] args) {
        List<Runnable> tasks = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            int j = i;
            tasks.add(() -> System.out.print("task" + j + " "));
        }
        for (String s : List.of("a", "b"))
            tasks.add(() -> System.out.print(s + " "));
        tasks.forEach(Runnable::run);
        System.out.println();
        TriFunction<Integer, Integer, Integer, Integer> vol = (a, b, c) -> a * b * c;
        System.out.println(vol.apply(2, 3, 4));
        Greeter g = new Greeter("Hi");
        Function<String, String> bound = g::greet;
        Function<String, String> stat = Greeter::shout;
        BiFunction<Greeter, String, String> unbound = Greeter::greet;
        Function<String, Greeter> ctor = Greeter::new;
        System.out.println(bound.apply("Ann") + " | " + stat.apply("hey") + " | " + unbound.apply(new Greeter("Yo"), "Bob") + " | " + ctor.apply("Hey").greet("Cid"));
        Predicate<String> empty = String::isEmpty;
        Predicate<String> longer = s -> s.length() > 3;
        System.out.println(empty.negate().and(longer).test("hello") + " " + empty.or(longer).test("") + " " + Predicate.not(empty).test("x") + " " + Predicate.isEqual("a").test("a"));
        Comparator<String> byLen = Comparator.comparingInt(String::length);
        List<String> ws = new ArrayList<>(List.of("banana", "kiwi", "apple", "fig", "cherry"));
        ws.sort(byLen.thenComparing(Comparator.naturalOrder()));
        System.out.println(ws);
        ws.sort(byLen.reversed().thenComparing(Function.identity(), Comparator.reverseOrder()));
        System.out.println(ws);
        System.out.println(applyTwice(x -> x * 3, 2) + " " + applyTwice(Math::abs, -5) + " " + IntUnaryOperator.identity().andThen(x -> x + 1).applyAsInt(1));
        Supplier<Integer> counter = new Supplier<>() {
            int n = 0;
            public Integer get() { return ++n; }
        };
        System.out.println(counter.get() + counter.get() + counter.get());
        int[] acc = {0};
        List.of(1, 2, 3, 4).forEach(x -> acc[0] += x);
        System.out.println(acc[0]);
        Map<String, Integer> m = new TreeMap<>();
        BiConsumer<String, Integer> put = m::put;
        put.accept("one", 1);
        put.andThen((k, v) -> m.put(k + k, v * 2)).accept("two", 2);
        m.replaceAll((k, v) -> v * 100);
        m.forEach((k, v) -> System.out.print(k + "=" + v + ";"));
        System.out.println();
        UnaryOperator<String> up = String::toUpperCase;
        System.out.println(up.andThen(s -> s + "?").apply("why") + " " + Optional.of("v").map(String::length).get());
        Runnable r = () -> {
            class Local {
                String hello() { return "local class in lambda"; }
            }
            System.out.println(new Local().hello());
        };
        r.run();
        Callback cb = new Callback();
        cb.register(msg -> "got " + msg);
        cb.register(String::toUpperCase);
        System.out.println(cb.fire("ping"));
        IntBinaryOperator gcd = new IntBinaryOperator() {
            public int applyAsInt(int a, int b) { return b == 0 ? a : applyAsInt(b, a % b); }
        };
        System.out.println(gcd.applyAsInt(84, 36));
    }

    static class Callback {
        private final List<Function<String, String>> handlers = new ArrayList<>();
        void register(Function<String, String> h) { handlers.add(h); }
        String fire(String msg) {
            StringJoiner j = new StringJoiner(", ", "[", "]");
            for (Function<String, String> h : handlers) j.add(h.apply(msg));
            return j.toString();
        }
    }
}
