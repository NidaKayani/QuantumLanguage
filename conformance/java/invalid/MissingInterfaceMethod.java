interface Animal {
    String sound();
}

class Dog implements Animal {
    public String name() {
        return "dog";
    }
}

public class Main {
    public static void main(String[] args) {
        System.out.println(new Dog().name());
    }
}
