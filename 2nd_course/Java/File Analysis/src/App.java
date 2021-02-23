import java.util.*;

public class App {
    public static void main(String[] args) {
        try (Scanner input = new Scanner(System.in)) {
            System.out.print("Input a file name: ");
            FileAnalyser analysis = new FileAnalyser(input.nextLine(), "data/result.txt");
            analysis.run();
        }
        catch (Exception ex) {
            System.out.println(ex.getMessage());
        } 
    }
}
