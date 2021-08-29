import java.util.*;

public class App {

    static int maxFloor = 9;
    static Lift[] lifts;

    public static void main(String[] args) throws Exception {
        createLifts(2, 4); // 2 лифта по 4 человека
        generatePassengers();
        pressEnterToExit();
    }

    // Создание и запуск лифтов
    public static void createLifts(int count, int maxCapacity) {
        lifts = new Lift[count];
        for (int i = 0; i < count; ++i) {
            Lift lift = new Lift(i + 1, maxFloor, maxCapacity);
            lifts[i] = lift;

            Thread thread = new Thread(lift);
            thread.start();
        }
    }

    // Генерация случайных пассажиров
    public static void generatePassengers() {
        Thread generator = new Thread(new Runnable() {
            public void run() {
                Random r = new Random();
                while (true) {
                    Passenger p = new Passenger(r.nextInt(maxFloor) + 1, r.nextInt(maxFloor) + 1,
                            lifts[r.nextInt(lifts.length)]);
                    Thread thread = new Thread(p);
                    thread.start();

                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException e) {
                        return;
                    }
                }
            }
        });
        generator.start();
    }

    // Отдельный поток для прослушивания нажатия клавиатуры
    private static void pressEnterToExit() {
        Thread listener = new Thread(new Runnable() {
            public void run() // Этот метод будет выполняться в побочном потоке
            {
                try {
                    System.in.read();
                } catch (Exception e) {
                }
                System.exit(0);
            }
        });
        listener.start();
    }

}
