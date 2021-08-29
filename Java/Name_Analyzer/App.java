import java.io.*;

import java.util.*;
import java.util.regex.*;

import java.time.*;
import java.time.format.*;

public class App {

    static final String RegExWord = "^[А-Я][а-я]+$|^[A-Z][a-z]+$"; // Русский или транслит
    static final String DateFormat = "dd.MM.uuuu";

    public static void main(String[] args) {
        try (Scanner input = new Scanner(System.in)) {
            System.out.println("Добро пожаловать в приложение \"Name Analysis\"!\n");

            StringBuilder output = new StringBuilder();
            output.append(handleName(input));
            output.append(" ");
            output.append(handleDate(input));

            System.out.println("Результат:");
            System.out.println(output.toString());

        } catch (Exception ex) {
            System.out.println(ex.getMessage());
        }
    }

    static String handleName(Scanner input) {
        StringBuilder output = new StringBuilder();

        label: while (true) {
            System.out.print("ФИО: ");
            String inputString = input.nextLine();

            // Проверка на количество слов
            String[] substrings = inputString.split(" ");
            if (substrings.length != 3) {
                System.out.printf("Ошибка: Слишком много/мало слов. Введено - %d, Необходимо - 3\n", substrings.length);
                System.out.println("Пример: Иванов Иван Иванович\n");
                continue;
            }

            // Проверка каждого слова по регулярному выражению
            for (String word : substrings) {
                if (!Pattern.matches(RegExWord, word)) {
                    System.out.println("Ошибка: Некорректно введено слово -> ");
                    System.out.println(word);
                    System.out.println("Пример: Иванов Иван Иванович\n");
                    continue label;
                }
            }

            String surname = substrings[0];
            String name = substrings[1];
            String patronymic = substrings[2];

            // Фамилия + инициалы
            output.append(surname);
            output.append(" ");
            output.append(name.charAt(0));
            output.append(".");
            output.append(patronymic.charAt(0));
            output.append(". ");

            // Пол
            output.append("Пол: ");
            char last = patronymic.charAt(patronymic.length() - 1);
            char preLast = patronymic.charAt(patronymic.length() - 2);
            if ((preLast == 'и' && last == 'ч') || (preLast == 'c' && last == 'h')) {
                output.append("M");
            } else if ((preLast == 'н' && last == 'а') || (preLast == 'n' && last == 'a')) {
                output.append("Ж");
            } else {
                output.append("?"); // Раньше здесь был рандомный выбор
            }
            break;
        }

        return output.toString();
    }

    static String handleDate(Scanner input) {
        StringBuilder output = new StringBuilder();

        while (true) {
            System.out.print("Дата рождения (дд.мм.гг): ");

            try {
                // Даты
                LocalDate now = LocalDate.now();
                LocalDate birthday = LocalDate.parse(input.nextLine(), DateTimeFormatter.ofPattern(DateFormat));

                // Полных лет
                int year = now.getYear() - birthday.getYear();
                if (now.getMonthValue() < birthday.getMonthValue())
                    year -= 1;
                else if (now.getMonthValue() == birthday.getMonthValue()) {
                    if (now.getDayOfMonth() < birthday.getDayOfMonth())
                        year -= 1;
                }

                // Корректный вывод
                output.append(year);
                char last = output.charAt(output.length() - 1);
                if (last == '1' || last == '2' || last == '3' || last == '4')
                    output.append(" год\n");
                else
                    output.append(" лет\n");

            } catch (DateTimeParseException ex) {
                System.out.println("Ошибка: Некорректно введена дата");
                System.out.println("Пример: 01.04.2021\n");
                continue;
            }
            break;
        }
        return output.toString();
    }
}
