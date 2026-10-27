import java.util.*;
import java.util.function.*;
import java.util.stream.*;

public class StreamsMore {
    record Person(String name, int age, String city) {}

    public static void main(String[] args) {
        System.out.println(IntStream.rangeClosed(1, 10).filter(i -> i % 3 != 0).map(i -> i * i).sum());
        System.out.println(IntStream.range(0, 5).boxed().collect(Collectors.toList()) + " " + IntStream.of(3, 1, 2).sorted().boxed().toList());
        System.out.println(IntStream.rangeClosed(1, 5).average().getAsDouble() + " " + IntStream.empty().average().isPresent() + " " + IntStream.of(4, 9, 2).max().getAsInt());
        List<List<Integer>> nested = List.of(List.of(1, 2), List.of(3), List.of(), List.of(4, 5, 6));
        System.out.println(nested.stream().flatMap(List::stream).map(x -> x * 10).collect(Collectors.toList()));
        System.out.println(Stream.of("x", "y", "z").collect(Collectors.joining(", ", "<", ">")) + " " + Stream.of("a", "b").collect(Collectors.joining()));
        List<Person> people = List.of(
            new Person("Ann", 34, "Oslo"), new Person("Bob", 25, "Rome"), new Person("Cid", 41, "Oslo"),
            new Person("Dee", 25, "Paris"), new Person("Eve", 38, "Rome"), new Person("Fay", 29, "Oslo"));
        Map<String, Long> perCity = people.stream().collect(Collectors.groupingBy(Person::city, TreeMap::new, Collectors.counting()));
        System.out.println(perCity);
        Map<Boolean, Long> over30 = people.stream().collect(Collectors.partitioningBy(p -> p.age() > 30, Collectors.counting()));
        System.out.println(over30);
        Map<String, Integer> ageSum = people.stream().collect(Collectors.toMap(Person::city, Person::age, Integer::sum, LinkedHashMap::new));
        System.out.println(ageSum);
        IntSummaryStatistics st = people.stream().mapToInt(Person::age).summaryStatistics();
        System.out.println(st.getMin() + " " + st.getMax() + " " + st.getAverage() + " " + st.getSum() + " " + st.getCount());
        System.out.println(st);
        System.out.println(people.stream().map(Person::name).reduce("", (acc, n) -> acc + n.charAt(0)) + " " + people.stream().map(Person::age).reduce(Integer::max).get() + " " + Stream.of(1, 2, 3, 4).reduce(0, (x, y) -> x + y, Integer::sum));
        System.out.println(Stream.iterate(1, x -> x * 3).limit(6).collect(Collectors.toList()) + " " + Stream.iterate(1, x -> x < 50, x -> x * 2).toList());
        System.out.println(Stream.of(5, 3, 5, 1, 3, 8).distinct().sorted(Comparator.reverseOrder()).toList());
        List<String> trace = new ArrayList<>();
        List<Integer> res = Stream.of(1, 2, 3, 4, 5).peek(x -> trace.add("p" + x)).filter(x -> x % 2 == 1).map(x -> {
            trace.add("m" + x);
            return x * 100;
        }).limit(2).collect(Collectors.toList());
        System.out.println(res + " " + trace);
        System.out.println(people.stream().anyMatch(p -> p.city().equals("Paris")) + " " + people.stream().allMatch(p -> p.age() >= 25) + " " + people.stream().noneMatch(p -> p.name().isEmpty()));
        Optional<Person> firstRoman = people.stream().filter(p -> p.city().equals("Rome")).findFirst();
        System.out.println(firstRoman.map(Person::name).orElse("none") + " " + people.stream().filter(p -> p.age() > 100).findAny().map(Person::name).orElseGet(() -> "nobody"));
        System.out.println(Optional.of(5).filter(x -> x > 3).map(x -> x * 2).orElse(0) + " " + Optional.empty().isPresent() + " " + Optional.ofNullable(null).or(() -> Optional.of("alt")).get());
        System.out.println(Stream.of(1, 2, 3, 10, 4, 1).takeWhile(x -> x < 5).toList() + " " + Stream.of(1, 2, 3, 10, 4, 1).dropWhile(x -> x < 5).toList());
        System.out.println(IntStream.range(0, 4).mapToObj(i -> "#" + i).collect(Collectors.joining(" ")) + " " + Arrays.stream(new int[] {7, 2, 9}).max().getAsInt() + " " + Arrays.stream(new String[] {"b", "a"}).sorted().toList());
        Map<Integer, List<String>> byAge = people.stream().collect(Collectors.groupingBy(Person::age, TreeMap::new, Collectors.mapping(Person::name, Collectors.toList())));
        System.out.println(byAge);
        System.out.println(people.stream().sorted(Comparator.comparing(Person::city).thenComparing(Person::age, Comparator.reverseOrder())).map(Person::name).toList());
        System.out.println(people.stream().collect(Collectors.averagingInt(Person::age)) + " " + people.stream().mapToDouble(Person::age).sum() + " " + DoubleStream.of(0.1, 0.2, 0.3).sum());
        Function<Integer, Integer> inc = x -> x + 1, dbl = x -> x * 2;
        BinaryOperator<Integer> mul = (x, y) -> x * y;
        Supplier<List<String>> sup = ArrayList::new;
        List<String> made = sup.get();
        made.add("made");
        System.out.println(inc.andThen(dbl).apply(3) + " " + inc.compose(dbl).apply(3) + " " + mul.apply(6, 7) + " " + made + " " + Function.<Integer>identity().apply(9));
        System.out.println(Stream.of("bb", "a", "ccc").collect(Collectors.toMap(s -> s, String::length)).entrySet().stream().sorted(Map.Entry.comparingByValue()).toList());
        System.out.println(LongStream.rangeClosed(1, 20).reduce(1L, (x, y) -> x * y) + " " + IntStream.iterate(1, i -> i * 2).limit(11).sum());
        String sentence = "the quick brown fox jumps over the lazy dog the end";
        Map<String, Long> freq = Arrays.stream(sentence.split(" ")).collect(Collectors.groupingBy(w -> w, Collectors.counting()));
        System.out.println(freq);
        System.out.println(freq.entrySet().stream().sorted(Map.Entry.<String, Long>comparingByValue().reversed().thenComparing(Map.Entry.comparingByKey())).limit(3).map(e -> e.getKey() + "=" + e.getValue()).collect(Collectors.joining(",")));
    }
}
