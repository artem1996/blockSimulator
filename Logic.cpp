#include "Logic.h"

Logic::Logic(double mu, double k, double x10, double x20):
    mu(mu), k(k), x10(x10), x20(x20) {
    recalculate();
}

void Logic::setK(double k)
{
    this -> k = k;
    recalculate();
}

void Logic::setX10(double x10) {
    this -> x10 = x10;
    recalculate();
}

void Logic::setX20(double x20)
{
    this -> x20 = x20;
    recalculate();
}

void Logic::setMu(double mu)
{
    this->mu = mu;
    recalculate();
}

double Logic::getX1(double t)
{
    return a1 * cos(w1 * t) * h11 + a2 * cos(w2 * t) * h12;
}

double Logic::getX2(double t)
{
    return a1 * cos(w1 * t) * h21 + a2 * cos(w2 * t) * h22;
}


void Logic::recalculate() {
    h21 = (mu - 1 - sqrt((mu - 1) * (mu - 1) + mu)) / mu;
    h22 = (mu - 1 + sqrt((mu - 1) * (mu - 1) + mu)) / mu;
    w1 = sqrt(k / m2) * sqrt(mu + 1 + sqrt(( mu - 1) * (mu - 1) + mu));
    w2 = sqrt(k / m2) * sqrt(mu + 1 - sqrt(( mu - 1) * (mu - 1) + mu));
    a1 = (x10 * h22 - x20 * h12) / (h11 * h22 - h12 * h21);
    a2 = (x20 * h11 - x10 * h21) / (h11 * h22 - h12 * h21);
}
