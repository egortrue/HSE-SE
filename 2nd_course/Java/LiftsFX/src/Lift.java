import java.util.*;

public class Lift implements Runnable {
    int id;

    int maxFloor;
    public int currentFloor;

    // Вместимость (человек)
    int maxCapacity;
    int currentCapacity;

    // Очередь посещения этажей
    LinkedList<Integer> queue;

    Controller controller;

    Lift(int id, int maxFloor, int maxCapacity, Controller controller) {
        this.id = id;
        this.maxFloor = maxFloor;
        this.maxCapacity = maxCapacity;
        this.controller = controller;

        currentFloor = 6;
        currentCapacity = 0;
        queue = new LinkedList<Integer>();
    }

    public void run() {
        while (true) {
            if (queue.isEmpty()) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    return;
                }
            }
            move();
        }
    }

    public synchronized boolean addPassenger() {
        if (currentCapacity == maxCapacity)
            return false;

        System.out.printf("№%d | пассажир вошел\n", id);
        currentCapacity++;
        return true;
    }

    public synchronized boolean removePassenger() {
        if (currentCapacity == 0)
            return false;

        System.out.printf("№%d | пассажир вышел\n", id);
        currentCapacity--;
        return true;
    }

    // Кнопка вызова лифта на этаже
    public synchronized void pressExternalButton(int floor) {
        System.out.printf("№%d | внешний вызов: %d этаж\n", id, floor);

        if (queue.isEmpty()) {
            queue.addFirst(floor);
            return;
        }

        // Вызов полностью загруженного лифта - минимальный приоритет
        if (currentCapacity == maxCapacity) {
            queue.addLast(floor);
            return;
        }

        queueInsert(floor);
    }

    // Кнопка отправки лифта на этаж
    public synchronized void pressInternalButton(int floor) {
        if (currentCapacity == 0)
            return;

        System.out.printf("№%d | внутренний вызов: %d этаж\n", id, floor);
        if (queue.isEmpty()) {
            queue.addFirst(floor);
            return;
        }

        queueInsert(floor);
    }

    public void move() {
        if (queue.isEmpty())
            return;

        if (currentFloor == queue.peekFirst()) {
            queue.removeFirst();
            return;
        }

        System.out.printf("№%d | передвижение: %d -> ", id, currentFloor);
        if (currentFloor < queue.peekFirst())
            currentFloor++;
        else if (currentFloor > queue.peekFirst())
            currentFloor--;
        System.out.printf("%d (%d)\n", currentFloor, queue.peekFirst());

        controller.updateLift(this);
    }

    private synchronized void queueInsert(int newFloor) {
        int newPriority = Math.abs(currentFloor - newFloor);

        int index = 0;
        for (Integer floor : queue) {
            int currentPriority = Math.abs(currentFloor - floor);
            if (newPriority < currentPriority)
                break;
            index++;
        }

        queue.add(index, newFloor);
    }

}
