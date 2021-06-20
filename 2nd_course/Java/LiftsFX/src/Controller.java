import javafx.animation.TranslateTransition;
import javafx.fxml.FXML;

import javafx.scene.text.*;
import javafx.scene.control.*;
import javafx.scene.image.ImageView;
import javafx.scene.layout.AnchorPane;

import javafx.util.Duration;

public class Controller {

    // Контекстное меню
    public MenuItem menu_start;

    // Первый подъезд
    public AnchorPane root1_elevator;
    public Text root1_elevator_text;
    public AnchorPane root1_floor1, root1_floor2, root1_floor3;
    public AnchorPane root1_floor4, root1_floor5, root1_floor6;
    // public ImageView root1_floor1_man1, root1_floor1_man2, root1_floor1_man3,
    // root1_floor1_man4;
    // public ImageView root1_floor2_man1, root1_floor2_man2, root1_floor2_man3,
    // root1_floor2_man4;
    // public ImageView root1_floor3_man1, root1_floor3_man2, root1_floor3_man3,
    // root1_floor3_man4;
    // public ImageView root1_floor4_man1, root1_floor4_man2, root1_floor4_man3,
    // root1_floor4_man4;
    // public ImageView root1_floor5_man1, root1_floor5_man2, root1_floor5_man3,
    // root1_floor5_man4;
    // public ImageView root1_floor6_man1, root1_floor6_man2, root1_floor6_man3,
    // root1_floor6_man4;

    // Второй подъезд
    public AnchorPane root2_elevator;
    public Text root2_elevator_text;
    public AnchorPane root2_floor1, root2_floor2, root2_floor3;
    public AnchorPane root2_floor4, root2_floor5, root2_floor6;

    @FXML
    private void buttonClicked() {

    }

    public void updateLift(Lift lift) {
        AnchorPane elevator = root1_elevator;
        AnchorPane floor = root1_floor1;

        if (lift.id == 1) {
            root1_elevator_text.setText(String.format("%d/%d", lift.currentCapacity, lift.maxCapacity));
            elevator = root1_elevator;
            if (lift.currentFloor == 1)
                floor = root1_floor1;
            else if (lift.currentFloor == 2)
                floor = root1_floor2;
            else if (lift.currentFloor == 3)
                floor = root1_floor3;
            else if (lift.currentFloor == 4)
                floor = root1_floor4;
            else if (lift.currentFloor == 5)
                floor = root1_floor5;
            else if (lift.currentFloor == 6)
                floor = root1_floor6;
        } else if (lift.id == 2) {
            elevator = root2_elevator;
            root2_elevator_text.setText(String.format("%d/%d", lift.currentCapacity, lift.maxCapacity));
            if (lift.currentFloor == 1)
                floor = root2_floor1;
            else if (lift.currentFloor == 2)
                floor = root2_floor2;
            else if (lift.currentFloor == 3)
                floor = root2_floor3;
            else if (lift.currentFloor == 4)
                floor = root2_floor4;
            else if (lift.currentFloor == 5)
                floor = root2_floor5;
            else if (lift.currentFloor == 6)
                floor = root2_floor6;
        }

        long time = 1000;
        TranslateTransition transition = new TranslateTransition(Duration.millis(time), elevator);
        double diff = floor.getBoundsInParent().getCenterY() - elevator.getBoundsInParent().getCenterY();
        transition.setByY(diff);
        transition.play();
        try {
            Thread.sleep(time);
        } catch (Exception e) {
            System.out.println(e);
        }
    }

}
