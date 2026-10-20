import java.util.*;

public class Boxing {
    static String which(int x) { return "int"; }
    static String which(long x) { return "long"; }
    static String which(double x) { return "double"; }
    static String which(Integer x) { return "Integer"; }
    static String which(Object x) { return "Object"; }
    static String which2(long x) { return "long"; }
    static String which2(Integer x) { return "Integer"; }
    static String which3(Object... xs) { return "varargs" + xs.length; }
    static String which3(Object x) { return "single"; }

    public static void main(String[] args) {
        Integer a = 127, b = 127, c = 1000, d = 1000;
        System.out.println(a.equals(b) + " " + c.equals(d) + " " + (c.intValue() == d) + " " + a.compareTo(c));
        List<Integer> list = new ArrayList<>(List.of(10, 20, 30));
        list.remove(1);
        list.remove(Integer.valueOf(10));
        System.out.println(list);
        Object o = 42;
        Object l = 42L;
        Object s = 'x';
        System.out.println((o instanceof Integer) + " " + (l instanceof Long) + " " + (l instanceof Integer) + " " + (s instanceof Character));
        System.out.println(which(1) + " " + which('c') + " " + which(1L) + " " + which(1.5f) + " " + which(Integer.valueOf(1)) + " " + which("s"));
        System.out.println(which2(5) + " " + which3(1) + " " + which3(1, 2) + " " + which3());
        Long big = 1L << 40;
        Double dbl = 2.5;
        Character ch = 'q';
        Boolean bool = true;
        System.out.println(big * 2 + " " + dbl * 2 + " " + (char) (ch + 1) + " " + !bool + " " + big.hashCode() + " " + dbl.hashCode() + " " + bool.hashCode());
        List<Double> ds = List.of(1.0, 2.5, 3.0);
        double total = 0;
        for (double x : ds) total += x;
        System.out.println(ds + " " + total + " " + ds.get(0));
        Map<Character, Integer> m = new TreeMap<>();
        for (char x : "hello".toCharArray()) m.put(x, m.getOrDefault(x, 0) + 1);
        System.out.println(m);
        Set<Long> longs = new HashSet<>(List.of(5000000000L, 1L, 7L));
        System.out.println(longs.contains(7L) + " " + longs.contains(7) + " " + new TreeSet<>(longs));
        int[] arr = {3, 1, 2};
        Integer[] boxed = Arrays.stream(arr).boxed().toArray(Integer[]::new);
        Arrays.sort(boxed, Collections.reverseOrder());
        System.out.println(Arrays.toString(boxed));
        System.out.println(Integer.valueOf(5).equals(5) + " " + Long.valueOf(5).equals(5) + " " + Objects.equals(null, null) + " " + Objects.hash(1, 2, 3));
        System.out.println(Integer.toString(255, 16) + " " + Integer.parseInt("ff", 16) + " " + Long.toString(-8, 2) + " " + Character.forDigit(11, 16));
        Integer nullable = null;
        System.out.println("nullable is " + nullable + " " + Objects.requireNonNullElse(nullable, -1));
        double sumObj = 0;
        Object[] mixed = {1, 2.5, 3L, 4.5f};
        for (Object x : mixed) sumObj += ((Number) x).doubleValue();
        System.out.println(sumObj + " " + Arrays.toString(mixed));
    }
}
