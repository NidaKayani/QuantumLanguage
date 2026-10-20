import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class ReaderInput {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int t = Integer.parseInt(br.readLine().trim());
        StringBuilder out = new StringBuilder();
        for (int i = 0; i < t; i++) {
            StringTokenizer st = new StringTokenizer(br.readLine());
            long a = Long.parseLong(st.nextToken());
            long b = Long.parseLong(st.nextToken());
            out.append(a * b).append('\n');
        }
        System.out.print(out);
        String line;
        int lines = 0;
        while ((line = br.readLine()) != null) {
            if (line.isEmpty()) continue;
            lines++;
            System.out.println(lines + ": " + new StringBuilder(line).reverse());
        }
    }
}
