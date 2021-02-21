package task1;
import java.util.Random;

public class Main {

    public static void main(String[] args) {

        //======================================================================
        // Инстанцирование комплексных чисел

        Complex zero = new Complex();
        System.out.println("Complex number:");
        System.out.println("\tStandard view: " + zero);
        System.out.println("\tTrigonometric view: " + zero.toTrigonometric());
        System.out.println("\n");

        Complex a = new Complex(5, 5);
        System.out.println("Complex number:");
        System.out.println("\tStandard view: " + a);
        System.out.println("\tTrigonometric view: " + a.toTrigonometric());
        System.out.println("\n");

        Complex b = new Complex(2, -3);
        System.out.println("Complex number:");
        System.out.println("\tStandard view: " + b);
        System.out.println("\tTrigonometric view: " + b.toTrigonometric());
        System.out.println("\n");

        //======================================================================
        // Арифметические операции с комплексными числами

        System.out.println("Addition:");
        System.out.println(String.format("(%s) + (%s) = (%s)", a, b, a.add(b)));
        System.out.println("\n");

        System.out.println("Subtraction:");
        System.out.println(String.format("(%s) - (%s) = (%s)", a, b, a.sub(b)));
        System.out.println("\n");

        System.out.println("Multiplication:");
        System.out.println(String.format("(%s) * (%s) = (%s)", a, b, a.mul(b)));
        System.out.println("\n");

        System.out.println("Division:");
        System.out.println(String.format("(%s) / (%s) = (%s)", a, b, a.div(b)));
        System.out.println("\n");

        //======================================================================
        // Данные для заполнения матриц

        // Поддержка обычных и двумерных массивов
        // c типами: int, double, Complex
        int size = 3;
        int[] data1 = new int[size*size];
        double[] data2 = new double[size*size];
        Complex[][] data3 = new Complex[size][size];

        Random r = new Random();
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) {
                data1[i * size + j] = r.nextInt(10);
                data2[i * size + j] = r.nextDouble() * 10;
                data3[i][j] = new Complex(r.nextDouble() * 10, r.nextDouble() * 10);
            }

        //======================================================================
        // Инстанцирование матриц

        Matrix mat1 = new Matrix(size);
        mat1.setData(data1);
        System.out.println("Matrix:");
        System.out.println(mat1);
        System.out.println("\n");

        Matrix mat2 = new Matrix(size);
        mat2.setData(data2);
        System.out.println("Matrix:");
        System.out.println(mat2);
        System.out.println("\n");

        Matrix mat3 = new Matrix(size);
        mat3.setData(data3);
        System.out.println("Matrix:");
        System.out.println(mat3);
        System.out.println("\n");

        //======================================================================
        // Операции с матрицами

        // Сложение/вычитание матриц
        // Умножение на число
        // Умножение матриц
        // Транспонирование

        System.out.println("Addition:");
        System.out.println(String.format("%s\n+\n%s\n=\n%s", mat1, mat2, mat1.add(mat2)));
        System.out.println("\n");

        System.out.println("Subtraction:");
        System.out.println(String.format("%s\n-\n%s\n=\n%s", mat1, mat2, mat1.sub(mat2)));
        System.out.println("\n");

        System.out.println("Multiplication on number:");
        System.out.println(String.format("%s\n*\n%s\n=\n%s", 3, mat1, mat1.mul(3)));
        System.out.println("\n");

        System.out.println("Multiplication:");
        System.out.println(String.format("%s\n*\n%s\n=\n%s", mat1, mat2, mat1.mul(mat2)));
        System.out.println("\n");

        System.out.println("Transposing:");
        System.out.println(String.format("%s\nT=\n%s", mat1, mat1.transpose()));
        System.out.println("\n");

    }
}
