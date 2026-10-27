import java.util.*;

public class Collections1 {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<>(List.of(5, 3, 8, 1, 9, 2));
        list.add(7);
        list.add(0, 4);
        list.remove(Integer.valueOf(8));
        list.remove(0);
        System.out.println(list + " size=" + list.size() + " get(2)=" + list.get(2) + " contains9=" + list.contains(9));
        Collections.sort(list);
        System.out.println(list + " max=" + Collections.max(list) + " min=" + Collections.min(list) + " idx=" + list.indexOf(7));
        list.sort(Comparator.reverseOrder());
        System.out.println(list);
        Collections.reverse(list);
        list.set(0, 100);
        System.out.println(list + " sub=" + list.subList(1, 3) + " freq=" + Collections.frequency(list, 9));
        list.removeIf(x -> x % 2 == 0);
        System.out.println(list + " empty=" + list.isEmpty());

        Map<String, Integer> counts = new HashMap<>();
        String text = "the quick brown fox jumps over the lazy dog the fox";
        for (String w : text.split(" ")) counts.put(w, counts.getOrDefault(w, 0) + 1);
        System.out.println(counts);
        System.out.println(counts.get("the") + " " + counts.get("cat") + " " + counts.containsKey("fox") + " " + counts.size());
        Map<Character, Integer> freq = new HashMap<>();
        for (char ch : "mississippi".toCharArray()) freq.merge(ch, 1, Integer::sum);
        System.out.println(freq);
        Map<Integer, String> squares = new HashMap<>();
        for (int i = 20; i >= 0; i -= 4) squares.put(i * i, "sq" + i);
        System.out.println(squares.keySet() + " " + squares.values());
        TreeMap<String, Integer> sorted = new TreeMap<>(counts);
        System.out.println(sorted + " first=" + sorted.firstKey() + " last=" + sorted.lastEntry());
        System.out.println(sorted.headMap("fox") + " " + sorted.tailMap("quick") + " " + sorted.ceilingKey("g") + " " + sorted.floorKey("g"));
        Map<String, List<Integer>> groups = new LinkedHashMap<>();
        for (int i = 1; i <= 10; i++) groups.computeIfAbsent(i % 3 == 0 ? "three" : i % 2 == 0 ? "even" : "odd", k -> new ArrayList<>()).add(i);
        for (Map.Entry<String, List<Integer>> e : groups.entrySet()) System.out.println(e.getKey() + " -> " + e.getValue());
        counts.putIfAbsent("cat", 9);
        counts.remove("dog");
        counts.replaceAll((k, v) -> v * 10);
        System.out.println(new TreeMap<>(counts));

        Set<String> set = new HashSet<>(Arrays.asList("banana", "apple", "cherry", "apple", "date"));
        System.out.println(set + " " + set.size() + " " + set.contains("apple"));
        Set<Integer> nums = new HashSet<>();
        for (int v : new int[]{42, 7, 99, 7, 15, 1000, 3}) nums.add(v);
        System.out.println(nums);
        Set<Integer> tree = new TreeSet<>(nums);
        System.out.println(tree + " " + ((TreeSet<Integer>) tree).first() + " " + ((TreeSet<Integer>) tree).headSet(15));
        Set<String> linked = new LinkedHashSet<>(List.of("z", "a", "m"));
        linked.add("a");
        System.out.println(linked);
        Set<Integer> a = new TreeSet<>(List.of(1, 2, 3, 4));
        Set<Integer> b = new TreeSet<>(List.of(3, 4, 5));
        Set<Integer> union = new TreeSet<>(a);
        union.addAll(b);
        Set<Integer> inter = new TreeSet<>(a);
        inter.retainAll(b);
        Set<Integer> diff = new TreeSet<>(a);
        diff.removeAll(b);
        System.out.println(union + " " + inter + " " + diff);

        Deque<Integer> stack = new ArrayDeque<>();
        stack.push(1);
        stack.push(2);
        stack.push(3);
        System.out.println(stack.pop() + " " + stack.peek() + " " + stack);
        Queue<String> queue = new LinkedList<>();
        queue.offer("a");
        queue.offer("b");
        queue.add("c");
        System.out.println(queue.poll() + " " + queue.peek() + " " + queue.size() + " " + queue);
        PriorityQueue<Integer> pq = new PriorityQueue<>(List.of(5, 1, 8, 3, 9, 2));
        System.out.println(pq);
        StringBuilder order = new StringBuilder();
        while (!pq.isEmpty()) order.append(pq.poll()).append(' ');
        System.out.println(order.toString().trim());
        PriorityQueue<String> maxHeap = new PriorityQueue<>(Comparator.comparing(String::length).reversed());
        maxHeap.addAll(List.of("kiwi", "fig", "banana", "plum"));
        System.out.println(maxHeap.poll() + " " + maxHeap.poll());
        Stack<Character> st = new Stack<>();
        for (char ch : "abc".toCharArray()) st.push(ch);
        System.out.println(st + " " + st.pop() + st.peek() + " " + st.search('a'));
        Iterator<Integer> it = new ArrayList<>(List.of(1, 2, 3, 4)).iterator();
        int s = 0;
        while (it.hasNext()) {
            int v = it.next();
            if (v % 2 == 0) s += v;
        }
        System.out.println("even sum " + s);
        List<String> immutable = List.of("x");
        try {
            immutable.add("y");
        } catch (UnsupportedOperationException e) {
            System.out.println("immutable list");
        }
        List<Integer> copy = new ArrayList<>(List.of(1, 2, 3));
        try {
            for (Integer v : copy) if (v == 2) copy.remove(v);
        } catch (ConcurrentModificationException e) {
            System.out.println("CME caught");
        }
        Collections.shuffle(copy, new Random(42));
        System.out.println(copy + " " + new Random(7).nextInt(100) + " " + new Random(7).nextDouble());
        System.out.println(Collections.nCopies(3, "ab") + " " + Collections.emptyList() + " " + Collections.singletonList(1));
    }
}
