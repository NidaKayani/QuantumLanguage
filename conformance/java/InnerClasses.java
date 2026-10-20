import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.Supplier;

public class InnerClasses {
    private int base = 10;
    private static int staticCount = 0;

    class Adder {
        int add(int x) {
            return base + x;
        }
    }

    static class Counter {
        private int n;

        void inc() {
            n++;
            staticCount++;
        }

        int get() {
            return n;
        }
    }

    interface Greeter {
        String greet(String name);
    }

    static class Range implements Iterable<Integer> {
        private final int lo, hi;

        Range(int lo, int hi) {
            this.lo = lo;
            this.hi = hi;
        }

        public Iterator<Integer> iterator() {
            return new Iterator<Integer>() {
                private int cur = lo;

                public boolean hasNext() {
                    return cur < hi;
                }

                public Integer next() {
                    return cur++;
                }
            };
        }
    }

    static class Node<T> {
        T value;
        Node<T> next;

        Node(T value, Node<T> next) {
            this.value = value;
            this.next = next;
        }
    }

    List<Supplier<String>> makeSuppliers() {
        List<Supplier<String>> out = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            final int k = i;
            out.add(() -> "supplier " + k + " base " + base);
        }
        return out;
    }

    public static void main(String[] args) {
        InnerClasses outer = new InnerClasses();
        InnerClasses.Adder adder = outer.new Adder();
        System.out.println(adder.add(5));
        outer.base = 100;
        System.out.println(adder.add(5));
        Counter c1 = new Counter(), c2 = new Counter();
        c1.inc();
        c1.inc();
        c2.inc();
        System.out.println(c1.get() + " " + c2.get() + " " + staticCount);
        String punct = "!";
        Greeter polite = new Greeter() {
            @Override
            public String greet(String name) {
                return "Good day, " + name + punct;
            }
        };
        Greeter casual = name -> "Hey " + name + punct;
        System.out.println(polite.greet("Ada") + " / " + casual.greet("Bob"));
        int total = 0;
        for (int v : new Range(1, 6)) total += v;
        System.out.println("range total " + total);
        Node<String> list = new Node<>("a", new Node<>("b", new Node<>("c", null)));
        StringBuilder sb = new StringBuilder();
        for (Node<String> n = list; n != null; n = n.next) sb.append(n.value);
        System.out.println(sb);
        for (Supplier<String> s : outer.makeSuppliers()) System.out.println(s.get());
        class Local {
            int twice(int x) {
                return x * 2 + punct.length();
            }
        }
        System.out.println(new Local().twice(20));
        Runnable r = () -> System.out.println("runnable ran");
        r.run();
    }
}
