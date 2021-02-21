
public class App {
    public static void main(String[] args) {
        FileAnalyser analysis = new FileAnalyser("data/lorem.txt", "data/result.txt");
        analysis.run();
    }
}
