import javafx.application.Application;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.fxml.FXMLLoader;

public class GUI extends Application implements Runnable {

    static public Controller controller = new Controller();

    @Override
    public void start(Stage primaryStage) throws Exception {
        FXMLLoader loader = new FXMLLoader();
        loader.setLocation(getClass().getResource("/fxml/simple.fxml"));
        loader.setController(controller);
        Parent root = loader.load();

        primaryStage.setTitle("LiftsFX");
        primaryStage.setScene(new Scene(root, 800, 625));
        primaryStage.setResizable(false);
        primaryStage.show();
    }

    @Override
    public void run() {
        launch();
        System.exit(0);
    }
}
