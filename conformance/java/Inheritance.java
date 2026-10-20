import java.util.*;

public class Inheritance {
    interface Named {
        String name();

        default String greet() { return "Hello, " + name() + "!"; }

        static Named of(String n) { return () -> n; }
    }

    static abstract class Animal implements Named, Comparable<Animal> {
        private static int count = 0;
        protected final String name;
        protected int legs;

        static {
            System.out.println("Animal class initialized");
        }

        {
            count++;
        }

        Animal(String name, int legs) {
            this.name = name;
            this.legs = legs;
        }

        public String name() { return name; }
        abstract String sound();
        String describe() { return name + " (" + legs + " legs) says " + sound(); }
        static int count() { return count; }

        @Override
        public int compareTo(Animal o) { return name.compareTo(o.name); }

        @Override
        public String toString() { return getClass().getSimpleName() + ":" + name; }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Animal a)) return false;
            return name.equals(a.name) && legs == a.legs;
        }

        @Override
        public int hashCode() { return Objects.hash(name, legs); }
    }

    static class Dog extends Animal {
        Dog(String name) { super(name, 4); }
        String sound() { return "Woof"; }
        @Override
        public String greet() { return "Wag! " + super.greet(); }
    }

    static class Puppy extends Dog {
        Puppy(String name) { super(name + " Jr."); }
        @Override
        String sound() { return super.sound().toLowerCase() + "?"; }
    }

    static class Bird extends Animal {
        private final boolean canFly;
        Bird(String name, boolean canFly) {
            super(name, 2);
            this.canFly = canFly;
        }
        String sound() { return canFly ? "Tweet" : "Squawk"; }
        @Override
        String describe() { return super.describe() + (canFly ? " and flies" : " and walks"); }
    }

    static class Counter {
        static int instances;
        final int id;
        Counter() { this(++instances * 10); }
        Counter(int id) { this.id = id; }
    }

    public static void main(String[] args) {
        System.out.println("main starts");
        List<Animal> zoo = new ArrayList<>(List.of(new Dog("Rex"), new Bird("Tweety", true), new Puppy("Rex"), new Bird("Pingu", false)));
        for (Animal a : zoo)
            System.out.println(a.describe());
        Collections.sort(zoo);
        System.out.println(zoo);
        System.out.println("count=" + Animal.count());
        System.out.println(zoo.get(0).greet() + " | " + zoo.get(1).greet());
        System.out.println(Named.of("Ada").greet());
        Animal d1 = new Dog("Rex"), d2 = new Dog("Rex");
        System.out.println(d1.equals(d2) + " " + (d1 == d2) + " " + (d1.hashCode() == d2.hashCode()) + " " + new HashSet<>(List.of(d1, d2)).size());
        Animal p = new Puppy("Bo");
        System.out.println((p instanceof Dog) + " " + (p instanceof Puppy) + " " + p.sound() + " " + ((Dog) p).legs);
        Object o = zoo.get(2);
        if (o instanceof Bird b && b.legs == 2)
            System.out.println("bird " + b.name());
        Counter c1 = new Counter(), c2 = new Counter(), c3 = new Counter(7);
        System.out.println(c1.id + " " + c2.id + " " + c3.id + " " + Counter.instances);
        System.out.println("count=" + Animal.count());
    }
}
