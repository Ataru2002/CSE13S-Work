#include "mathlib.h"

//calculation using the Taylor Series
double Exp(double x) {
    double trm = 1.0;
    double sum = trm;
    double k = 1.0;
    while (trm > EPSILON) {
        trm *= Abs(x) / k;
        sum += trm;
        k++;
    }
    if (x > 0)
        return sum;
    return 1 / sum;
}

//calculation using the Taylor Series
double Sin(double x) {
    double s = 1.0;
    double total = x;
    double terms = x;
    double k = 3.0;
    while (Abs(terms) > EPSILON) {
        terms *= (x * x) / (k * (k - 1));
        k += 2;
        s *= -1;
        total += s * terms;
    }
    return total;
}

//Calculation using the Taylor Series
double Cos(double x) {
    double s = 1.0;
    double total = 1.0;
    double terms = 1.0;
    double k = 2.0;
    while (Abs(terms) > EPSILON) {
        terms *= (x * x) / (k * (k - 1));
        k += 2;
        s *= -1;
        total += s * terms;
    }
    return total;
}

//Calculation using Newton's Method
double Sqrt(double x) {
    double f = 1.0;
    // Scaling
    while (x > 1) {
        x /= 4.0;
        f *= 2.0;
    }
    double z = 0.0;
    double y = 1.0;
    while (Abs(y - z) > EPSILON) {
        z = y;
        y = (z + x / z) / 2;
    }
    return f * y;
}

//Calculation using Newton's Method
double Log(double x) {
    //scaling
    double e = 2.7182818284590455;
    double f = 0;
    while (x > e) {
        x /= e;
        f += 1.0;
    }
    double y = 1.0;
    double p = Exp(y);
    while (Abs(p - x) > EPSILON) {
        y = y + x / p - 1;
        p = Exp(y);
    }
    return f + y;
}
