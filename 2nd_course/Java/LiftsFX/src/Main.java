import java.util.Random;

public class Main {

    public static void main(String[] args) {
        int max_floor = 6;

        GUI gui = new GUI();
        Thread GUI_Thread = new Thread(gui);
        GUI_Thread.start();

        Lift lift1 = new Lift(1, max_floor, 4, GUI.controller);
        Thread lift1_Thread = new Thread(lift1);
        lift1_Thread.start();

        Lift lift2 = new Lift(2, max_floor, 4, GUI.controller);
        Thread lift2_Thread = new Thread(lift2);
        lift2_Thread.start();

        Random r = new Random();
        while (true) {
            Lift lift = lift1;
            if (r.nextInt(100) % 2 == 0)
                lift = lift2;
            Passenger passenger = new Passenger(r.nextInt(max_floor) + 1, r.nextInt(max_floor) + 1, lift,
                    GUI.controller);
            Thread passenger_Thread = new Thread(passenger);
            passenger_Thread.start();

            try {
                Thread.sleep(1500);
            } catch (InterruptedException e) {
                return;
            }
        }
    }

}
