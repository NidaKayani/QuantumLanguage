import java.util.*;
import java.util.function.*;

public class Generics {
    static class Box<T extends Comparable<T>> {
        private T value;

        Box(T value) {
            this.value = value;
        }

        T get() {
            return value;
        }

        boolean isBigger(Box<T> other) {
            return value.compareTo(other.value) > 0;
        }

        <R extends Comparable<R>> Box<R> map(Function<T, R> f) {
            return new Box<>(f.apply(value));
        }

        @Override
        public String toString() {
            return "Box(" + value + ")";
        }
    }

    static class Stack<E> implements Iterable<E> {
        private final ArrayList<E> items = new ArrayList<>();

        void push(E e) {
            items.add(e);
        }

        E pop() {
            if (items.isEmpty()) throw new RuntimeException("empty stack");
            return items.remove(items.size() - 1);
        }

        boolean isEmpty() {
            return items.isEmpty();
        }

        public Iterator<E> iterator() {
            return items.iterator();
        }
    }

    static <T extends Comparable<? super T>> T maxOf(List<T> list) {
        T best = list.get(0);
        for (T t : list) if (t.compareTo(best) > 0) best = t;
        return best;
    }

    static <K, V> Map<V, K> invert(Map<K, V> m) {
        Map<V, K> out = new TreeMap<>();
        for (Map.Entry<K, V> e : m.entrySet()) out.put(e.getValue(), e.getKey());
        return out;
    }

    static double sumAll(Collection<? extends Number> nums) {
        double s = 0;
        for (Number n : nums) s += n.doubleValue();
        return s;
    }

    static void fill(List<? super Integer> sink, int n) {
        for (int i = 0; i < n; i++) sink.add(i);
    }

    @SafeVarargs
    static <T> List<T> listOf(T... items) {
        return new ArrayList<>(Arrays.asList(items));
    }

    public static void main(String[] args) {
        Box<Integer> a = new Box<>(7), b = new Box<>(3);
        System.out.println(a + " " + a.isBigger(b) + " " + a.map(x -> "v" + x) + " " + a.map(x -> x * 1.5).get());
        Stack<String> st = new Stack<>();
        st.push("x");
        st.push("y");
        st.push("z");
        for (String s : st) System.out.print(s);
        System.out.println(" pop=" + st.pop());
        System.out.println(maxOf(Arrays.asList(3, 17, 5)) + " " + maxOf(List.of("pear", "apple", "zoo")));
        Map<String, Integer> ages = new HashMap<>();
        ages.put("ann", 30);
        ages.put("bob", 25);
        System.out.println(invert(ages));
        List<Double> ds = new ArrayList<>();
        ds.add(1.5);
        ds.add(2.25);
        List<Integer> is = List.of(1, 2, 3);
        System.out.println(sumAll(ds) + " " + sumAll(is) + " " + ds);
        List<Number> sink = new ArrayList<>();
        fill(sink, 3);
        sink.add(2.5);
        System.out.println(sink);
        List<String> l = listOf("a", "b");
        l.add("c");
        System.out.println(l + " " + listOf(1, 2, 3).size());
        BiFunction<Integer, Integer, Integer> add = (x, y) -> x + y;
        Function<Integer, Integer> twice = x -> x * 2;
        UnaryOperator<String> shout = s -> s.toUpperCase() + "!";
        Predicate<String> isEmpty = String::isEmpty;
        Supplier<List<String>> maker = ArrayList::new;
        BinaryOperator<Integer> mul = (x, y) -> x * y;
        System.out.println(add.andThen(twice).apply(3, 4) + " " + twice.compose(twice).apply(5) + " " + shout.apply("hi"));
        System.out.println(isEmpty.test("") + " " + isEmpty.negate().test("") + " " + maker.get().size() + " " + mul.apply(6, 7));
        Optional<String> opt = Optional.of("value");
        Optional<String> none = Optional.empty();
        System.out.println(opt.map(String::length).orElse(-1) + " " + none.orElse("default") + " " + opt.isPresent() + " " + none.isEmpty());
        Map<Boolean, List<Integer>> parts = new TreeMap<>();
        for (int i = 1; i <= 6; i++) parts.computeIfAbsent(i % 2 == 0, k -> new ArrayList<>()).add(i);
        System.out.println(parts);
        Comparator<String> byLen = Comparator.comparingInt(String::length);
        List<String> words = new ArrayList<>(List.of("ccc", "a", "bb", "dd", "e"));
        words.sort(byLen.thenComparing(Comparator.reverseOrder()));
        System.out.println(words);
    }
}
