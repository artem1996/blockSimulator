#ifndef LOGIC_H
#define LOGIC_H

#include "cmath"
#include "iostream"

class Logic
{
    const double m2 = 2;

    double k;
    double x10;
    double x20;
    double mu;

    double h11 = 1;
    double h21;
    double h12 = 1;
    double h22;
    double w1;
    double w2;
    double a1;
    double a2;
protected:
    void recalculate();
public:
    Logic(double mu, double k, double x10, double x20);
    void setK(double k);
    void setX10(double x10);
    void setX20(double x20);
    void setMu(double mu);
    double getX1(double t);
    double getX2(double t);
};

#endif // LOGIC_H
