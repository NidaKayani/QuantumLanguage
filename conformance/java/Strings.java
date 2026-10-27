import java.util.Arrays;

public class Strings {
    public static void main(String[] args) {
        String s = "Hello, World";
        System.out.println(s.length() + " " + s.charAt(4) + " " + s.indexOf('o') + " " + s.lastIndexOf("o") + " " + s.indexOf("xyz"));
        System.out.println(s.substring(7) + "|" + s.substring(0, 5) + "|" + s.toUpperCase() + "|" + s.toLowerCase());
        System.out.println(s.contains("World") + " " + s.startsWith("Hell") + " " + s.endsWith("x") + " " + s.isEmpty() + " " + "".isEmpty());
        System.out.println("  padded  ".trim() + "|" + "  x ".strip() + "|" + "ab".repeat(3) + "|" + "   ".isBlank());
        System.out.println(s.replace('l', 'L') + " " + s.replace("World", "Java") + " " + "a1b2c3".replaceAll("[0-9]", "#"));
        String[] parts = "red,green,,blue".split(",");
        System.out.println(parts.length + " " + Arrays.toString(parts) + " " + Arrays.toString("a b  c".split("\\s+")));
        System.out.println(String.join("-", "x", "y", "z") + " " + String.join("+", Arrays.asList("1", "2")));
        System.out.println("apple".compareTo("banana") + " " + "b".compareTo("a") + " " + "abc".compareTo("abcd") + " " + "ABC".equalsIgnoreCase("abc"));
        String a = "hello";
        String b = "hel" + "lo";
        String c = new String("hello");
        System.out.println(a.equals(c) + " " + (a == b) + " " + a.hashCode() + " " + "".hashCode() + " " + "Java".hashCode());
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 5; i++) sb.append(i).append(',');
        sb.setLength(sb.length() - 1);
        sb.insert(0, "[").append("]");
        System.out.println(sb + " " + sb.length() + " " + sb.reverse() + " " + sb.charAt(0));
        StringBuilder sb2 = new StringBuilder("abcdef");
        sb2.deleteCharAt(2).setCharAt(0, 'Z');
        sb2.delete(1, 3);
        System.out.println(sb2.toString() + " " + sb2.indexOf("e") + " " + new StringBuilder("racecar").reverse().toString().equals("racecar"));
        System.out.println(String.valueOf(42) + String.valueOf(3.5) + String.valueOf(true) + String.valueOf('c') + String.valueOf((Object) null));
        char[] chars = s.toCharArray();
        Arrays.sort(chars);
        System.out.println(new String(chars).trim() + " " + String.valueOf(chars, 0, 3).length() + " " + chars.length);
        int vowels = 0;
        for (char ch : s.toLowerCase().toCharArray()) if ("aeiou".indexOf(ch) >= 0) vowels++;
        System.out.println("vowels=" + vowels);
        System.out.println(Character.isDigit('5') + " " + Character.isLetter('x') + " " + Character.isUpperCase('a') + " " + Character.toUpperCase('q') + " " + Character.isWhitespace(' ') + " " + Character.isLetterOrDigit('_'));
        String text = """
            Text blocks
              keep relative indent
            and "quotes"\tok
            """;
        System.out.print(text);
        System.out.println("null concat: " + null + " " + (String) null);
        String n = null;
        System.out.println("a" + 1 + 2 + " " + 1 + 2 + "b" + " " + 'x' + 1 + " " + n);
        System.out.println("chars: " + "abc".chars().sum() + " " + "hello".chars().filter(ch -> ch == 'l').count());
        System.out.println("Hello".matches("H.*o") + " " + "x1y2".replaceFirst("\\d", "_") + " " + "Mississippi".indexOf("ss", 3));
        String multi = "line1\nline2\nline3";
        System.out.println(multi.lines().count() + " " + multi.split("\n")[1] + " " + "%d items".formatted(3));
        System.out.println("tab\tquote\"back\\slash" + " unicode:Aé");
    }
}
