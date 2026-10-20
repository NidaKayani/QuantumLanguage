public class Uncaught {
    static int divide(int a, int b) {
        return a / b;
    }

    public static void main(String[] args) {
        System.out.println("before");
        System.out.println(divide(10, 2));
        System.out.println(divide(1, 0));
        System.out.println("never printed");
    }
}
