import java.util.*;

public class InnerAnon {
    private int counter = 100;
    private static String tag = "IA";

    class Inner {
        int counter = 5;
        int sum() { return counter + InnerAnon.this.counter; }
        void bump() { InnerAnon.this.counter++; }
    }

    static class Nested {
        String hello() { return "nested " + tag; }
    }

    interface Shape {
        double area();
        default String label() { return getClass().getSimpleName().isEmpty() ? "anon" : getClass().getSimpleName(); }
    }

    static abstract class Base {
        protected final String name;
        Base(String name) { this.name = name; }
        abstract String speak();
        String intro() { return "I am " + name + ": " + speak(); }
    }

    class Range implements Iterable<Integer> {
        private final int lo, hi;
        Range(int lo, int hi) {
            this.lo = lo;
            this.hi = hi;
        }
        public Iterator<Integer> iterator() {
            return new Iterator<>() {
                private int cur = lo;
                public boolean hasNext() { return cur < hi; }
                public Integer next() { return cur++ * counter / 100; }
            };
        }
    }

    List<Runnable> makeCounters(int n) {
        List<Runnable> out = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            final int id = i;
            out.add(new Runnable() {
                int calls = 0;
                public void run() {
                    calls++;
                    System.out.print("c" + id + ":" + calls + ":" + counter + " ");
                }
            });
        }
        return out;
    }

    public static void main(String[] args) {
        InnerAnon outer = new InnerAnon();
        InnerAnon.Inner in = outer.new Inner();
        in.bump();
        System.out.println(in.sum() + " " + outer.counter + " " + new Nested().hello());
        int base = 3;
        class Local {
            int times(int x) { return x * base; }
        }
        System.out.println(new Local().times(7));
        Shape sq = new Shape() {
            public double area() { return 4.0; }
        };
        Shape circle = () -> Math.PI;
        System.out.println(sq.area() + " " + sq.label() + " " + String.format("%.3f", circle.area()));
        Base cat = new Base("Tom") {
            String speak() { return "meow"; }
        };
        System.out.println(cat.intro() + " " + new Base("Rex") {
            String speak() { return "woof x" + base; }
        }.intro());
        int total = 0;
        for (int v : outer.new Range(2, 6)) total += v;
        System.out.println(total);
        List<Runnable> cs = outer.makeCounters(2);
        cs.get(0).run();
        cs.get(0).run();
        cs.get(1).run();
        System.out.println();
        Comparator<String> byLast = new Comparator<String>() {
            @Override
            public int compare(String a, String b) { return Character.compare(a.charAt(a.length() - 1), b.charAt(b.length() - 1)); }
        };
        List<String> names = new ArrayList<>(List.of("anna", "bob", "carl", "dave"));
        names.sort(byLast);
        System.out.println(names);
        Object anon = new Object() {
            @Override
            public String toString() { return "custom toString"; }
        };
        System.out.println(anon + " " + (anon.getClass() == Object.class));
        Map<String, Integer> m = new HashMap<>() {
            {
                put("a", 1);
                put("b", 2);
            }
        };
        System.out.println(m.get("b") + " " + m.size());
    }
}
