import java.util.*;
import java.util.stream.Collectors;

public class StudentGrades {
    static class Student implements Comparable<Student> {
        private static int nextId = 1;
        private final int id;
        private final String name;
        private final Map<String, Integer> scores = new LinkedHashMap<>();

        Student(String name) {
            this.id = nextId++;
            this.name = name;
        }

        void addScore(String subject, int score) {
            if (score < 0 || score > 100) throw new IllegalArgumentException("Invalid score " + score + " for " + name);
            scores.put(subject, score);
        }

        double average() {
            return scores.values().stream().mapToInt(Integer::intValue).average().orElse(0);
        }

        char letter() {
            double avg = average();
            if (avg >= 90) return 'A';
            if (avg >= 80) return 'B';
            if (avg >= 70) return 'C';
            if (avg >= 60) return 'D';
            return 'F';
        }

        @Override
        public int compareTo(Student o) {
            return Double.compare(o.average(), average());
        }

        @Override
        public boolean equals(Object o) {
            return o instanceof Student s && s.id == id;
        }

        @Override
        public int hashCode() {
            return Integer.hashCode(id);
        }

        @Override
        public String toString() {
            return String.format("#%d %-6s avg=%6.2f grade=%c %s", id, name, average(), letter(), scores);
        }
    }

    public static void main(String[] args) {
        String[][] raw = {
                {"Alice", "math:95", "physics:88", "art:92"},
                {"Bob", "math:72", "physics:65", "art:80"},
                {"Carol", "math:85", "physics:91", "art:78"},
                {"Dan", "math:55", "physics:61", "art:49"},
                {"Eve", "math:100", "physics:97", "art:120"},
        };
        List<Student> students = new ArrayList<>();
        for (String[] row : raw) {
            Student s = new Student(row[0]);
            try {
                for (int i = 1; i < row.length; i++) {
                    String[] kv = row[i].split(":");
                    s.addScore(kv[0], Integer.parseInt(kv[1]));
                }
            } catch (IllegalArgumentException e) {
                System.out.println("Warning: " + e.getMessage());
            }
            students.add(s);
        }
        Collections.sort(students);
        students.forEach(System.out::println);
        Map<Character, List<String>> byGrade = students.stream()
                .collect(Collectors.groupingBy(Student::letter, TreeMap::new, Collectors.mapping(st -> st.name, Collectors.toList())));
        System.out.println(byGrade);
        OptionalDouble classAvg = students.stream().mapToDouble(Student::average).average();
        System.out.printf("Class average: %.3f%n", classAvg.getAsDouble());
        Set<Student> set = new HashSet<>(students);
        set.add(students.get(0));
        System.out.println("unique students: " + set.size());
        String best = students.stream().filter(st -> st.letter() == 'A').map(st -> st.name).collect(Collectors.joining(" & "));
        System.out.println("Top: " + best);
    }
}
