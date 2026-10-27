public class Main {
    static void risky() throws Exception {
        throw new Exception("x");
    }

    public static void main(String[] args) {
        risky();
    }
}
