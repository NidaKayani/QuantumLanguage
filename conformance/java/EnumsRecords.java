import java.util.ArrayList;
import java.util.List;

public class EnumsRecords {
    enum Color { RED, GREEN, BLUE }

    enum Planet {
        MERCURY(3.303e+23, 2.4397e6),
        EARTH(5.976e+24, 6.37814e6);

        private final double mass, radius;

        Planet(double mass, double radius) {
            this.mass = mass;
            this.radius = radius;
        }

        double surfaceGravity() {
            return 6.67300E-11 * mass / (radius * radius);
        }
    }

    enum Op {
        ADD("+") {
            int apply(int a, int b) { return a + b; }
        },
        MUL("*") {
            int apply(int a, int b) { return a * b; }
        };

        final String symbol;

        Op(String symbol) {
            this.symbol = symbol;
        }

        abstract int apply(int a, int b);
    }

    record Point(int x, int y) {
        Point {
            if (x < 0) throw new IllegalArgumentException("negative x");
        }

        double dist() {
            return Math.sqrt(x * x + y * y);
        }

        static Point origin() {
            return new Point(0, 0);
        }
    }

    record Pair<A, B>(A first, B second) {}

    sealed interface Expr permits Num, Add, Mul {}
    record Num(int v) implements Expr {}
    record Add(Expr l, Expr r) implements Expr {}
    record Mul(Expr l, Expr r) implements Expr {}

    static int eval(Expr e) {
        return switch (e) {
            case Num n -> n.v();
            case Add a -> eval(a.l()) + eval(a.r());
            case Mul m -> eval(m.l()) * eval(m.r());
        };
    }

    static String kind(Object o) {
        return switch (o) {
            case null -> "null!";
            case Integer i when i > 10 -> "big int " + i;
            case Integer i -> "int " + i;
            case String s -> "string of " + s.length();
            default -> "other " + o;
        };
    }

    public static void main(String[] args) {
        for (Color c : Color.values()) System.out.print(c + ":" + c.ordinal() + " ");
        System.out.println();
        Color c = Color.valueOf("GREEN");
        System.out.println(c.name() + " " + (c == Color.GREEN) + " " + c.compareTo(Color.BLUE) + " " + Color.values().length);
        switch (c) {
            case RED -> System.out.println("is red");
            case GREEN -> System.out.println("is green");
            default -> System.out.println("is other");
        }
        for (Planet p : Planet.values()) System.out.printf("%s %.2f%n", p, p.surfaceGravity());
        for (Op op : Op.values()) System.out.println("6 " + op.symbol + " 7 = " + op.apply(6, 7));
        Point p = new Point(3, 4);
        Point q = new Point(3, 4);
        System.out.println(p + " " + p.x() + " " + p.dist() + " " + p.equals(q) + " " + (p == q) + " " + (p.hashCode() == q.hashCode()));
        System.out.println(Point.origin());
        try {
            new Point(-1, 0);
        } catch (IllegalArgumentException e) {
            System.out.println("rejected: " + e.getMessage());
        }
        Pair<String, List<Integer>> pair = new Pair<>("nums", new ArrayList<>(List.of(1, 2)));
        System.out.println(pair + " " + pair.second().size());
        Expr e = new Add(new Num(2), new Mul(new Num(3), new Num(4)));
        System.out.println(e + " = " + eval(e));
        System.out.println(kind(42) + ", " + kind(7) + ", " + kind("hey") + ", " + kind(2.5) + ", " + kind(null));
    }
}
