import java.util.*;

public class CollectionsMore {
    static class LRU<K, V> extends LinkedHashMap<K, V> {
        private final int cap;
        LRU(int cap) {
            super(16, 0.75f, true);
            this.cap = cap;
        }
        @Override
        protected boolean removeEldestEntry(Map.Entry<K, V> eldest) { return size() > cap; }
    }

    static Map<Character, Integer> dijkstra(Map<Character, Map<Character, Integer>> g, char src) {
        Map<Character, Integer> dist = new TreeMap<>();
        PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[1] != b[1] ? Integer.compare(a[1], b[1]) : Integer.compare(a[0], b[0]));
        pq.add(new int[] {src, 0});
        while (!pq.isEmpty()) {
            int[] cur = pq.poll();
            char u = (char) cur[0];
            if (dist.containsKey(u)) continue;
            dist.put(u, cur[1]);
            for (Map.Entry<Character, Integer> e : g.getOrDefault(u, Map.of()).entrySet())
                if (!dist.containsKey(e.getKey()))
                    pq.add(new int[] {e.getKey(), cur[1] + e.getValue()});
        }
        return dist;
    }

    public static void main(String[] args) {
        Deque<Integer> stack = new ArrayDeque<>();
        Deque<Integer> queue = new LinkedList<>();
        for (int i = 1; i <= 5; i++) {
            stack.push(i);
            queue.offer(i * 10);
        }
        System.out.println(stack + " " + stack.pop() + " " + stack.peek() + " " + queue.poll() + " " + queue.peekLast() + " " + queue);
        TreeMap<Integer, String> tm = new TreeMap<>(Map.of(10, "ten", 20, "twenty", 30, "thirty", 40, "forty"));
        System.out.println(tm.floorKey(25) + " " + tm.ceilingKey(25) + " " + tm.lowerKey(10) + " " + tm.higherEntry(30) + " " + tm.headMap(30) + " " + tm.tailMap(30, false) + " " + tm.firstEntry().getValue() + " " + tm.descendingMap());
        TreeSet<String> ts = new TreeSet<>(Comparator.comparing(String::length).thenComparing(Comparator.reverseOrder()));
        ts.addAll(List.of("pear", "fig", "banana", "kiwi", "apple", "date"));
        System.out.println(ts + " " + ts.first() + " " + ts.pollLast() + " " + ts.headSet("kiwi") + " " + ts.ceiling("zzzz"));
        LRU<String, Integer> lru = new LRU<>(3);
        lru.put("a", 1);
        lru.put("b", 2);
        lru.put("c", 3);
        lru.get("a");
        lru.put("d", 4);
        System.out.println(lru + " " + lru.keySet());
        List<Integer> nums = new ArrayList<>();
        for (int i = 0; i < 10; i++) nums.add(i * i % 7);
        Iterator<Integer> it = nums.iterator();
        while (it.hasNext())
            if (it.next() % 2 == 0) it.remove();
        System.out.println(nums);
        ListIterator<Integer> li = nums.listIterator();
        while (li.hasNext()) {
            int v = li.next();
            if (v == 1) li.set(100);
            else li.add(-v);
        }
        System.out.println(nums);
        try {
            for (Integer n : nums)
                if (n < 0) nums.remove(n);
        } catch (ConcurrentModificationException e) {
            System.out.println("CME");
        }
        try {
            List.of(1, 2).add(3);
        } catch (UnsupportedOperationException e) {
            System.out.println("immutable");
        }
        List<String> fixed = Arrays.asList("x", "y", "z");
        fixed.set(1, "Y");
        try {
            fixed.add("w");
        } catch (UnsupportedOperationException e) {
            System.out.println("fixed size " + fixed);
        }
        List<Integer> big = new ArrayList<>(List.of(1, 2, 3, 4, 5, 6, 7, 8));
        big.subList(2, 5).clear();
        System.out.println(big + " " + Collections.frequency(List.of(1, 2, 1, 1), 1) + " " + Collections.nCopies(3, "ab"));
        Collections.rotate(big, 2);
        Collections.swap(big, 0, big.size() - 1);
        System.out.println(big + " " + Collections.max(big) + " " + Collections.min(big, Comparator.reverseOrder()));
        List<Integer> a = new ArrayList<>(List.of(1, 2, 3, 4, 5)), b = new ArrayList<>(a);
        a.retainAll(List.of(2, 4, 6));
        b.removeAll(List.of(2, 4, 6));
        System.out.println(a + " " + b + " " + Collections.binarySearch(List.of(1, 3, 5, 7), 5) + " " + Collections.binarySearch(List.of(1, 3, 5, 7), 4));
        Map<Character, Map<Character, Integer>> g = new HashMap<>();
        g.put('A', Map.of('B', 4, 'C', 1));
        g.put('C', Map.of('B', 2, 'D', 7));
        g.put('B', Map.of('D', 1));
        System.out.println(dijkstra(g, 'A'));
        PriorityQueue<String> pq = new PriorityQueue<>(Comparator.comparing(String::length).reversed());
        pq.addAll(List.of("a", "ccc", "bb", "dddd"));
        StringBuilder order = new StringBuilder();
        while (!pq.isEmpty()) order.append(pq.poll()).append(' ');
        System.out.println(order.toString().trim());
        Map<String, List<Integer>> multi = new TreeMap<>();
        for (String w : "one two three two one two".split(" "))
            multi.computeIfAbsent(w, k -> new ArrayList<>()).add(w.length());
        System.out.println(multi);
    }
}
