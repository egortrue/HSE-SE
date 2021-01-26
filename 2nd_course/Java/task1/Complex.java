package task1;
import java.lang.Math;

public class Complex {
    private double re, im;

    Complex(double re, double im) {
        this.re = re;
        this.im = im;
    }

    Complex() {
        this(0, 0);
    }

    Complex(double re) {
        this(re, 0);
    }

    public double getRe() {
        return re;
    }

    public double getIm() {
        return im;
    }

    public void setRe(double re) {
        this.re = re;
    }

    public void setIm(double im) {
        this.im = im;
    }

    public Complex add(Complex other) {
        return new Complex(this.re + other.re,
                           this.im + other.im);
    }
    public Complex add(double num) {
        return this.add(new Complex(num));
    }

    public Complex sub(Complex other) {
        return new Complex(this.re - other.re,
                           this.im - other.im);
    }
    public Complex sub(double num) {
        return this.sub(new Complex(num));
    }

    public Complex mul(Complex other) {
        double re = this.re * other.re - this.im * other.im;
        double im = this.re * other.im + this.im * other.re;
        return new Complex(re, im);
    }
    public Complex mul(double num) {
        return this.mul(new Complex(num));
    }

    public Complex div(Complex other) {
        double re = (this.re * other.re + this.im * other.im) / (other.re * other.re + other.im * other.im);
        double im = (this.im * other.re - this.re * other.im) / (other.re * other.re + other.im * other.im);
        return new Complex(re, im);
    }
    public Complex div(double num) {
        return this.div(new Complex(num));
    }

    public String toTrigonometric() {

        double z_ = Math.sqrt(Math.pow(re, 2) + Math.pow(im, 2));
        String z = String.format("%.2f", z_);

        double alpha_;
        if (this.re != 0.0)
            alpha_ = Math.toDegrees(Math.atan(im/re));
        else
            alpha_ = 0;
        String alpha = String.format("%.2f", alpha_);

        return String.format("%1$s * (cos(%2$s\u00b0) + sin(%2$s\u00b0) * i)", z, alpha);
    }

    @Override
    public String toString() {
        if (this.im > 0)
            return String.format("%.2f", this.re) + " + " + String.format("%.2f", this.im) + "i";
        else if (this.im < 0)
            return String.format("%.2f", this.re) + " - " + String.format("%.2f", Math.abs(this.im)) + "i";
        else
            return String.format("%.2f", this.re);
    }
}
