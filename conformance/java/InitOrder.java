public class InitOrder {
    static int counter = next("static field");

    static {
        System.out.println("static block");
    }

    int instanceField = next("instance field");

    {
        System.out.println("instance block");
    }

    InitOrder() {
        this("delegating");
        System.out.println("no-arg constructor");
    }

    InitOrder(String why) {
        System.out.println("constructor(" + why + ")");
    }

    static int next(String what) {
        System.out.println("init " + what);
        return 1;
    }

    static class Lazy {
        static {
            System.out.println("Lazy initialized");
        }

        static int VALUE = 7;

        static void touch() {
            System.out.println("Lazy.touch");
        }
    }

    static class Base {
        Base() {
            System.out.println("Base() sees " + describe());
        }

        String describe() {
            return "base";
        }
    }

    static class Derived extends Base {
        private String name = "derived-field";

        Derived() {
            super();
            System.out.println("Derived() sees " + describe());
        }

        @Override
        String describe() {
            return "derived:" + name;
        }
    }

    public static void main(String[] args) {
        System.out.println("main starts");
        new InitOrder();
        System.out.println("before Lazy");
        Lazy.touch();
        System.out.println("Lazy.VALUE=" + Lazy.VALUE);
        new Derived();
    }
}
