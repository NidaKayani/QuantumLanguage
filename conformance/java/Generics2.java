import java.util.*;
import java.util.function.*;

public class Generics2 {
    static class Box<T extends Comparable<T>> {
        private T value;
        Box(T value) { this.value = value; }
        T get() { return value; }
        boolean isBigger(Box<T> other) { return value.compareTo(other.value) > 0; }
        <R extends Comparable<R>> Box<R> map(Function<? super T, ? extends R> f) { return new Box<>(f.apply(value)); }
        @Override
        public String toString() { return "Box(" + value + ")"; }
    }

    record Pair<A, B>(A first, B second) {
        <C> Pair<A, C> withSecond(C c) { return new Pair<>(first, c); }
        static <X> Pair<X, X> twin(X x) { return new Pair<>(x, x); }
    }

    interface Stack<E> extends Iterable<E> {
        void push(E e);
        E pop();
        boolean isEmpty();
    }

    static class ArrayStack<E> implements Stack<E> {
        private final List<E> items = new ArrayList<>();
        public void push(E e) { items.add(e); }
        public E pop() { return items.remove(items.size() - 1); }
        public boolean isEmpty() { return items.isEmpty(); }
        public Iterator<E> iterator() {
            return new Iterator<E>() {
                int i = items.size() - 1;
                public boolean hasNext() { return i >= 0; }
                public E next() { return items.get(i--); }
            };
        }
    }

    static <T extends Comparable<? super T>> T maxOf(List<? extends T> list) {
        T best = list.get(0);
        for (T t : list)
            if (t.compareTo(best) > 0) best = t;
        return best;
    }

    static double sum(Collection<? extends Number> nums) {
        double s = 0;
        for (Number n : nums) s += n.doubleValue();
        return s;
    }

    static void fill(List<? super Integer> sink, int n) {
        for (int i = 0; i < n; i++) sink.add(i);
    }

    static <K, V extends Comparable<V>> K argMax(Map<K, V> m) {
        K best = null;
        V bestV = null;
        for (Map.Entry<K, V> e : m.entrySet())
            if (bestV == null || e.getValue().compareTo(bestV) > 0) {
                best = e.getKey();
                bestV = e.getValue();
            }
        return best;
    }

    @SafeVarargs
    static <T> List<T> listOf(T... xs) { return new ArrayList<>(Arrays.asList(xs)); }

    static <T> void swap(T[] a, int i, int j) {
        T t = a[i];
        a[i] = a[j];
        a[j] = t;
    }

    public static void main(String[] args) {
        Box<Integer> b1 = new Box<>(5), b2 = new Box<>(9);
        System.out.println(b1.isBigger(b2) + " " + b2.isBigger(b1) + " " + b1.map(x -> "v" + x) + " " + b2.map(x -> x * 0.5).get());
        Pair<String, Integer> p = new Pair<>("age", 30);
        Pair<String, List<String>> q = p.withSecond(List.of("x"));
        System.out.println(p + " " + q + " " + Pair.twin('z') + " " + p.equals(new Pair<>("age", 30)));
        Stack<String> st = new ArrayStack<>();
        for (String s : "a b c d".split(" ")) st.push(s);
        StringBuilder sb = new StringBuilder();
        for (String s : st) sb.append(s);
        sb.append('|').append(st.pop()).append(st.pop());
        System.out.println(sb + " " + st.isEmpty());
        System.out.println(maxOf(List.of(3, 17, 5)) + " " + maxOf(List.of("pear", "apple", "zoo")) + " " + sum(List.of(1, 2.5, 3L)) + " " + sum(Set.of(10)));
        List<Number> sink = new ArrayList<>();
        fill(sink, 4);
        List<Object> objs = new ArrayList<>(List.of("s"));
        fill(objs, 2);
        System.out.println(sink + " " + objs);
        Map<String, Double> scores = new LinkedHashMap<>();
        scores.put("ann", 81.5);
        scores.put("bob", 92.0);
        scores.put("cid", 77.25);
        System.out.println(argMax(scores) + " " + listOf(1, 2, 3).getClass().getSimpleName() + " " + listOf().size());
        String[] arr = {"x", "y", "z"};
        swap(arr, 0, 2);
        System.out.println(String.join("", arr));
        Map<String, List<Integer>> index = new TreeMap<>();
        String[] words = "to be or not to be".split(" ");
        for (int i = 0; i < words.length; i++)
            index.computeIfAbsent(words[i], k -> new ArrayList<>()).add(i);
        System.out.println(index);
        List<? extends Number> ro = List.of(1, 2.0);
        Number first = ro.get(0);
        System.out.println(first + " " + ro.size());
        BiFunction<Integer, Integer, Integer> add = Integer::sum;
        Function<Integer, Function<Integer, Integer>> curried = x -> y -> x * 10 + y;
        System.out.println(add.apply(2, 3) + " " + curried.apply(4).apply(2));
    }
}
