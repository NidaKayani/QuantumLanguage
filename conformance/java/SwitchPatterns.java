import java.util.*;

public class SwitchPatterns {
    sealed interface Shape permits Circle, Square, Rect {}
    record Circle(double r) implements Shape {}
    record Square(double side) implements Shape {}
    record Rect(double w, double h) implements Shape {}

    enum Day { MON, TUE, WED, THU, FRI, SAT, SUN }

    static double area(Shape s) {
        return switch (s) {
            case Circle c -> Math.PI * c.r() * c.r();
            case Square sq -> sq.side() * sq.side();
            case Rect r -> r.w() * r.h();
        };
    }

    static String classify(Object o) {
        return switch (o) {
            case null -> "null!";
            case Integer i when i > 100 -> "big int " + i;
            case Integer i -> "int " + i;
            case String s when s.isEmpty() -> "empty string";
            case String s -> "string of " + s.length();
            case int[] arr -> "int array of " + arr.length;
            case Circle c when c.r() == 0 -> "point";
            case Shape sh -> "shape with area " + String.format("%.1f", area(sh));
            default -> "other " + o.getClass().getSimpleName();
        };
    }

    static String kind(Day d) {
        return switch (d) {
            case SAT, SUN -> "weekend";
            case FRI -> {
                String s = "almost";
                yield s + " weekend";
            }
            default -> "weekday";
        };
    }

    static int daysIn(int month, boolean leap) {
        int days;
        switch (month) {
            case 2:
                days = leap ? 29 : 28;
                break;
            case 4: case 6: case 9: case 11:
                days = 30;
                break;
            default:
                days = 31;
        }
        return days;
    }

    static String fallthrough(int n) {
        StringBuilder sb = new StringBuilder();
        switch (n) {
            case 1: sb.append("one ");
            case 2: sb.append("two ");
            case 3: sb.append("three "); break;
            case 4: sb.append("four ");
            default: sb.append("many ");
        }
        return sb.toString().trim();
    }

    static String describe(Object o) {
        if (o instanceof Rect r && r.w() == r.h()) return "square-ish rect";
        if (!(o instanceof String s)) return "not a string";
        return "string " + s.toUpperCase();
    }

    public static void main(String[] args) {
        List<Shape> shapes = List.of(new Circle(1), new Square(2), new Rect(2, 3));
        for (Shape s : shapes) System.out.printf("%s -> %.2f%n", s, area(s));
        for (Object o : new Object[] {7, 1000, "", "hey", new int[] {1, 2}, new Circle(0), new Rect(1, 2), 3.5, null, 'c'})
            System.out.println(classify(o));
        for (Day d : Day.values()) System.out.print(d + ":" + kind(d) + " ");
        System.out.println();
        System.out.println(daysIn(2, true) + " " + daysIn(2, false) + " " + daysIn(9, false) + " " + daysIn(12, false));
        for (int i = 1; i <= 5; i++) System.out.print("[" + fallthrough(i) + "]");
        System.out.println();
        System.out.println(describe(new Rect(3, 3)) + ", " + describe(new Rect(1, 3)) + ", " + describe("abc"));
        char grade = 'B';
        String msg = switch (grade) {
            case 'A', 'B' -> "good";
            case 'C' -> "ok";
            default -> "bad";
        };
        System.out.println(msg + " " + Day.valueOf("WED").ordinal() + " " + Day.FRI.compareTo(Day.MON) + " " + Arrays.toString(Day.values()).length());
        int code = 3;
        String word = switch (code) {
            case 1, 2, 3 -> {
                if (code > 2) yield "three-ish";
                yield "small";
            }
            default -> "large";
        };
        System.out.println(word);
    }
}
