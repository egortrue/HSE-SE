package task1;
import java.util.Random;

// Хотел использовать шаблонный класс, но не удалось его правильно реализовать
// Довольно таки сильно отличается от того, как это делается в c++
public class Matrix {
    private int size;
    private Complex[][] data;

    Matrix(int size) {
        this.size = size;
        this.data = new Complex[size][size];
    }

    Matrix() {
        this(1);
    }

    private boolean isConvenient(int size) {
        if (this.size == size)
            return true;
        System.out.println("Sizes of data are different!");
        return false;
    }

    public void setData(int[] data) {
        if (!this.isConvenient((int)Math.sqrt(data.length))) return;
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                this.data[row][col] = new Complex(data[row * size + col]);
        }
    }

    public void setData(int[][] data) {
        if (!this.isConvenient(data.length)) return;
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                this.data[row][col] = new Complex(data[row][col]);
        }
    }

    public void setData(double[] data) {
        if (!this.isConvenient((int)Math.sqrt(data.length))) return;
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                this.data[row][col] = new Complex(data[row * size + col]);
        }
    }

    public void setData(double[][] data) {
        if (!this.isConvenient(data.length)) return;
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                this.data[row][col] = new Complex(data[row][col]);
        }
    }

    public void setData(Complex[] data) {
        if (!this.isConvenient((int)Math.sqrt(data.length))) return;
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                this.data[row][col] = new Complex(data[row * size + col].getRe(), data[row * size + col].getIm());
        }
    }

    public void setData(Complex[][] data) {
        if (!this.isConvenient(data.length)) return;
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                this.data[row][col] = new Complex(data[row][col].getRe(), data[row][col].getIm());
        }
    }

    public Matrix add(Matrix other) {
        if(!this.isConvenient(other.size)) return null;

        Complex[][] newData = new Complex[size][size];
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                newData[row][col] = this.data[row][col].add(other.data[row][col]);
        }

        Matrix newMatrix = new Matrix(this.size);
        newMatrix.setData(newData);

        return newMatrix;
    }

    public Matrix sub(Matrix other) {
        if(!this.isConvenient(other.size)) return null;

        Complex[][] newData = new Complex[size][size];
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                newData[row][col] = this.data[row][col].sub(other.data[row][col]);
        }

        Matrix newMatrix = new Matrix(this.size);
        newMatrix.setData(newData);

        return newMatrix;
    }

    public Matrix mul(double num) {

        Complex[][] newData = new Complex[size][size];
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                newData[row][col] = this.data[row][col].mul(num);
        }

        Matrix newMatrix = new Matrix(this.size);
        newMatrix.setData(newData);

        return newMatrix;
    }

    public Matrix mul(Matrix other) {
        if(!this.isConvenient(other.size)) return null;

        Complex[][] newData = new Complex[size][size];
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++) {

                Complex el = new Complex();
                for (int i = 0; i < this.size; i++)
                    el = el.add(this.data[row][i].mul(other.data[i][col]));

                newData[row][col] = el;
            }
        }

        Matrix newMatrix = new Matrix(this.size);
        newMatrix.setData(newData);

        return newMatrix;
    }

    public Matrix transpose() {
        Complex[][] newData = new Complex[size][size];
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++)
                newData[row][col] = this.data[col][row];
        }

        Matrix newMatrix = new Matrix(this.size);
        newMatrix.setData(newData);

        return newMatrix;
    }

    @Override
    public String toString() {
        String res = "";
        for (int row = 0; row < this.size; row++) {
            for (int col = 0; col < this.size; col++) {
                res = res.concat(String.format("%6s", this.data[row][col]));

                if (col == this.size - 1) break;
                res = res.concat("  |  ");
            }

            if (row == this.size - 1) break;
            res = res.concat("\n");
        }
        return res;
    }
}
