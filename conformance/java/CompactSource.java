int square(int x) {
    return x * x;
}

String greeting = "compact source files";

void main() {
    System.out.println("Hello from " + greeting);
    for (int i = 1; i <= 3; i++) System.out.print(square(i) + " ");
    System.out.println();
}
