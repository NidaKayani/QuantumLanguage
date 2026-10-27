import java.io.*;
import java.util.*;

public class FastIO {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.out)));
        StringTokenizer st = new StringTokenizer(br.readLine());
        int n = Integer.parseInt(st.nextToken());
        int q = Integer.parseInt(st.nextToken());
        long[] prefix = new long[n + 1];
        st = new StringTokenizer(br.readLine());
        for (int i = 1; i <= n; i++) prefix[i] = prefix[i - 1] + Long.parseLong(st.nextToken());
        for (int k = 0; k < q; k++) {
            st = new StringTokenizer(br.readLine());
            int l = Integer.parseInt(st.nextToken()), r = Integer.parseInt(st.nextToken());
            out.println(prefix[r] - prefix[l - 1]);
        }
        String line;
        List<String> rest = new ArrayList<>();
        while ((line = br.readLine()) != null) {
            if (line.isBlank()) continue;
            rest.add(line.trim());
        }
        Collections.reverse(rest);
        out.printf("%d extra lines: %s%n", rest.size(), rest);
        TreeMap<String, Integer> wc = new TreeMap<>();
        for (String l : rest)
            for (String w : l.split("\\W+"))
                if (!w.isEmpty()) wc.merge(w.toLowerCase(), 1, Integer::sum);
        out.println(wc);
        out.flush();
    }
}
