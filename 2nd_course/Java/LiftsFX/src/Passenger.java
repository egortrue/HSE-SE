public class Passenger implements Runnable {
    int source;
    int destination;
    Lift lift;

    Passenger(int source, int destination, Lift lift) {
        this.source = source;
        this.destination = destination;
        this.lift = lift;
    }

    public void run() {

        // Вызываем лифт
        lift.pressExternalButton(source);

        // Ждём лифт
        while (lift.currentFloor != source) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                return;
            }
        }

        // Пытаемся войти в лифт
        while (!lift.addPassenger()) {
            try {
                // Ждём освобождения лифта
                Thread.sleep(2000);
                // Заново вызываем лифт
                lift.pressExternalButton(source);
            } catch (InterruptedException e) {
                return;
            }
        }

        // Вошли в лифт
        lift.pressInternalButton(destination);

        // Едем на лифте
        while (lift.currentFloor != destination) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                return;
            }
        }

        // Выходим из лифта
        lift.removePassenger();
    }

}
