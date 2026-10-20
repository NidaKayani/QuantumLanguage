import java.util.*;
import java.util.stream.*;

public class Streams {
    record Employee(String name, String dept, double salary, int age) {}

    public static void main(String[] args) {
        List<Integer> nums = List.of(5, 3, 8, 1, 9, 2, 7, 4, 6);
        List<Integer> evens = nums.stream().filter(n -> n % 2 == 0).sorted().collect(Collectors.toList());
        System.out.println(evens);
        System.out.println(nums.stream().map(n -> n * n).filter(n -> n > 20).toList());
        System.out.println(nums.stream().mapToInt(Integer::intValue).sum() + " " + nums.stream().reduce(0, Integer::sum)
                + " " + nums.stream().max(Integer::compare).get() + " " + nums.stream().count());
        System.out.println(IntStream.rangeClosed(1, 5).map(x -> x * x).boxed().collect(Collectors.toList()));
        System.out.println(IntStream.range(0, 5).mapToObj(i -> "i" + i).collect(Collectors.joining(", ", "[", "]")));
        System.out.println(nums.stream().mapToInt(x -> x).average().getAsDouble() + " " + IntStream.of(3, 1, 2).sorted().boxed().toList());
        System.out.println(nums.stream().anyMatch(n -> n > 8) + " " + nums.stream().allMatch(n -> n > 0) + " " + nums.stream().noneMatch(n -> n > 100));
        System.out.println(nums.stream().sorted(Comparator.reverseOrder()).limit(3).toList() + " " + nums.stream().skip(7).toList());
        System.out.println(Stream.of("b", "a", "c", "a").distinct().sorted().collect(Collectors.joining()));
        System.out.println(Stream.iterate(1, x -> x * 2).limit(10).toList());
        List<String> words = List.of("apple", "banana", "avocado", "blueberry", "cherry", "apricot");
        Map<Character, List<String>> byFirst = words.stream().collect(Collectors.groupingBy(w -> w.charAt(0)));
        System.out.println(byFirst);
        Map<Integer, Long> byLen = words.stream().collect(Collectors.groupingBy(String::length, TreeMap::new, Collectors.counting()));
        System.out.println(byLen);
        Map<Boolean, List<String>> parts = words.stream().collect(Collectors.partitioningBy(w -> w.length() > 6));
        System.out.println(parts);
        Map<String, Integer> lengths = words.stream().collect(Collectors.toMap(w -> w, String::length));
        System.out.println(new TreeMap<>(lengths));
        List<Employee> staff = List.of(
                new Employee("Ann", "Eng", 120000, 34),
                new Employee("Bob", "Eng", 95000, 45),
                new Employee("Cid", "Ops", 70000, 29),
                new Employee("Dee", "Ops", 82000, 51),
                new Employee("Eve", "HR", 60000, 38));
        Map<String, Double> avgByDept = staff.stream().collect(Collectors.groupingBy(Employee::dept, TreeMap::new, Collectors.averagingDouble(Employee::salary)));
        System.out.println(avgByDept);
        Optional<Employee> oldest = staff.stream().max(Comparator.comparingInt(Employee::age));
        System.out.println(oldest.map(Employee::name).orElse("none"));
        System.out.println(staff.stream().sorted(Comparator.comparing(Employee::dept).thenComparing(Employee::salary, Comparator.reverseOrder()))
                .map(Employee::name).collect(Collectors.joining(",")));
        DoubleSummaryStatistics stats = staff.stream().mapToDouble(Employee::salary).summaryStatistics();
        System.out.printf("min=%.0f max=%.0f avg=%.1f n=%d%n", stats.getMin(), stats.getMax(), stats.getAverage(), stats.getCount());
        System.out.println(staff.stream().filter(e -> e.age() > 40).map(Employee::name).findFirst().orElse("?"));
        List<List<Integer>> nested = List.of(List.of(1, 2), List.of(3), List.of(4, 5, 6));
        System.out.println(nested.stream().flatMap(List::stream).map(String::valueOf).collect(Collectors.joining("+")));
        System.out.println(Arrays.stream(new String[]{"x", "yy", "zzz"}).mapToInt(String::length).sum());
        StringBuilder sb = new StringBuilder();
        nums.forEach(n -> sb.append(n));
        System.out.println(sb);
        System.out.println(IntStream.rangeClosed(1, 5).reduce(1, (x, y) -> x * y) + " " + LongStream.rangeClosed(1, 20).reduce(1L, (x, y) -> x * y));
        System.out.println(Stream.of(1, 2, 3, 4).collect(Collectors.summingInt(x -> x)) + " " + Stream.of("a", "bb").collect(Collectors.toSet()).size());
    }
}
