import java.util.*;

public class StringsMore {
    static boolean isPalindrome(String s) {
        String t = s.toLowerCase().replaceAll("[^a-z0-9]", "");
        return new StringBuilder(t).reverse().toString().equals(t);
    }

    static String caesar(String s, int k) {
        StringBuilder sb = new StringBuilder();
        for (char c : s.toCharArray()) {
            if (Character.isUpperCase(c)) sb.append((char) ('A' + (c - 'A' + k) % 26));
            else if (Character.isLowerCase(c)) sb.append((char) ('a' + (c - 'a' + k) % 26));
            else sb.append(c);
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        StringBuilder sb = new StringBuilder("hello");
        sb.insert(0, "[").append(']').setCharAt(1, 'H');
        sb.deleteCharAt(sb.length() - 1).append(" world").replace(0, 1, "<<");
        System.out.println(sb + " " + sb.length() + " " + sb.indexOf("o") + " " + sb.charAt(3));
        System.out.println(String.format("|%5d|%-5d|%05d|%x|%X|%o|%,d|", 42, 42, 42, 255, 255, 8, 1234567));
        System.out.println(String.format("|%8.3f|%-8.2f|%.0f|%e|%.2e|%10s|%-10s|", Math.PI, 2.5, 2.5, 12345.678, 0.000123, "right", "left"));
        System.out.println(String.format("%b %c %s %%  %n", true, 'x', null).trim());
        System.out.println(Arrays.toString("a,b,,c,,".split(",")) + " " + "a,b,,c,,".split(",").length + " " + Arrays.toString("a,b,,c,,".split(",", -1)));
        System.out.println(Arrays.toString("one  two   three".split("\\s+")) + " " + Arrays.toString("2024-01-15".split("-", 2)));
        System.out.println(String.join("|", List.of("x", "y", "z")) + " " + "ab".repeat(3) + " [" + "  pad  ".strip() + "] [" + "  pad  ".trim() + "]");
        System.out.println("Mississippi".chars().filter(c -> c == 's').count() + " " + "Mississippi".indexOf("ss", 3) + " " + "Mississippi".lastIndexOf('i'));
        String text = """
            Hello,
              Text Block!
            End""";
        System.out.println(text);
        String cmd = "stop";
        switch (cmd) {
            case "go" -> System.out.println("going");
            case "stop", "halt" -> System.out.println("stopping");
            default -> System.out.println("?");
        }
        System.out.println("apple".compareTo("banana") + " " + "b".compareTo("a") + " " + "abc".compareTo("abcd") + " " + "ABC".equalsIgnoreCase("abc"));
        char[] cs = "dcba".toCharArray();
        Arrays.sort(cs);
        System.out.println(new String(cs) + " " + String.valueOf(cs, 1, 2) + " " + isPalindrome("A man, a plan, a canal: Panama") + " " + isPalindrome("Java"));
        System.out.println(caesar("Hello, World!", 3) + " " + caesar(caesar("Hello, World!", 3), 23));
        char c = 'A';
        c += 2;
        int code = c + 1;
        System.out.println(c + " " + code + " " + (char) code + " " + (int) 'z' + " " + Character.getNumericValue('7') + " " + Character.isLetterOrDigit('_'));
        System.out.println("x=" + 1 + 2 + " " + (1 + 2) + "y" + 'a' + 'b' + " " + ('a' + 'b') + " " + (char) ('a' + 1));
        System.out.println("phone: 555-1234".replaceAll("(\\d{3})-(\\d{4})", "$2-$1") + " " + "a.b.c".replace(".", "::") + " " + "tab\there".contains("\t"));
        String s = null;
        System.out.println("null concat: " + s + " " + Objects.requireNonNullElse(s, "default") + " " + String.valueOf((Object) null));
        System.out.println("Hello".substring(1, 4) + " " + "Hello".charAt(0) + "Hello".charAt(4) + " " + "HELLO".toLowerCase().toUpperCase() + " " + "".isEmpty() + " " + "  ".isBlank());
        System.out.println(String.valueOf(3.0) + String.valueOf(true) + String.valueOf('c') + String.valueOf(10L) + " " + Integer.toString(255, 16) + " " + Integer.parseInt("-123") + " " + Double.parseDouble("1e3"));
    }
}
