import java.util.ArrayList;
import java.util.List;

interface Shape extends Comparable<Shape> {
    double area();

    default String describe() {
        return name() + " with area " + String.format("%.2f", area());
    }

    String name();

    static Shape largest(List<Shape> shapes) {
        Shape best = shapes.get(0);
        for (Shape s : shapes) if (s.area() > best.area()) best = s;
        return best;
    }

    @Override
    default int compareTo(Shape other) {
        return Double.compare(area(), other.area());
    }
}

abstract class AbstractShape implements Shape {
    private static int created = 0;
    protected final String label;

    AbstractShape(String label) {
        this.label = label;
        created++;
    }

    public String name() {
        return getClass().getSimpleName() + "(" + label + ")";
    }

    static int getCreated() {
        return created;
    }

    @Override
    public String toString() {
        return name();
    }
}

class Circle extends AbstractShape {
    private final double r;

    Circle(String label, double r) {
        super(label);
        this.r = r;
    }

    public double area() {
        return Math.PI * r * r;
    }
}

class Rect extends AbstractShape {
    protected final double w, h;

    Rect(String label, double w, double h) {
        super(label);
        this.w = w;
        this.h = h;
    }

    Rect(double side) {
        this("square", side, side);
    }

    public double area() {
        return w * h;
    }

    @Override
    public String describe() {
        return "Rectangle " + w + "x" + h + " -> " + super.describe();
    }
}

class Square extends Rect {
    Square(double s) {
        super(s);
    }

    @Override
    public String name() {
        return "Square[" + w + "]";
    }
}

public class Shapes {
    public static void main(String[] args) {
        List<Shape> shapes = new ArrayList<>();
        shapes.add(new Circle("c1", 1.5));
        shapes.add(new Rect("r1", 2, 3));
        shapes.add(new Square(2.5));
        shapes.add(new Circle("tiny", 0.5));
        for (Shape s : shapes) System.out.println(s.describe());
        System.out.println("largest: " + Shape.largest(shapes));
        shapes.sort(null);
        System.out.println("sorted: " + shapes);
        System.out.println("created: " + AbstractShape.getCreated());
        Shape s = shapes.get(1);
        if (s instanceof Rect r) System.out.println("rect width " + r.w);
        else System.out.println("not a rect: " + s.name());
        Object o = shapes.get(2);
        System.out.println((o instanceof Shape) + " " + (o instanceof Circle) + " " + (o instanceof Comparable<?>));
        AbstractShape anon = new AbstractShape("anon") {
            public double area() {
                return 42;
            }
        };
        System.out.println(anon.describe() + " | " + anon.label);
    }
}
